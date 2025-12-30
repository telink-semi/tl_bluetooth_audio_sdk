/********************************************************************************************************
 * @file    tlkapp_lemgrMsg.c
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
#include "stack/ble/ble.h"

#include "tlkapi/tlkapi.h"

#if (TLKAPP_LEMGR_ENABLE)
#include "tlkapp/tlkapp.h"

/**
 * @brief   Send common response message.
 * @param[in]   cmdID    - Command ID.
 * @param[in]   status   - Status of the response.
 * @param[in]   reason   - Reason code.
 * @param[in]   pData    - Pointer to the data to send.
 * @param[in]   dataLen  - Length of the data.
 * @return      Status of the operation.
 */
int tlkapp_lemgr_sendCommRsp(uint8_t cmdID, uint8_t status, uint8_t reason, uint8_t *pData, uint8_t dataLen)
{
    uint8_t buffer[dataLen + TLKPTI_SYS_SERIAL_HEADLEN];
    buffer[0] = TLKPRT_COMM_PTYPE_RSP;
    buffer[1] = TLKPRT_COMM_MTYPE_LE;
    buffer[2] = cmdID;
    buffer[3] = status;
    buffer[4] = reason;
    tmemcpy(buffer + TLKPTI_SYS_SERIAL_HEADLEN, pData, dataLen);
    return tlksys_sendMsg(TLKSYS_TASKID_SYSTEM, TLKSYS_SYS_MSGID_SERIAL_SEND, buffer, dataLen + TLKPTI_SYS_SERIAL_HEADLEN);
}

/**
 * @brief   Send common event message.
 * @param[in]   evtID    - Event ID.
 * @param[in]   pData    - Pointer to the data to send.
 * @param[in]   dataLen  - Length of the data.
 * @return      Status of the operation.
 */
int tlkapp_lemgr_sendCommEvt(uint8_t evtID, uint8_t *pData, uint8_t dataLen)
{
    uint8_t buffer[dataLen + TLKPTI_SYS_SERIAL_HEADLEN];
    buffer[0] = TLKPRT_COMM_PTYPE_EVT;
    buffer[1] = TLKPRT_COMM_MTYPE_LE;
    buffer[2] = evtID;
    tmemcpy(buffer + TLKPTI_SYS_SERIAL_HEADLEN, pData, dataLen);
    return tlksys_sendMsg(TLKSYS_TASKID_SYSTEM, TLKSYS_SYS_MSGID_SERIAL_SEND, buffer, dataLen + TLKPTI_SYS_SERIAL_HEADLEN);
}


#if (TLKAPP_LEMGR_EXT_SCAN_ENABLE)
static TlkApiTimerHandle_t s_lemgr_ext_scan_timer = NULL;
static uint32_t            s_lemgr_ext_scan_count = 0;

static void tlkapp_le_mgr_ext_scan_handler(uint32_t event_id, const void *data, void *user_data);

static const struct ble_host_gap_evt_subscribe_param s_tlkapp_le_mgr_ext_scan_param = {
    .gap_le_scan_mask = 1,
    .handler          = tlkapp_le_mgr_ext_scan_handler,
};

static struct ble_host_gap_evt_subscribe s_tlkapp_le_mgr_ext_scan = {
    .next_handler = NULL,
    .param        = &s_tlkapp_le_mgr_ext_scan_param,
};

/**
 * @brief   Extended scan timer callback function.
 * @param[in]   handle   - Timer handle.
 * @param[in]   userArg  - User argument.
 * @return      None.
 */
static void tlkapp_lemgr_ext_scan_timer_callback(TlkApiTimerHandle_t handle, void *userArg)
{
    (void)userArg;
    s_lemgr_ext_scan_count--;
    if (s_lemgr_ext_scan_count > 0) {
        tlksys_timer_start(TLKSYS_TASKID_LEMGR, handle);
    } else {
        ble_host_gap_ext_scan_disable();
    }

    tlkapi_printf(TLKAPP_LEMGR_LOG_ENABLE, "BLE extend scan timeout, %ds", s_lemgr_ext_scan_count * 5);
}

struct tlkapp_lemgr_ext_scan_report
{
    uint8_t flags;
    uint8_t device_name_len;
    uint8_t device_name[31];
};

/**
 * @brief   Unpack advertising data during extended scan.
 * @param[in]   length  - Length of the data.
 * @param[in]   type    - Type of the data.
 * @param[in]   value   - Pointer to the data value.
 * @param[in]   user    - User data pointer.
 * @return      1 on success, 0 on failure.
 */
static int tlkapp_lemgr_ext_scan_adv_unpack(uint8_t length, uint8_t type, const uint8_t *value, void *user)
{
    struct tlkapp_lemgr_ext_scan_report *p_report = (struct tlkapp_lemgr_ext_scan_report *)user;
    if (type == DT_FLAGS) {
        p_report->flags = value[0];
    } else if (type == DT_COMPLETE_LOCAL_NAME) {
        p_report->device_name_len = length - 1;
        memcpy(p_report->device_name, value, min(sizeof(p_report->device_name), p_report->device_name_len));
    }

    return 1;
}

/**
 * @brief   Handle undirected advertising report during extended scan (weak function).
 * @param[in]   p_adv_report  - Pointer to the advertising report.
 * @return      None.
 */
__attribute__((weak)) void tlkapp_lemgr_ext_scan_undirected_report_handler(const struct ble_host_gap_evt_scan_adv_report_undirected *p_adv_report)
{
    struct tlkapp_lemgr_ext_scan_report report;
    memset(&report, 0, sizeof(report));
    int32_t res = ltv_unpack(p_adv_report->data, p_adv_report->data_len, tlkapp_lemgr_ext_scan_adv_unpack, &report);

    if (res != LTV_UNPACK_SUCCESS) {
        return;
    }
    if ((report.flags & (FLAGS_LE_LIMITED_DISCOVERABLE_MODE | FLAGS_LE_GENERAL_DISCOVERABLE_MODE)) == 0x00) {
        return;
    }

    tlkapi_printf(0, "BLE extend scan report, flags:0x%02x, device_name:%s", report.flags, report.device_name);

    tlkapp_lemgr_sendExtScanDataEvt(p_adv_report->addr_type, p_adv_report->addr, report.device_name, report.device_name_len);
}

/**
 * @brief   Handle directed advertising report during extended scan (weak function).
 * @param[in]   p_adv_report  - Pointer to the advertising report.
 * @return      None.
 */
__attribute__((weak)) void tlkapp_lemgr_ext_scan_directed_report_handler(const struct ble_host_gap_evt_scan_adv_report_directed *p_adv_report)
{
    struct tlkapp_lemgr_ext_scan_report report;
    memset(&report, 0, sizeof(report));
    int32_t res = ltv_unpack(p_adv_report->data, p_adv_report->data_len, tlkapp_lemgr_ext_scan_adv_unpack, &report);

    if (res != LTV_UNPACK_SUCCESS) {
        return;
    }
    if ((report.flags & (FLAGS_LE_LIMITED_DISCOVERABLE_MODE | FLAGS_LE_GENERAL_DISCOVERABLE_MODE)) == 0x00) {
        return;
    }

    tlkapi_printf(0, "BLE extend scan report, flags:0x%02x, device_name:%s", report.flags, report.device_name);

    tlkapp_lemgr_sendExtScanDataEvt(p_adv_report->addr_type, p_adv_report->addr, report.device_name, report.device_name_len);
}

/**
 * @brief   Extended scan event handler.
 * @param[in]   event_id   - Event ID.
 * @param[in]   data       - Pointer to the event data.
 * @param[in]   user_data  - User data pointer.
 * @return      None.
 */
static void tlkapp_le_mgr_ext_scan_handler(uint32_t event_id, const void *data, void *user_data)
{
    (void)user_data;
    if (event_id == BLE_HOST_GAP_EVT_LE_SCAN_REPORT_UNDIRECTED) {
        const struct ble_host_gap_evt_scan_adv_report_undirected *p_adv_report = (const struct ble_host_gap_evt_scan_adv_report_undirected *)data;
        if (p_adv_report->connectable == false) {
            // only report connectable adv
            return;
        }
        tlkapp_lemgr_ext_scan_undirected_report_handler(p_adv_report);
    } else if (event_id == BLE_HOST_GAP_EVT_LE_SCAN_REPORT_DIRECTED) {
        struct ble_host_gap_evt_scan_adv_report_directed *p_dir_adv_report = (struct ble_host_gap_evt_scan_adv_report_directed *)data;
        tlk_printf("direct adv received: %s", addr_to_str(p_dir_adv_report->addr));
        tlkapp_lemgr_ext_scan_directed_report_handler(p_dir_adv_report);
    }
}

#endif

/**
 * @brief   Handle start extended scan command.
 * @param[in]   pData    - Pointer to the command data.
 * @param[in]   dataLen  - Length of the command data.
 * @return      None.
 */
static void tlkapp_lemgr_recvExtScanStartCmdDeal(uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
#if (TLKAPP_LEMGR_EXT_SCAN_ENABLE)
    if (s_lemgr_ext_scan_timer == NULL) {
        int res = tlksys_timer_create(TLKSYS_TASKID_LEMGR, &s_lemgr_ext_scan_timer, 5 * 1000 * 1000, false, tlkapp_lemgr_ext_scan_timer_callback, NULL);
        if (res != TLK_ENONE) {
            tlkapp_lemgr_sendCommRsp(TLKSYS_LE_MSGID_START_EXT_SCAN, TLKPRT_COMM_RSP_STATUE_FAILURE, TLK_EFAIL, NULL, 0);
            return;
        }
    }

    if (s_lemgr_ext_scan_count == 0) {
        s_lemgr_ext_scan_count = 24;
        tlksys_timer_start(TLKSYS_TASKID_LEMGR, s_lemgr_ext_scan_timer);
        ble_host_gap_ext_scan_set_param(BLE_HOST_GAP_EXT_SCAN_PARAM_BASIC_UNFILTERED, BLE_HOST_GAP_PASSIVE_SCAN(100, 100));
        if (ble_host_gap_ext_scan_enable() == BLE_HOST_ERR_SUCC) {
            ble_host_gap_evt_add_subscriber(&s_tlkapp_le_mgr_ext_scan);
        } else {
            tlkapp_lemgr_sendCommRsp(TLKSYS_LE_MSGID_START_EXT_SCAN, TLKPRT_COMM_RSP_STATUE_FAILURE, TLK_EFAIL, NULL, 0);
        }
    } else {
        s_lemgr_ext_scan_count = 24;
    }

    tlkapp_lemgr_sendCommRsp(TLKSYS_LE_MSGID_START_EXT_SCAN, TLKPRT_COMM_RSP_STATUE_SUCCESS, TLK_ENONE, NULL, 0);
#else
    (void)pData;
    (void)dataLen;
    tlkapp_lemgr_sendCommRsp(TLKSYS_LE_MSGID_START_EXT_SCAN, TLKPRT_COMM_RSP_STATUE_FAILURE, TLK_ENOSUPPORT, NULL, 0);
#endif
}

/**
 * @brief   Handle stop extended scan command.
 * @param[in]   pData    - Pointer to the command data.
 * @param[in]   dataLen  - Length of the command data.
 * @return      None.
 */
static void tlkapp_lemgr_recvExtScanStopCmdDeal(uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
    int cap_device_table_scan_device_clear(void);
    cap_device_table_scan_device_clear();
#if TLKAPP_LEMGR_EXT_SCAN_ENABLE
    if (s_lemgr_ext_scan_count != 0) {
        tlkapi_printf(TLKAPP_LEMGR_LOG_ENABLE, "exit ext scan");
        tlksys_timer_stop(TLKSYS_TASKID_LEMGR, s_lemgr_ext_scan_timer);
        ble_host_gap_ext_scan_disable();
        s_lemgr_ext_scan_count = 0;
    }
    ble_host_gap_evt_remove_subscriber(&s_tlkapp_le_mgr_ext_scan);
    tlkapp_lemgr_sendCommRsp(TLKSYS_LE_MSGID_STOP_EXT_SCAN, TLKPRT_COMM_RSP_STATUE_SUCCESS, TLK_ENONE, NULL, 0);
#else
    tlkapp_lemgr_sendCommRsp(TLKSYS_LE_MSGID_START_EXT_SCAN, TLKPRT_COMM_RSP_STATUE_FAILURE, TLK_ENOSUPPORT, NULL, 0);
#endif
}

/**
 * @brief   Handle set scan filter parameter command.
 * @param[in]   pData    - Pointer to the command data.
 * @param[in]   dataLen  - Length of the command data.
 * @return      None.
 */
static void tlkapp_lemgr_recvSetScanFilterCmdDeal(uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
    // if (tlkapp_lemgr_setScanFilter(pData, dataLen) == TLK_ENONE) {
    //     tlkapp_lemgr_sendCommRsp(TLKSYS_LE_MSGID_SET_SCAN_FILTER, TLKPRT_COMM_RSP_STATUE_SUCCESS, TLK_ENONE, NULL, 0);
    // } else {
    //     tlkapp_lemgr_sendCommRsp(TLKSYS_LE_MSGID_SET_SCAN_FILTER, TLKPRT_COMM_RSP_STATUE_FAILURE, TLK_EFAIL, NULL, 0);
    // }
}

/**
 * @brief   Extended create connection callback function.
 * @param[in]   result  - Connection result.
 * @return      None.
 */
static void tlkapp_lemgr_ext_create_connection_callback(bool result)
{
    if (result == false) {
        uint8_t status = 0xFF;
        tlkapp_lemgr_sendCommRsp(TLKSYS_LE_MSGID_CONNECT, TLKPRT_COMM_RSP_STATUE_FAILURE, TLK_ENONE, &status, 1);
    } else {
        uint8_t status = 0x00;
        tlkapp_lemgr_sendCommRsp(TLKSYS_LE_MSGID_CONNECT, TLKPRT_COMM_RSP_STATUE_SUCCESS, TLK_ENONE, (uint8_t *)&status, 1);
    }
}

/**
 * @brief   Handle connect command.
 * @param[in]   pData    - Pointer to the command data.
 * @param[in]   dataLen  - Length of the command data.
 * @return      None.
 */
static void tlkapp_lemgr_recvConnectCmdDeal(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    uint8_t *p_addr    = pData + 1;
    uint8_t  addr_type = pData[0];
    int      res       = BLE_HOST_ERR_SUCC;
    if ((res = ble_host_gap_acl_central_ext_create_connection(
             BLE_HOST_GAP_ACL_CENTRAL_EXT_CREATE_CONN_PARAMS_BALANCED(addr_type, p_addr, tlkapp_lemgr_ext_create_connection_callback))) == BLE_HOST_ERR_SUCC) {
    } else {
        tlkapp_lemgr_sendCommRsp(TLKSYS_LE_MSGID_CONNECT, TLKPRT_COMM_RSP_STATUE_FAILURE, TLK_ENONE, (uint8_t *)&res, 1);
    }
}

/**
 * @brief   Handle disconnect command.
 * @param[in]   pData    - Pointer to the command data.
 * @param[in]   dataLen  - Length of the command data.
 * @return      None.
 */
static void tlkapp_lemgr_recvDisconnectCmdDeal(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;

    u16 disconnct_handle = pData[0] | pData[1] << 8;

    int status = ble_host_gap_disconnect_acl(disconnct_handle);

    if (status == BLE_HOST_ERR_SUCC) {
        tlkapp_lemgr_sendCommRsp(TLKSYS_LE_MSGID_DISCONNECT, TLKPRT_COMM_RSP_STATUE_SUCCESS, TLK_ENONE, (uint8_t *)&status, 1);
    } else {
        tlkapi_send_string_data(APP_LOG_EN, "ble_host_gap_disconnect_acl error", &status, 4);
        tlkapp_lemgr_sendCommRsp(TLKSYS_LE_MSGID_DISCONNECT, TLKPRT_COMM_RSP_STATUE_FAILURE, TLK_EFAIL, (uint8_t *)&status, 1);
    }
}

/**
 * @brief   Handle open auto reconnection command.
 * @param[in]   pData    - Pointer to the command data.
 * @param[in]   dataLen  - Length of the command data.
 * @return      None.
 */
static void tlkapp_lemgr_recvOpenRecCmdDeal(uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
    if (tlkapp_lemgr_SetAutoRec(pData, dataLen) == TLK_ENONE) {
        tlkapp_lemgr_sendCommRsp(TLKSYS_LE_MSGID_OPEN_REC, TLKPRT_COMM_RSP_STATUE_SUCCESS, TLK_ENONE, NULL, 0);
    } else {
        tlkapp_lemgr_sendCommRsp(TLKSYS_LE_MSGID_OPEN_REC, TLKPRT_COMM_RSP_STATUE_FAILURE, TLK_EFAIL, NULL, 0);
    }
}

struct tlkapp_lemgr_bound_informs
{
    uint8_t cnt;

    struct
    {
        uint8_t address_type;
        uint8_t address[6];
    } item[6];
};

/**
 * @brief   Handle get bound table command.
 * @param[in]   pData    - Pointer to the command data.
 * @param[in]   dataLen  - Length of the command data.
 * @return      None.
 */
static void tlkapp_lemgr_getBoundTableCmdDeal(uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
    struct tlkapp_lemgr_bound_informs bound_informs = {0};
    bound_informs.cnt                               = ble_host_smp_store_get_current_number_central();
    if (bound_informs.cnt > 6) {
        bound_informs.cnt = 6;
    }
    for (int i = 0; i < bound_informs.cnt; i++) {
        struct ble_host_smp_store_key *p_smp_key = ble_host_smp_store_load_info_by_index_central(i);

        bound_informs.item[i].address_type = p_smp_key->peer_ota_addr_type;
        memcpy(bound_informs.item[i].address, p_smp_key->peer_ota_addr, 6);
    }

    if (bound_informs.cnt) {
        tlkapp_lemgr_sendCommRsp(TLKSYS_LE_MSGID_GET_BOUND_TABLE, TLKPRT_COMM_RSP_STATUE_SUCCESS, TLK_ENONE, (uint8_t *)&bound_informs, bound_informs.cnt * 7 + 1);
    } else {
        uint8_t count = 0;
        tlkapp_lemgr_sendCommRsp(TLKSYS_LE_MSGID_GET_BOUND_TABLE, TLKPRT_COMM_RSP_STATUE_SUCCESS, TLK_ENONE, &count, 1);
    }
}

/**
 * @brief   Handle delete bound table command.
 * @param[in]   pData    - Pointer to the command data.
 * @param[in]   dataLen  - Length of the command data.
 * @return      None.
 */
static void tlkapp_lemgr_delBoundTableCmdDeal(uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
    ble_host_smp_store_delete_all_info_central();
    tlkapp_lemgr_sendCommRsp(TLKSYS_LE_MSGID_DELETE_BOUND_TABLE, TLKPRT_COMM_RSP_STATUE_SUCCESS, TLK_ENONE, NULL, 0);
}

/**
 * @brief   Handle delete bound item command.
 * @param[in]   pData    - Pointer to the command data.
 * @param[in]   dataLen  - Length of the command data.
 * @return      None.
 */
static void tlkapp_lemgr_delBoundItemCmdDeal(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    uint8_t address_type;
    uint8_t address[6];

    address_type = pData[0];
    memcpy(address, pData + 1, 6);
    if (ble_host_smp_store_delete_pairing(address_type, address) == BLE_HOST_ERR_SUCC) {
        tlkapp_lemgr_sendCommRsp(TLKSYS_LE_MSGID_DELETE_BOUND_ITEM, TLKPRT_COMM_RSP_STATUE_SUCCESS, TLK_ENONE, NULL, 0);
    } else {
        tlkapp_lemgr_sendCommRsp(TLKSYS_LE_MSGID_DELETE_BOUND_ITEM, TLKPRT_COMM_RSP_STATUE_FAILURE, TLK_EFAIL, NULL, 0);
    }
}

/**
 * @brief   Handle incoming messages based on message ID.
 * @param[in]   msgID    - Message ID.
 * @param[in]   pData    - Pointer to the message data.
 * @param[in]   dataLen  - Length of the message data.
 * @return      Status of the operation.
 */
int tlkapp_lemgr_msgHandle(uint8_t msgID, uint8_t *pData, uint16_t dataLen)
{
    switch (msgID) {
    case TLKSYS_LE_MSGID_START_EXT_SCAN:
        tlkapp_lemgr_recvExtScanStartCmdDeal(pData, dataLen);
        break;
    case TLKSYS_LE_MSGID_STOP_EXT_SCAN:
        tlkapp_lemgr_recvExtScanStopCmdDeal(pData, dataLen);
        break;
    case TLKSYS_LE_MSGID_SET_SCAN_FILTER:
        tlkapp_lemgr_recvSetScanFilterCmdDeal(pData, dataLen);
        break;
    case TLKSYS_LE_MSGID_CONNECT:
        tlkapp_lemgr_recvConnectCmdDeal(pData, dataLen);
        break;
    case TLKSYS_LE_MSGID_DISCONNECT:
        tlkapp_lemgr_recvDisconnectCmdDeal(pData, dataLen);
        break;
    case TLKSYS_LE_MSGID_OPEN_REC:
        tlkapp_lemgr_recvOpenRecCmdDeal(pData, dataLen);
        break;
    case TLKSYS_LE_MSGID_GET_BOUND_TABLE:
        tlkapp_lemgr_getBoundTableCmdDeal(pData, dataLen);
        break;
    case TLKSYS_LE_MSGID_DELETE_BOUND_TABLE:
        tlkapp_lemgr_delBoundTableCmdDeal(pData, dataLen);
        break;
    case TLKSYS_LE_MSGID_DELETE_BOUND_ITEM:
        tlkapp_lemgr_delBoundItemCmdDeal(pData, dataLen);
        break;
    default:
        return -TLK_ENOSUPPORT;
    }
    return TLK_ENONE;
}

/**
 * @brief   Start scanning for devices.
 * @return  None.
 */
void tlkapp_lemgr_start_scan(void)
{
    if (ble_host_gap_ext_scan_enable() == BLE_HOST_ERR_SUCC) {
        s_lemgr_ext_scan_count = 24;
        tlksys_timer_start(TLKSYS_TASKID_LEMGR, s_lemgr_ext_scan_timer);
        ble_host_gap_evt_add_subscriber(&s_tlkapp_le_mgr_ext_scan);
    }
}
#endif //TLKAPP_LEMGR_ENABLE
