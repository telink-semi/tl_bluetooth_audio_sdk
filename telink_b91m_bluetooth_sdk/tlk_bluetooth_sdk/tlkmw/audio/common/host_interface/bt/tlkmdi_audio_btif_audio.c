/********************************************************************************************************
 * @file    tlkmdi_audio_btif_audio.c
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
#include "drivers.h"
#include "tlkapi/tlkapi.h"
#include "tlkmw/tlkmw.h"
#include "core/tlkmdi_audio_btif_inner.h"
#include "stack/bt/host/bth/bth_stdio.h"
#include "stack/bt/host/btp/btp_stdio.h"
#include "stack/tpsll/tpsll.h"
#include "stack/tpsll/host/tpsll_hcicmd.h"
#if (TLK_MW_AUDIO_ENABLE)

tlkmdi_audio_btif_ctrl_t s_tlkmdi_audio_btif_ctrl = {0};

/**
 * @brief       Set BTC performance mode
 * @param[in]   handle - ACL connection handle
 * @param[in]   enable - Enable or disable performance mode
 * @return      None
 */
static void tlkmw_audio_btif_setBtcPerformanceMode(uint16_t handle, uint8_t enable)
{
    (void)handle;
    (void)enable;
#if (TLK_STK_BT_ENABLE)
    uint8_t audio_packet_num = 41;
    uint8_t codec_type       = tlkmw_audio_btif_music_get_avdtp_seid_type();
#if TLKBTP_CFG_A2DPSNK_ENABLE
    uint32_t sample_rate = btp_a2dpsnk_getSampleRate(handle);
#else
    uint32_t sample_rate = 48000;
#endif
    if (codec_type == SEPID_SBC) {
        if (sample_rate == 44100) {
            // 600ms / ((1/44100) * 128 * 5) = 600/14.515 = 41.336
            audio_packet_num = 41;
        } else if (sample_rate == 48000) {
            // 600ms / ((1/48000) * 128 * 5) = 600/13.33 = 45.01
            audio_packet_num = 45;
        }
    } else if (codec_type == SEPID_AAC) {
        if (sample_rate == 44100) {
            // 600ms / ((1/44100) * 1024) = 600/23.282 = 25.77
            audio_packet_num = 25;
        } else if (sample_rate == 48000) {
            // 600ms / ((1/48000) * 1024) = 600/21.33 = 28.13
            audio_packet_num = 28;
        }
    }
    bth_hci_sendSetMoreDataModeCmd(handle, enable, audio_packet_num);
#endif
}

/**
 * @brief       Enter music mode
 * @param[in]   handle - ACL connection handle
 * @return      None
 */
static inline void tlkmw_audio_btif_enter_music(uint16_t handle)
{
    // bth_hci_sendSetMaxSlotCmd(handle, 0x05);
    tlkmw_audio_btif_setBtcPerformanceMode(handle, true);
#if (TLKSTK_BT_TPS_ENABLE)
    bth_acl_handle_t *pHandle = bth_handle_getUsedAcl(handle);
    if (pHandle->curRole == BTH_ROLE_SLAVE) {
#if (TLK_STK_TPH_ENABLE)
        tpsll_hci_sendSetHeadsetCurModeCmd(TPH_HOST_MODE_BT_MUSIC);
#elif (TLK_STK_TPT_ENABLE)
        tpsll_hci_sendSetHeadsetCurModeCmd(TPT_HOST_MODE_BT_MUSIC);
#endif
    }
#endif
#if (TLK_BT_MULTIPNT_ENABLE)
    uint8_t pData[2];
    pData[0] = 0; //inqScan
    pData[1] = 1; //pageScan
    tlksys_sendMsg(TLKSYS_TASKID_HOST, TLKSYS_BT_MSGID_SET_SCAN, pData, 2);
#endif
}

/**
 * @brief       Leave music mode
 * @param[in]   handle - ACL connection handle
 * @return      None
 */
static inline void tlkmw_audio_btif_leave_music(uint16_t handle)
{
    tlkmw_audio_btif_setBtcPerformanceMode(handle, false);
#if (TLKSTK_BT_TPS_ENABLE)
    bth_acl_handle_t *pHandle = bth_handle_getUsedAcl(handle);
    if (pHandle->curRole == BTH_ROLE_SLAVE) {
#if (TLK_STK_TPH_ENABLE)
        tpsll_hci_sendClearHeadsetCurModeCmd(TPH_HOST_MODE_BT_MUSIC);
#elif (TLK_STK_TPT_ENABLE)
        tpsll_hci_sendClearHeadsetCurModeCmd(TPT_HOST_MODE_BT_MUSIC);
#endif
    }
#endif
#if (TLK_BT_MULTIPNT_ENABLE)
    uint8_t pData[2];
    pData[0] = 1; //inqScan
    pData[1] = 1; //pageScan
    tlksys_sendMsg(TLKSYS_TASKID_HOST, TLKSYS_BT_MSGID_SET_SCAN, pData, 2);
#endif
}

/**
 * @brief       Enter voice mode
 * @param[in]   handle - ACL connection handle
 * @return      None
 */
static inline void tlkmw_audio_btif_enter_vocie(uint16_t handle)
{
    (void)handle;
#if (TLK_BT_MULTIPNT_ENABLE)
    uint8_t pData[2];
    pData[0] = 0; //inqScan
    pData[1] = 1; //pageScan
    tlksys_sendMsg(TLKSYS_TASKID_HOST, TLKSYS_BT_MSGID_SET_SCAN, pData, 2);
#endif
}

/**
 * @brief       Leave voice mode
 * @param[in]   handle - ACL connection handle
 * @return      None
 */
static inline void tlkmw_audio_btif_leave_vocie(uint16_t handle)
{
    (void)handle;
#if (TLK_BT_MULTIPNT_ENABLE)
    uint8_t pData[2];
    pData[0] = 1; //inqScan
    pData[1] = 1; //pageScan
    tlksys_sendMsg(TLKSYS_TASKID_HOST, TLKSYS_BT_MSGID_SET_SCAN, pData, 2);
#endif
}

/**
 * @brief       Inform host that audio is enabled
 * @param[in]   handle - ACL connection handle
 * @param[in]   is_music - Music or voice flag
 * @return      TLK_ENONE: success; TLK_EREPEAT: repeat operation
 */
int tlkmw_audio_btif_inform_host_audio_en(uint16_t handle, uint8_t is_music)
{
    if (s_tlkmdi_audio_btif_ctrl.handle != 0) {
        return -TLK_EREPEAT;
    }
    s_tlkmdi_audio_btif_ctrl.handle   = handle;
    s_tlkmdi_audio_btif_ctrl.is_music = is_music;
    if (is_music) {
        tlkmw_audio_btif_enter_music(handle);
    } else {
        tlkmw_audio_btif_enter_vocie(handle);
    }
    return TLK_ENONE;
}

/**
 * @brief       Inform host that audio is disabled
 * @param[in]   handle - ACL connection handle
 * @return      TLK_ENONE: success; TLK_ENOOBJECT: object not found
 */
int tlkmw_audio_btif_inform_host_audio_dis(uint16_t handle)
{
    if (s_tlkmdi_audio_btif_ctrl.handle != handle) {
        return -TLK_ENOOBJECT;
    }
    s_tlkmdi_audio_btif_ctrl.handle = 0;
    if (s_tlkmdi_audio_btif_ctrl.is_music) {
        tlkmw_audio_btif_leave_music(handle);
    } else {
        tlkmw_audio_btif_leave_vocie(handle);
    }
    return TLK_ENONE;
}

/**
 * @brief       Get AVDTP SEID type for music
 * @param[in]   None
 * @return      Codec type
 */
uint8_t tlkmw_audio_btif_music_get_avdtp_seid_type(void)
{
#if TLKBTP_CFG_A2DPSNK_ENABLE
    uint16_t handle     = s_tlkmdi_audio_btif_ctrl.handle;
    uint8_t  codec_type = btp_a2dpsnk_getCurrCodec(handle);
    if (codec_type == 255) {
        return SEPID_LHDC;
    }
    return codec_type;
#else
    return 0;
#endif
}

/**
 * @brief       Get current playing handle
 * @param[in]   None
 * @return      Current handle
 */
uint16_t tlkmw_audio_btif_getPlayHandle(void)
{
    return s_tlkmdi_audio_btif_ctrl.handle;
}

/**
 * @brief       Check if handle is AG
 * @param[in]   handle - ACL connection handle
 * @return      True if handle is AG, otherwise false
 */
bool tlkmw_audio_btif_handle_is_ag(uint16_t handle)
{
    (void)handle;
#if TLKBTP_CFG_HFP_ENABLE
    if (handle == 0) {
        return false;
    }

    return handle == btp_hfp_getAgHandle();
#else
    return false;
#endif
}

/**
 * @brief  Trigger one avrcp key press and release.
 *
 * @param[in]  handle: acl handle
 * @param[in]  key: avrcp key id,refer to AUD_BTIF_AVRCP_KEYID_ENUM
 *
 * @returns Returning TLK_ENONE(0x00) means the send process success.
 *          If others value is returned means the send process fail.
 */
/**
 * @brief       Trigger one avrcp key press and release
 * @param[in]   handle - ACL handle
 * @param[in]   key - AVRCP key ID, refer to AUD_BTIF_AVRCP_KEYID_ENUM
 * @return      TLK_ENONE: success; Others: failure
 */
int tlkmdi_audio_btif_avrcp_sendKey(uint16_t handle, uint8_t key)
{
    uint8_t buffer[4];
    uint8_t buffLen   = 0;
    buffer[buffLen++] = key;
    buffer[buffLen++] = (handle & 0x00FF);
    buffer[buffLen++] = (handle & 0xFF00) >> 8;
    return tlksys_sendMsg(TLKSYS_TASKID_HOST, TLKSYS_BT_MSGID_AVRCP_SEND_KEY, buffer, buffLen);
}

/**
 * @brief  Get is allowed slave headset create sco when hfp is not connected.
 *
 * @param[in]  handle acl handle
 *
 * @returns True means allowed.
 */
/**
 * @brief       Get is allowed slave headset create sco when hfp is not connected
 * @param[in]   handle - ACL handle
 * @return      True means allowed
 */
bool tlkmdi_audio_btif_allowedCreateScoWithoutHfp(uint16_t handle)
{
    (void)handle;
#if (TLKMW_INTERPHONE_EN)
    if (tlkmdi_audio_hostif_isInProductTest()) {
        return true;
    }
    if (handle == btp_hfp_getAgHandle()) {
        return false;
    } // temp fix, for cover interphone scene.
#if (TLK_CHECK_REMOTE_DEV)
    bth_aclGetNameReportEvt_t *info = (bth_aclGetNameReportEvt_t *)tlkmdi_btacl_get_remote_dev();
    tlk_printf("tlkmdi_audio_btif_allowedCreateScoWithoutHfp:info_handle[%d] handle[%d] isCustomerDev[%d]", info->handle, handle, info->isCustomerDev);
    if (info->handle == handle) {
        return false;
    }
#endif //TLK_CHECK_REMOTE_DEV
#endif
    return true;
}

/**
 * @brief       Set increase peer power max command
 * @param[in]   handle - ACL connection handle
 * @param[in]   enable - Enable or disable the command
 * @return      Command execution status
 */
int tlkmdi_audio_btif_setIncPeerPwrMax(uint16_t handle, uint8_t enable)
{
    return bth_hci_sendSetIncPeerPwrMaxCmd(handle, enable);
}

#endif // #if (TLK_MW_AUDIO_ENABLE)
