/********************************************************************************************************
 * @file    tlkmw_btble_key_derivation.c
 *
 * @brief   This is the source file for TLSR/TL
 *
 * @author  Bluetooth Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#include "tl_common.h"
#include "tlkapi/tlkapi.h"
#include "common/types.h"
#include "common/utility.h"
#include "tlkmw_btble_key_derivation.h"
#include "stack/bt/host/bth/bth_smp.h"
#include "stack/bt/host/bth/bth_acl.h"
#include "stack/ble/host_v1/misc/inc/ble_crypto.h"
#include "stack/ble/host_v1/l2cap/smp/inc/ble_smp_format.h"
#include "stack/bt/host/bth/bth_l2cap.h"
#include "stack/ble/host_v1/l2cap/smp/inc/ble_smp.h"
#include "stack/bt/host/bth/bth_define.h"
#include "stack/bt/host/bth/bth_hcicmd.h"

#if (TLK_STK_BTBLE_CTKD)
static btble_key_derivation_t s_tlk_mw_btble_key_derivation;

/*Just support single BT+BLE ctkd now, adapt bt multipoint later.*/
extern void ble_host_smp_prepare_pairing_value(uint8_t *p_value);

static void tlkmw_btble_key_derivation_reset(void)
{
    tlksys_timer_destroy(TLKSYS_TASKID_HOST, &s_tlk_mw_btble_key_derivation.smp_timer);
    tmemset(&s_tlk_mw_btble_key_derivation, 0, sizeof(btble_key_derivation_t) - sizeof(TlkApiTimer_t));
}

static void tlkmw_btble_bredr_set_smp_over_bredr_state(uint16_t acl_handle, uint16_t state)
{
    if (acl_handle == s_tlk_mw_btble_key_derivation.acl_handle) {
        s_tlk_mw_btble_key_derivation.state = state;
        tlksys_task_setEvt(TLKSYS_TASKID_HOST, TLKSYS_TASK_EVT_HOST_APP2);
    }
}

static void tlkmw_btble_key_derivation_get_ltk_from_linkkey(bool use_ct2)
{
    uint8_t ltk_be[16];
    uint8_t link_key_be[16];

    ble_host_crypto_swap_block(link_key_be, s_tlk_mw_btble_key_derivation.link_key);
    ble_host_smp_toolbox_derivation_le_long_term_key(link_key_be, use_ct2, ltk_be);
    ble_host_crypto_swap_block(s_tlk_mw_btble_key_derivation.ltk, ltk_be);
}

static int tlkmw_btble_bredr_send_smp_data(uint8_t *p_data, uint16_t len)
{
    if (p_data == NULL || len == 0) {
        return -TLK_EPARAM;
    }

    return bth_l2cap_sendChannelData(s_tlk_mw_btble_key_derivation.acl_handle, BTH_L2CAP_SERCURITY_MANAGER_CID, NULL, 0, p_data, len);
}

static int tlkmw_btble_bredr_send_smp_pairing_failed(uint16_t acl_handle, uint8_t reason)
{
    if (acl_handle != s_tlk_mw_btble_key_derivation.acl_handle) {
        return -TLK_EPARAM;
    }

    struct btble_smp_pairing_failed smp_pairing_failed = {
        .code   = SMP_OPCODE_PAIRING_FAILED,
        .reason = reason,
    };

    return tlkmw_btble_bredr_send_smp_data((uint8_t *)&smp_pairing_failed, sizeof(struct btble_smp_pairing_failed));
    /*TODO: stop timer*/
}

static void tlkmw_btble_bredr_encryption_complete(void)
{
    /*Check BR/EDR if use secure connection.*/
    tlkapi_printf(BTBLE_KEY_DERIVATION_LOG_EN, "tlkmw_btble_bredr_encryption_complete encryption_enabled:%d, current_role:%d", s_tlk_mw_btble_key_derivation.encryption_enabled,
                  s_tlk_mw_btble_key_derivation.role);

    if (s_tlk_mw_btble_key_derivation.encryption_enabled != 2) {
        return;
    }

    if (s_tlk_mw_btble_key_derivation.role == BTH_ROLE_SLAVE) {
        tlkmw_btble_bredr_set_smp_over_bredr_state(s_tlk_mw_btble_key_derivation.acl_handle, SM_BR_EDR_RESPONDER_W4_PAIRING_REQUEST);
    } else if (s_tlk_mw_btble_key_derivation.role == BTH_ROLE_MASTER) {
        // Check if remote supports SMP over BR/EDR channel.
        if (s_tlk_mw_btble_key_derivation.simple_pairing_complete &&
            bth_acl_getPeerFixChannelSupport(s_tlk_mw_btble_key_derivation.acl_handle) & BTH_L2CAP_EXT_CHANNEL_SERCURITY_MANAGER) {
            tlkmw_btble_bredr_set_smp_over_bredr_state(s_tlk_mw_btble_key_derivation.acl_handle, SM_BR_EDR_INITIATOR_SEND_PAIRING_REQUEST);
        }
    }
}

static void tlkmw_btble_smp_over_bredr_timer(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)pTimer;
    (void)userArg;
    tlkapi_printf(BTBLE_KEY_DERIVATION_LOG_EN, "tlkmw_btble_smp_over_bredr_timer SMP OVER BR/EDR Timeout!");
    /*TODO: Notify to APP.*/
    tlkmw_btble_key_derivation_reset();
}

static void tlkmw_btble_smp_over_bredr_state_machine(void)
{
    tlk_printf("tlkmw_btble_smp_over_bredr_state_machine state[%d]", s_tlk_mw_btble_key_derivation.state);
    if (SM_BR_EDR_ENCRYPTION_COMPLETE == s_tlk_mw_btble_key_derivation.state) {
        tlkmw_btble_bredr_encryption_complete();
    } else if (SM_BR_EDR_INITIATOR_SEND_PAIRING_REQUEST == s_tlk_mw_btble_key_derivation.state) {
        struct btble_smp_pairing smp_pairing_request = {
            .code = SMP_OPCODE_PAIRING_REQUEST,
        };

        ble_host_smp_prepare_pairing_value(&smp_pairing_request.io_capability);
        smp_pairing_request.responder_key.linkKey = false;
        smp_pairing_request.initiator_key.linkKey = false;

        if (tlkmw_btble_bredr_send_smp_data((uint8_t *)&smp_pairing_request, sizeof(struct btble_smp_pairing)) == TLK_ENONE) {
            tlkmw_btble_bredr_set_smp_over_bredr_state(s_tlk_mw_btble_key_derivation.acl_handle, SM_BR_EDR_INITIATOR_W4_PAIRING_RESPONSE);
        }
    } else if (SM_BR_EDR_RESPONDER_SEND_PAIRING_RESPONSE == s_tlk_mw_btble_key_derivation.state) {
        struct btble_smp_pairing smp_pairing_response = {
            .code = SMP_OPCODE_PAIRING_RESPONSE,
        };

        ble_host_smp_prepare_pairing_value(&smp_pairing_response.io_capability);
        smp_pairing_response.initiator_key.linkKey = false;
        smp_pairing_response.responder_key.linkKey = false;

        if (tlkmw_btble_bredr_send_smp_data((uint8_t *)&smp_pairing_response, sizeof(struct btble_smp_pairing)) == TLK_ENONE) {
            tlkmw_btble_bredr_set_smp_over_bredr_state(s_tlk_mw_btble_key_derivation.acl_handle, SM_BR_EDR_RESPONDER_GENERATE_LTK_FROM_LINKKEY);
        }
    } else if (SM_BR_EDR_RESPONDER_GENERATE_LTK_FROM_LINKKEY == s_tlk_mw_btble_key_derivation.state) {
        tlkmw_btble_key_derivation_get_ltk_from_linkkey(s_tlk_mw_btble_key_derivation.use_ct2_flag);
        tlkmw_btble_bredr_set_smp_over_bredr_state(s_tlk_mw_btble_key_derivation.acl_handle, SM_BR_EDR_RESPONDER_SEND_IDENTITY_INFORMATION);
    } else if (SM_BR_EDR_INITIATOR_GENERATE_LTK_FROM_LINKKEY == s_tlk_mw_btble_key_derivation.state) {
        tlkmw_btble_key_derivation_get_ltk_from_linkkey(s_tlk_mw_btble_key_derivation.use_ct2_flag);
        tlkmw_btble_bredr_set_smp_over_bredr_state(s_tlk_mw_btble_key_derivation.acl_handle, SM_BR_EDR_INITIATOR_W4_IDENTITY_INFORMATION);
    } else if (SM_BR_EDR_RESPONDER_SEND_IDENTITY_INFORMATION == s_tlk_mw_btble_key_derivation.state ||
               SM_BR_EDR_INITIATOR_SEND_IDENTITY_INFORMATION == s_tlk_mw_btble_key_derivation.state) {
        struct smp_identity_information_format identity_info = {
            .code = SMP_OPCODE_IDENTITY_INFORMATION,
            .irk  = {0},
        };
        ble_host_smp_get_local_irk(identity_info.irk);

        if (tlkmw_btble_bredr_send_smp_data((uint8_t *)&identity_info, sizeof(struct smp_identity_information_format)) == TLK_ENONE) {
            tlkmw_btble_bredr_set_smp_over_bredr_state(s_tlk_mw_btble_key_derivation.acl_handle,
                                                       s_tlk_mw_btble_key_derivation.state == SM_BR_EDR_RESPONDER_SEND_IDENTITY_INFORMATION ?
                                                           SM_BR_EDR_RESPONDER_SEND_IDENTITY_ADDRESS_INFORMATION :
                                                           SM_BR_EDR_INITIATOR_SEND_IDENTITY_ADDRESS_INFORMATION);
        }
    } else if (SM_BR_EDR_RESPONDER_SEND_IDENTITY_ADDRESS_INFORMATION == s_tlk_mw_btble_key_derivation.state ||
               SM_BR_EDR_INITIATOR_SEND_IDENTITY_ADDRESS_INFORMATION == s_tlk_mw_btble_key_derivation.state) {
        struct smp_identity_address_information_format identity_addr_info = {
            .code     = SMP_OPCODE_IDENTITY_ADDRESS_INFORMATION,
            .addr     = {0},
            .addrType = 0,
        };

        extern int tlkmdi_tinySql_getBtMacAddress(uint8_t * recBuffer);
        uint8_t    local_addr[6];
        tlkmdi_tinySql_getBtMacAddress(local_addr);
        tmemcpy(identity_addr_info.addr, local_addr, 6);

        if (tlkmw_btble_bredr_send_smp_data((uint8_t *)&identity_addr_info, sizeof(struct smp_identity_address_information_format)) == TLK_ENONE) {
            tlkmw_btble_bredr_set_smp_over_bredr_state(
                s_tlk_mw_btble_key_derivation.acl_handle,
                s_tlk_mw_btble_key_derivation.state == SM_BR_EDR_RESPONDER_SEND_IDENTITY_ADDRESS_INFORMATION ? SM_BR_EDR_RESPONDER_W4_IDENTITY_INFORMATION : SM_GENERAL_IDLE);
        }
    }
}

static void tlkmw_btble_smp_over_bredr_receive_pairing_request(uint16_t handle, uint8_t *pData, uint16_t dataLen)
{
    if (handle != s_tlk_mw_btble_key_derivation.acl_handle || dataLen < sizeof(struct btble_smp_pairing)) {
        return;
    }

    if (s_tlk_mw_btble_key_derivation.state != SM_BR_EDR_RESPONDER_W4_PAIRING_REQUEST) {
        tlkmw_btble_bredr_send_smp_pairing_failed(handle, SMP_FAILED_UNSPECIFIED_REASON);
    }

    /*Check bredr linkkey type and ltk generate info.*/
    if (s_tlk_mw_btble_key_derivation.link_key_type != 0x07 && s_tlk_mw_btble_key_derivation.link_key_type != 0x08) {
        tlkmw_btble_bredr_send_smp_pairing_failed(handle, SMP_FAILED_CROSS_TRANSPORT_KEY_DERIVATION_GENERATION_NOT_ALLOWED);
    }

    bool linkkey_authenticated = s_tlk_mw_btble_key_derivation.link_key_type == 0x08 ? true : false;
    (void)linkkey_authenticated;
    /*TODO: If linkkey is not authenticated, but ltk is authenticated, then not allowed ctkd.*/

    struct btble_smp_pairing smp_pairing_response = {
        .code = SMP_OPCODE_PAIRING_RESPONSE,
    };

    ble_host_smp_prepare_pairing_value(&smp_pairing_response.io_capability);
    smp_pairing_response.initiator_key.linkKey = false;
    smp_pairing_response.responder_key.linkKey = false;

    s_tlk_mw_btble_key_derivation.use_ct2_flag = (pData[3] >> 5 & 0x01) & smp_pairing_response.auth_req.ct2;

    tlk_printf("use_ct2_flag:%d,  local_ct2:%d", s_tlk_mw_btble_key_derivation.use_ct2_flag, smp_pairing_response.auth_req.ct2);

    if (tlkmw_btble_bredr_send_smp_data((uint8_t *)&smp_pairing_response, sizeof(struct btble_smp_pairing)) == TLK_ENONE) {
        tlkmw_btble_bredr_set_smp_over_bredr_state(handle, SM_BR_EDR_RESPONDER_GENERATE_LTK_FROM_LINKKEY);
    } else {
        tlkmw_btble_bredr_set_smp_over_bredr_state(handle, SM_BR_EDR_RESPONDER_SEND_PAIRING_RESPONSE);
    }
}

static void tlkmw_btble_smp_over_bredr_receive_pairing_response(uint16_t handle, uint8_t *pData, uint16_t dataLen)
{
    if (handle != s_tlk_mw_btble_key_derivation.acl_handle || dataLen < (sizeof(struct btble_smp_pairing))) {
        return;
    }

    if (SM_BR_EDR_INITIATOR_W4_PAIRING_RESPONSE != s_tlk_mw_btble_key_derivation.state) {
        tlkmw_btble_bredr_send_smp_pairing_failed(handle, SMP_FAILED_UNSPECIFIED_REASON);
        return;
    }

    struct btble_smp_pairing *smp_pairing_response = (struct btble_smp_pairing *)pData;

    struct btble_smp_pairing local_smp_pairing;
    ble_host_smp_prepare_pairing_value(&local_smp_pairing.io_capability);
    local_smp_pairing.responder_key.linkKey = false;
    local_smp_pairing.initiator_key.linkKey = false;

    s_tlk_mw_btble_key_derivation.use_ct2_flag = local_smp_pairing.auth_req.ct2 & smp_pairing_response->auth_req.ct2;
    tlkmw_btble_bredr_set_smp_over_bredr_state(handle, SM_BR_EDR_INITIATOR_GENERATE_LTK_FROM_LINKKEY);
}

static void tlkmw_btble_smp_over_bredr_receive_identity_information(uint16_t handle, uint8_t *pData, uint16_t dataLen)
{
    if (handle != s_tlk_mw_btble_key_derivation.acl_handle || dataLen < sizeof(struct btble_smp_identity_information)) {
        return;
    }

    if (s_tlk_mw_btble_key_derivation.state == SM_BR_EDR_RESPONDER_W4_IDENTITY_INFORMATION) {
        tmemcpy(s_tlk_mw_btble_key_derivation.peer_irk, pData + 1, dataLen);
        tlkmw_btble_bredr_set_smp_over_bredr_state(handle, SM_BR_EDR_RESPONDER_W4_IDENTITY_ADDRESS_INFORMATION);
    } else if (s_tlk_mw_btble_key_derivation.state == SM_BR_EDR_INITIATOR_W4_IDENTITY_INFORMATION) {
        tmemcpy(s_tlk_mw_btble_key_derivation.peer_irk, pData + 1, dataLen);
        tlkmw_btble_bredr_set_smp_over_bredr_state(handle, SM_BR_EDR_INITIATOR_W4_IDENTITY_ADDRESS_INFORMATION);
    } else {
        tlkmw_btble_bredr_send_smp_pairing_failed(handle, SMP_FAILED_UNSPECIFIED_REASON);
    }
}

static void tlkmw_btble_smp_over_bredr_receive_identity_address_information(uint16_t handle, uint8_t *pData, uint16_t dataLen)
{
    if (handle != s_tlk_mw_btble_key_derivation.acl_handle || dataLen < sizeof(struct btble_smp_identity_address_information)) {
        return;
    }

    if (s_tlk_mw_btble_key_derivation.state == SM_BR_EDR_RESPONDER_W4_IDENTITY_ADDRESS_INFORMATION) {
        tlkmw_btble_bredr_set_smp_over_bredr_state(handle, SM_GENERAL_IDLE);
    } else if (s_tlk_mw_btble_key_derivation.state == SM_BR_EDR_INITIATOR_W4_IDENTITY_ADDRESS_INFORMATION) {
        tlkmw_btble_bredr_set_smp_over_bredr_state(handle, SM_BR_EDR_INITIATOR_SEND_IDENTITY_INFORMATION);
    } else {
        tlkmw_btble_bredr_send_smp_pairing_failed(handle, SMP_FAILED_UNSPECIFIED_REASON);
        return;
    }

    extern void ble_host_smp_store_new_share_pairing(uint8_t addr_type, uint8_t addr[6], uint8_t irk[16], uint8_t ltk[16]);
    ble_host_smp_store_new_share_pairing(pData[1], pData + 2, s_tlk_mw_btble_key_derivation.peer_irk, s_tlk_mw_btble_key_derivation.ltk);
}

void bth_smp_recvHandler(uint16_t handle, uint8_t *pData, uint16_t dataLen)
{
    if (handle != s_tlk_mw_btble_key_derivation.acl_handle) {
        return;
    }

    switch (pData[0]) {
    case SMP_OPCODE_PAIRING_REQUEST:
    {
        tlkmw_btble_smp_over_bredr_receive_pairing_request(handle, pData, dataLen);
    } break;
    case SMP_OPCODE_PAIRING_RESPONSE:
    {
        tlkmw_btble_smp_over_bredr_receive_pairing_response(handle, pData, dataLen);
    } break;
    case SMP_OPCODE_IDENTITY_INFORMATION:
    {
        tlkmw_btble_smp_over_bredr_receive_identity_information(handle, pData, dataLen);
    } break;
    case SMP_OPCODE_IDENTITY_ADDRESS_INFORMATION:
    {
        tlkmw_btble_smp_over_bredr_receive_identity_address_information(handle, pData, dataLen);
    } break;
    case SMP_OPCODE_PAIRING_FAILED:
        tlkmw_btble_key_derivation_reset();
        break;
    default:
        break;
    }
}

void tlkmw_btble_key_derivation_store_link_key(uint8_t bd_addr[6], uint8_t link_key[16], uint8_t key_type)
{
    if (tmemcmp(bd_addr, s_tlk_mw_btble_key_derivation.peer_addr, 6) == 0) {
        s_tlk_mw_btble_key_derivation.link_key_type = key_type;
        tmemcpy(s_tlk_mw_btble_key_derivation.link_key, link_key, 16);
    }
}

void tlkmw_btble_bredr_set_encryption_enabled(uint16_t acl_handle, uint8_t encryption_enabled)
{
    if (s_tlk_mw_btble_key_derivation.acl_handle == acl_handle) {
        s_tlk_mw_btble_key_derivation.encryption_enabled = encryption_enabled;
        tlkmw_btble_bredr_set_smp_over_bredr_state(acl_handle, SM_BR_EDR_ENCRYPTION_COMPLETE);
    }
}

void tlkmw_btble_bredr_set_simple_pairing_complete(uint8_t bd_addr[6])
{
    if (tmemcmp(s_tlk_mw_btble_key_derivation.peer_addr, bd_addr, 6) == 0) {
        s_tlk_mw_btble_key_derivation.simple_pairing_complete = true;
    }
}

void tlkmw_btble_bredr_set_connect_complete(uint16_t aclHandle, uint8_t peer_addr[6])
{
    s_tlk_mw_btble_key_derivation.acl_handle = aclHandle;
    s_tlk_mw_btble_key_derivation.role       = bth_acl_get_current_role(aclHandle);
    tmemcpy(s_tlk_mw_btble_key_derivation.peer_addr, peer_addr, 6);
    tlksys_task_regEvtCB(TLKSYS_TASKID_HOST, TLKSYS_TASK_EVT_HOST_APP2, tlkmw_btble_smp_over_bredr_state_machine);
    tlkmw_btble_bredr_set_smp_over_bredr_state(aclHandle, SM_BR_EDR_W4_ENCRYPTION_COMPLETE);
    tlksys_timer_createStatic(TLKSYS_TASKID_HOST, &s_tlk_mw_btble_key_derivation.smp_timer, TLKMDI_SMP_OVER_BREDR_TIMEOUT, false, tlkmw_btble_smp_over_bredr_timer, NULL);
}

void tlkmw_btble_key_derivation_init(bool enable)
{
    tmemset(&s_tlk_mw_btble_key_derivation, 0, sizeof(btble_key_derivation_t));
    ble_host_smp_derivation_key_enable(enable);
    bth_hci_sendWriteLeHostSupportCmd(0x01);
    bth_hci_sendSetLeFeatSuppCmd(0x01);
    bth_l2cap_setExtChannelBits(BTH_L2CAP_EXT_CHANNEL_SERCURITY_MANAGER);
}
#else
void tlkmw_btble_key_derivation_init(bool enable)
{
    (void)enable;
}

void tlkmw_btble_key_derivation_store_link_key(uint8_t bd_addr[6], uint8_t link_key[16], uint8_t key_type)
{
    (void)bd_addr;
    (void)link_key;
    (void)key_type;
}

void tlkmw_btble_bredr_set_encryption_enabled(uint16_t acl_handle, uint8_t encryption_enabled)
{
    (void)acl_handle;
    (void)encryption_enabled;
}

void tlkmw_btble_bredr_set_simple_pairing_complete(uint8_t bd_addr[6])
{
    (void)bd_addr;
}

void tlkmw_btble_bredr_set_connect_complete(uint16_t aclHandle, uint8_t peer_addr[6])
{
    (void)aclHandle;
    (void)peer_addr;
}

#endif