/********************************************************************************************************
 * @file    tlkapp_sysMsg.c
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
#if (TLK_CFG_SYSTEM_ENABLE)
#include "tlkmw/tlkmw.h"
#include "tlkapp/tlkapp.h"
#include "tlklib/usb/tlkusb.h"
#include "stack/bt/host/bth/bth_stdio.h"

#define TLKAPP_SYS_DBG_FLAG 0XFFFFFFFF
#define TLKAPP_SYS_DBG_SIGN "[SYS]"

/**
 * @brief       Sends a common command.
 * @param[in]   cmdID    - The command ID.
 * @param[in]   pData    - Pointer to the data buffer.
 * @param[in]   dataLen  - Length of the data.
 * @return      Returns TLK_ENONE on success, or an error code on failure.
 */
int tlkapp_sys_sendCommCmd(uint8_t cmdID, uint8_t *pData, uint8_t dataLen)
{
#if (TLK_CFG_UART_TOOL_ENABLE)
    return tlkmdi_comm_sendCmd(TLKPRT_COMM_MTYPE_SYS, cmdID, pData, dataLen);
#else
    (void)cmdID;
    (void)pData;
    (void)dataLen;
    return -TLK_ENOSUPPORT;
#endif
}

/**
 * @brief       Sends a common response.
 * @param[in]   cmdID    - The command ID.
 * @param[in]   status   - The status code.
 * @param[in]   reason   - The reason code.
 * @param[in]   pData    - Pointer to the data buffer.
 * @param[in]   dataLen  - Length of the data.
 * @return      Returns TLK_ENONE on success, or an error code on failure.
 */
int tlkapp_sys_sendCommRsp(uint8_t cmdID, uint8_t status, uint8_t reason, uint8_t *pData, uint8_t dataLen)
{
#if (TLK_CFG_UART_TOOL_ENABLE)
    return tlkmdi_comm_sendRsp(TLKPRT_COMM_MTYPE_SYS, cmdID, status, reason, pData, dataLen);
#else
    (void)cmdID;
    (void)status;
    (void)reason;
    (void)pData;
    (void)dataLen;
    return -TLK_ENOSUPPORT;
#endif
}

/**
 * @brief       Sends a common event.
 * @param[in]   evtID    - The event ID.
 * @param[in]   pData    - Pointer to the data buffer.
 * @param[in]   dataLen  - Length of the data.
 * @return      Returns TLK_ENONE on success, or an error code on failure.
 */
int tlkapp_sys_sendCommEvt(uint8_t evtID, uint8_t *pData, uint8_t dataLen)
{
#if (TLK_CFG_UART_TOOL_ENABLE)
    return tlkmdi_comm_sendEvt(TLKPRT_COMM_MTYPE_SYS, evtID, pData, dataLen);
#else
    (void)evtID;
    (void)pData;
    (void)dataLen;
    return -TLK_ENOSUPPORT;
#endif
}

/**
 * @brief       Handles serial message sending.
 * @param[in]   pData    - Pointer to the data buffer.
 * @param[in]   dataLen  - Length of the data.
 * @return      Returns TLK_ENONE on success, or an error code on failure.
 */
static int tlkapp_sys_serialSendMsgDeal(uint8_t *pData, uint16_t dataLen)
{
#if (TLK_CFG_UART_TOOL_ENABLE)
    if (pData == NULL || dataLen < TLKPTI_SYS_SERIAL_HEADLEN) {
        return -TLK_EFORMAT;
    }
    if (pData[0] == TLKPRT_COMM_PTYPE_CMD) {
        return tlkmdi_comm_sendCmd(pData[1], pData[2], pData + TLKPTI_SYS_SERIAL_HEADLEN, dataLen - TLKPTI_SYS_SERIAL_HEADLEN);
    } else if (pData[0] == TLKPRT_COMM_PTYPE_RSP) {
        return tlkmdi_comm_sendRsp(pData[1], pData[2], pData[3], pData[4], pData + TLKPTI_SYS_SERIAL_HEADLEN, dataLen - TLKPTI_SYS_SERIAL_HEADLEN);
    } else if (pData[0] == TLKPRT_COMM_PTYPE_EVT) {
        return tlkmdi_comm_sendEvt(pData[1], pData[2], pData + TLKPTI_SYS_SERIAL_HEADLEN, dataLen - TLKPTI_SYS_SERIAL_HEADLEN);
    } else if (pData[0] == TLKPRT_COMM_PTYPE_DAT) {
        return tlkmdi_comm_sendDat(pData[1], ((uint16_t)pData[3] << 8) | pData[2], pData + TLKPTI_SYS_SERIAL_HEADLEN, dataLen - TLKPTI_SYS_SERIAL_HEADLEN);
    } else {
        return -TLK_ENOSUPPORT;
    }
#else
    (void)pData;
    (void)dataLen;
    return -TLK_ENOSUPPORT;
#endif
}

/**
 * @brief       Handles the 'Get Version' command.
 * @param[in]   none.
 * @return      none.
 */
static void tlkapp_sys_recvGetVersionCmdDeal(void)
{
    uint8_t  buffLen;
    uint8_t  buffer[12];
    uint16_t prtVersion;
    uint32_t libVersion;
    uint32_t appVersion;

    libVersion = TLK_LIB_VERSION;
    prtVersion = TLK_PRT_VERSION;
    appVersion = TLK_APP_VERSION;

    buffLen           = 0;
    buffer[buffLen++] = (libVersion & 0xFF000000) >> 24;
    buffer[buffLen++] = (libVersion & 0x00FF0000) >> 16;
    buffer[buffLen++] = (libVersion & 0x0000FF00) >> 8;
    buffer[buffLen++] = (libVersion & 0x000000FF);
    buffer[buffLen++] = (prtVersion & 0x0000FF00) >> 8;
    buffer[buffLen++] = (prtVersion & 0x000000FF);
    buffer[buffLen++] = (appVersion & 0xFF000000) >> 24;
    buffer[buffLen++] = (appVersion & 0x00FF0000) >> 16;
    buffer[buffLen++] = (appVersion & 0x0000FF00) >> 8;
    buffer[buffLen++] = (appVersion & 0x000000FF);

    tlkapp_sys_sendCommRsp(TLKSYS_SYS_MSGID_VERSION, TLKPRT_COMM_RSP_STATUE_SUCCESS, TLK_ENONE, buffer, buffLen);
}

/**
 * @brief       Handles the 'Format U Disk' command.
 * @param[in]   pData    - Pointer to the data buffer.
 * @param[in]   dataLen  - Length of the data.
 * @return      none.
 */
static void tlkapp_sys_recvFormatUDiskCmdDeal(uint8_t *pData, uint8_t dataLen)
{
    (void)pData;
    (void)dataLen;
    tlkmdi_tinySql_restoreFactorySettings();
    tlkapp_sys_sendCommRsp(TLKSYS_SYS_MSGID_CLEAN_SQL, TLKPRT_COMM_RSP_STATUE_SUCCESS, TLK_ENONE, NULL, 0);
}

//timer to delay 15ms for usb hardware
#if (TLK_CFG_USB_ENABLE)
/**
 * @brief       Timer callback for setting USB mode.
 * @param[in]   timerHandle - Handle of the timer.
 * @param[in]   userArg     - User-defined argument.
 * @return      none.
 */
static void tlkapp_sys_recvSetUSBModeCmdDealTimer(TlkApiTimerHandle_t timerHandle, void *userArg)
{
    (void)timerHandle;
    if (tlkusb_setModule(TLK_CFG_USB_UDB_INDEX, (uint32_t)userArg)) {
        tlkapp_sys_sendCommRsp(TLKSYS_SYS_MSGID_SET_USB_MODE, TLKPRT_COMM_RSP_STATUE_SUCCESS, TLK_ENONE, NULL, 0);
    } else {
        tlkapp_sys_sendCommRsp(TLKSYS_SYS_MSGID_SET_USB_MODE, TLKPRT_COMM_RSP_STATUE_FAILURE, TLK_EFAIL, NULL, 0);
    }
    tlksys_timer_destroy(TLKSYS_TASKID_SYSTEM, timerHandle);
}
#endif

/**
 * @brief       Handles the 'Set USB Mode' command.
 * @param[in]   pData    - Pointer to the data buffer.
 * @param[in]   dataLen  - Length of the data.
 * @return      none.
 */
static void tlkapp_sys_recvSetUSBModeCmdDeal(uint8_t *pData, uint8_t dataLen)
{
    (void)dataLen;
    (void)pData;
#if (TLK_CFG_USB_ENABLE)
    extern uint8_t tlkusb_get_curMode(uint8_t index);
    if (tlkusb_get_curMode(TLK_CFG_USB_UDB_INDEX) == pData[0]) {
        tlkapp_sys_sendCommRsp(TLKSYS_SYS_MSGID_SET_USB_MODE, TLKPRT_COMM_RSP_STATUE_FAILURE, TLK_EBUSY, NULL, 0);
    } else {
        tlkusb_close(TLK_CFG_USB_UDB_INDEX);
        //delay 15ms for usb hardware
        TlkApiTimerHandle_t timerHandle = 0;
        tlksys_timer_create(TLKSYS_TASKID_SYSTEM, &timerHandle, 15 * 1000, false, tlkapp_sys_recvSetUSBModeCmdDealTimer, (void *)(uint32_t)pData[0]);
        tlksys_timer_start(TLKSYS_TASKID_SYSTEM, timerHandle);
    }
#else
    tlkapp_sys_sendCommRsp(TLKSYS_SYS_MSGID_SET_USB_MODE, TLKPRT_COMM_RSP_STATUE_FAILURE, TLK_ENOSUPPORT, NULL, 0);
#endif
}

/**
 * @brief       Handles key action events.
 * @param[in]   pData    - Pointer to the data buffer.
 * @param[in]   dataLen  - Length of the data.
 * @return      none.
 */
static void tlkapp_sys_recvKeyActionDeal(uint8_t *pData, uint16_t dataLen)
{
    if (pData == NULL || dataLen != 2) {
        return;
    }
    tlkapi_array(TLKAPP_SYS_DBG_FLAG, TLKAPP_SYS_DBG_SIGN, "tlkapp_sys_recvKeyActionDeal: ", pData, dataLen);
    uint8_t       keyID               = pData[0];
    uint8_t       evtID               = pData[1];
    keyConfigs_t *sApp_key_cur_config = NULL;
    tlkmdi_tinySql_getKeyCofnig(&sApp_key_cur_config);
    if (sApp_key_cur_config != NULL) {
        if (sApp_key_cur_config->EvtModes[keyID - 1][evtID] != KEY_EVT_MODE_NONE) {
#if (TLK_DEV_KEY_ENABLE)
            tlkapp_sysKey_evtAction(sApp_key_cur_config->EvtModes[keyID - 1][evtID]);
#endif
        }
    }
}

/**
 * @brief       Handles vendor-specific key configuration.
 * @param[in]   offsetId - The offset ID for the vendor configuration.
 * @return      none.
 */
static void tlkapp_sys_recvKeyVendorConfig(uint8_t offsetId)
{
    tlkapi_trace(TLKAPP_SYS_DBG_FLAG, TLKAPP_SYS_DBG_SIGN, "%s: key vendor offset id is %d", __func__, offsetId);
#if TLK_DEV_KEY_ENABLE
    tlkdrv_key_vendorConfigCallback(offsetId);
#endif
}

/**
 * @brief       Handles system messages.
 * @param[in]   msgID    - The message ID.
 * @param[in]   pData    - Pointer to the data buffer.
 * @param[in]   dataLen  - Length of the data.
 * @return      Returns TLK_ENONE on success, or an error code on failure.
 */
int tlkapp_system_msgHandle(uint8_t msgID, uint8_t *pData, uint16_t dataLen)
{
    switch (msgID) {
    case TLKSYS_SYS_MSGID_KEY_ACTION:
        tlkapp_sys_recvKeyActionDeal(pData, dataLen);
        break;
    case TLKSYS_SYS_MSGID_SERIAL_SEND:
        return tlkapp_sys_serialSendMsgDeal(pData, dataLen);
    case TLKSYS_SYS_MSGID_VERSION:
        tlkapp_sys_recvGetVersionCmdDeal();
        break;
    case TLKSYS_SYS_MSGID_CLEAN_SQL:
        tlkapp_sys_recvFormatUDiskCmdDeal(pData, dataLen);
        break;
    case TLKSYS_SYS_MSGID_SET_USB_MODE:
        tlkapp_sys_recvSetUSBModeCmdDeal(pData, dataLen);
        break;
    case TLKSYS_SYS_MSGID_POWEROFF:
        tlksys_pm_forcePowerOff();
        break;
    case TLKSYS_SYS_MSGID_KEY_VENDOR_CONFIG ... TLKSYS_SYS_MSGID_KEY_VENDOR_CONFIG_END - 1:
        tlkapp_sys_recvKeyVendorConfig(msgID - TLKSYS_SYS_MSGID_KEY_VENDOR_CONFIG);
        break;
    default:
        return -TLK_ENOSUPPORT;
    }
    return TLK_ENONE;
}


#endif // #if (TLK_CFG_SYSTEM_ENABLE)
