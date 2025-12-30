/********************************************************************************************************
 * @file    tlkapp_sys.c
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
#if (TLK_CFG_SYSTEM_ENABLE)
#include "tlklib/usb/tlkusb.h"
#include "tlklib/usb/uac_hs/tlkuac_hs.h"
#include "tlkmw/tlkmw.h"
#include "tlkapp/tlkapp.h"

#ifndef TLKAPP_SYSTEM_TASK_STASK_SIZE
#define TLKAPP_SYSTEM_TASK_STASK_SIZE (6 * 1024)
#endif

/**
 * @brief       Initializes the system components.
 * @param[in]   none.
 * @return      none.
 */
static void tlkapp_system_init(void)
{
    tlkmdi_pm_init();
    tlksys_pm_regChn(TLKSYS_PM_CHN_SYS);
    tlksys_pm_setChn(TLKSYS_PM_CHN_SYS, 1, 0);

#if (TLK_DEV_RTC_ENABLE)
    tlkdrv_rtc_init();
#endif

#if (TLK_DEBUG_ENABLE)
    tlkmdi_debug_init();
    tlksys_task_regEvtCB(TLKSYS_TASKID_SYSTEM, TLKSYS_TASK_EVT_SYS_PRINT_LOG, tlkmdi_debug_handler);
#endif

    trng_init();
    tlk_readFlashSize_autoConfigCustomFlashSector();

    tlkapp_sys_pm_init();
    tlksys_task_regEvtCB(TLKSYS_TASKID_SYSTEM, TLKSYS_TASK_EVT_SYS_PM, tlkapp_sys_pm_handler);

    tlkapp_sys_ctrlInit();
    tlksys_task_regEvtCB(TLKSYS_TASKID_SYSTEM, TLKSYS_TASK_EVT_SYS_SQL_SAVE, tlkapp_sys_flashHandler);

#if (TLK_DEV_KEY_ENABLE)
    tlkapp_sysKey_init();
#endif
#if (TLK_DEV_LED_ENABLE)
    tlkapp_sysLed_init();
#endif
    tlkapp_sysUI_updateHandleState(TLKAPP_UI_HANDLE_GROUP_SYS, 0, TLKAPP_UI_STATE_POWER_ON);

#if (TLK_MW_USER_CTRL_ENABLE)
    tlkmw_user_ctrl_init();
    tlksys_task_regEvtCB(TLKSYS_TASKID_SYSTEM, TLKSYS_TASK_EVT_SYS_USER_CTRL, tlkmw_user_ctrl_common_handler);
#endif

#if (TLK_CFG_UART_TOOL_ENABLE)
    tlkmdi_comm_init();
    tlksys_task_regEvtCB(TLKSYS_TASKID_SYSTEM, TLKSYS_TASK_EVT_SYS_SERIAL, tlkmdi_comm_handler);
#endif

#if (TLK_CFG_USB_ENABLE)
    tlksys_task_regEvtCB(TLKSYS_TASKID_SYSTEM, TLKSYS_TASK_EVT_SYS_USB, tlkusb_handler);

#if PROJ_BTTPSLL_TWS || PROJ_BLETPSLL_TWS || PROJ_HEARING_AID || PROJ_INTERNAL_TEST
    uint8_t usbFlag = 0;
    flash_read_page((0xD6000 + flash_full_size - 0x100000), 1, (uint8_t *)&usbFlag);
    uint16_t usbID = ((0xff == usbFlag) ? 0x120 : (0x0100 + usbFlag));
    tlkmdi_tinySql_setUsbID(usbID);
#elif PROJ_MESH_AUDIO_DONGLE || PROJ_TPSLL_AUDIO_DONGLE
    uint16_t usbID = 0x123;
#else
    uint16_t usbID = 0x120;
    (void)usbID;
#endif //ZIYU TODO: code need clean
#if PROJ_TPSLL_AUDIO_DONGLE
    tlkusb_init(TLK_CFG_USB_UDB_INDEX, usbID);
    tlkusb_open(TLK_CFG_USB_UDB_INDEX, 1);
#else
#if (TLK_USB_UDB_ENABLE)
    tlkusb_init(TLK_CFG_USB_UDB_INDEX, usbID);
    tlkusb_open(TLK_CFG_USB_UDB_INDEX, 0);
#endif

#if (TLK_USB_UAC_ENABLE)
    tlkusb_init(TLK_CFG_USB_UAC_INDEX, 0x121);
    tlkusb_open(TLK_CFG_USB_UAC_INDEX, 1);
#endif
#endif
    tlkusb_hal_enable_eventMode();

#if (TLK_USB_UAC_HS_ENABLE)
    /*TODO: clean later.*/
    tlksys_task_regEvtCB(TLKSYS_TASKID_SYSTEM, TLKSYS_TASK_EVT_SYS_USB_HS, tlkusb_uachs_handler);
    extern void tlkusb_msg_init(void);
    tlkusb_msg_init();
    tlkhal_usb_function_init(TLK_CFG_USB_UAC_INDEX, TLKUSB_MODTYPE_UAC_HS, NULL);
#endif

#endif

#if (TLK_CFG_FS_ENABLE)
    tlkmw_fs_init();
#endif

    tlkapp_sys_taskInitCompletedHook();

    tlkapp_sys_sendCommEvt(TLKPRT_COMM_EVTID_SYS_READY, NULL, 0);
    tlkapp_sysUI_updateHandleState(TLKAPP_UI_HANDLE_GROUP_SYS, 0, TLKAPP_UI_STATE_IDLE);
}

/**
 * @brief       Handles input messages for the system task.
 * @param[in]   msgID    - The ID of the message.
 * @param[in]   pData    - Pointer to the data buffer.
 * @param[in]   dataLen  - Length of the data.
 * @return      Returns TLK_ENONE on success, or an error code on failure.
 */
static int tlkapp_system_input(uint16_t msgID, uint8_t *pData, uint16_t dataLen)
{
    if (msgID == TLKSYS_SYS_MSGID_RUN_FUNC) {
        const uint8_t headLen = sizeof(tlksys_requestRunFunc_t);
        if (dataLen == headLen) {
            tlksys_requestRunFunc_t func = *(tlksys_requestRunFunc_t *)pData;
            func();
        } else if (dataLen >= headLen) {
            tlksys_requestRunFuncWithArg_t func = *(tlksys_requestRunFuncWithArg_t *)pData;
            func(pData + headLen, dataLen - headLen);
        } else {
            return -TLK_EPARAM;
        }
        return TLK_ENONE;
    }
    return tlkapp_system_msgHandle(msgID, pData, dataLen);
}

/**
 * @brief       Gets the configuration for the system task.
 * @param[in]   none.
 * @return      Returns a pointer to the system task configuration.
 */
const tlksys_task_cfg_t *tlkapp_system_getTaskCfg(void)
{
    _attribute_os_heap_sec_ static uint8_t sTlkAppSystemTaskBuffer[TLKAPP_SYSTEM_TASK_STASK_SIZE];
    (void)sTlkAppSystemTaskBuffer;
    static const tlksys_task_cfg_t sTlkAppSystemTask = {
        .Init      = tlkapp_system_init,
        .Input     = tlkapp_system_input,
        .priority  = TLKSYS_TASK_SYSTEM_PRIORITY,
        .stackSize = TLKAPP_SYSTEM_TASK_STASK_SIZE,
        .pTaskName = "SYSTEM",
#if TLK_CFG_RTOS_ENABLE
        .pTaskStaticBuffer = &sTlkAppSystemTaskBuffer,
#endif
    };
    return &sTlkAppSystemTask;
}

/**
 * @brief       Provides a hook function when the system task init is completed.
 * @param[in]   none.
 * @return      none.
 */
__attribute__((weak)) void tlkapp_sys_taskInitCompletedHook(void) {}

#endif // #if (TLK_CFG_SYSTEM_ENABLE)
