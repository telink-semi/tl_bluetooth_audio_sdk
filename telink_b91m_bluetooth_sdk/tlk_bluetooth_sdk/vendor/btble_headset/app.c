/********************************************************************************************************
 * @file    app.c
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

#include "stack/btble.h"
#include "stack/controller.h"
#include "tlkmw/tlkmw.h"

/**
 * @brief       Initialize the Telink controller
 * @param       none
 * @return      none
 * @note        This function initializes the RF module, TRNG, flash configuration and basic MCU hardware.
 *              It also initializes the BR/EDR core and scheduler when MCU is not in dual core mode.
 */
static void tlk_controller_init(void)
{
#if (!MCU_DUAL_CORE_ENABLE)
    rf_module_init();
    /* random number generator must be initiated here(in the beginning of user initialization).
     * When deepSleep retention wakeUp, no need initialize again */
    trng_init();

    /* initialize some basic MCU hardware */
    tlk_sys_init_mcu_hardware();

    /*initialize BR/EDR core*/
    controller_init(BT_BLE, HCI_TR_SOC, NULL, NULL);
    //////////////////////////// basic hardware Initialization  End /////////////////////////////////

    tlk_sch_init();
#endif
}

/**
 * @brief       Hook function called when system initialization is finished
 * @param       none
 * @return      none
 * @note        This function calls tlk_controller_init to initialize the controller
 */
void tlksys_initFinishedHook(void)
{
    tlk_controller_init();
}

/**
 * @brief       USB debug shell hook function
 * @param[in]   pData - pointer to received data
 * @param[in]   dataLen - length of received data
 * @return      none
 * @note        This function handles debug commands received via USB and performs corresponding actions
 */
__attribute__((noinline)) void tlkusb_debug_shell_hook(uint8_t *pData, uint16_t dataLen)
{
    tlkapi_send_string_data(APP_LOG_EN, "tlkusb_debug_shell_hook: received", pData, dataLen);
#if MCU_CORE_TL752X_TEMP
    /*test code for lynx, remove later.*/
    if (pData[1] == 0x01) {
    } else if (pData[1] == 0x02) {
        void tlkmdi_btSet_scan(uint8_t scan_value, uint16_t time);
        tlkmdi_btSet_scan(3, 120);
    }
#endif

#if (TLK_AUDIO_DSP_BYPASS_TEST_ENABLE) && (CHIP_TYPE == CHIP_TYPE_TL752X)
    if (pData[1] == 0x03) {
        void tlkmw_audio_start_codec_test(uint8_t mode);
        tlkmw_audio_start_codec_test(pData[2]); //02:codec bypass 01:dsp bypass
    }
#endif

    if (pData[1] == 0x05) {
        //extern void bt_audio_print_task_stack_info(void);
        //bt_audio_print_task_stack_info();


        bt_rd_tcf_info_t cur_time;
        extern int       bth_hci_sendReadTcfInfoCmd(uint16_t aclHandle);
        bth_hci_sendReadTcfInfoCmd(0x0008);
        tlksdk_host_get_clkn_fcnt_from_tick(&cur_time, clock_time());

        //    	tlkbt_hci_sendH2cCmd(HCI_RD_SEC_CON_HOST_SUPP_CMD_OPCODE, NULL, 0);
    }

    if (pData[1] == 0x06) {
        //extern void tlkmdi_audio_start_codec_test(uint8_t mode);
        //tlkmdi_audio_start_codec_test(1);
    }
    if (pData[1] == 0x60) {
        if (pData[2] == 0x01) {
            tlkdrv_codec_test_mode_en(1);
            tlkapi_trace(0xffffffff, "****Audio Test Mode****", "Audio Test Stat:Open");
        } else if (pData[2] == 0x00) {
            tlkdrv_codec_test_mode_en(0);
            tlkapi_trace(0xffffffff, "****Audio Test Mode****", "Audio Test Stat:Close");
        }
    }
}
