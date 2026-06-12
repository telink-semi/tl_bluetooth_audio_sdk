/********************************************************************************************************
 * @file    main.c
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
#include "tlkmw/tlkmw.h"
#include "tlkapp/tlkapp.h"
#include "stack/controller.h"

/**
 * @brief       Overwrite this function to set platform(mcu) config.
 * @param[in]	none
 * @return      none
 */
const tlksys_hal_platform_init_cfg_t *tlksys_hal_port_getPlatformInitCfg(void)
{
    static const tlksys_hal_platform_init_cfg_t cfg = {
        .clockLevel = 0,
        .gpioCfg    = TLKSYS_HAL_INIT_GPIO_CFG_SHUTDOWN,
#if (MCU_CORE_TYPE == MCU_CORE_TL721X && TLK_MW_OTA_ENABLE)
        .lpTmrCfg = TLKSYS_HAL_INIT_LP_TMR_CFG_32kXTAL,
#endif
    };
    return &cfg;
}

void tlksys_initFinishedHook(void)
{
#if (MCU_CORE_TYPE != MCU_CORE_TL751X) /*bt controller initial*/
    rf_module_init();
    trng_init();
    tlk_sys_init_mcu_hardware();
    controller_init(BLE_only, HCI_TR_SOC, NULL, NULL);

    tlk_sch_init();
#endif
}

/**
 * @brief       This is main function
 * @param[in]	none
 * @return      none
 */
int main(void)
{
    tlksys_init();
    tlksys_start(tlkapp_create_allTasks);
    //*note: if use rtos,code will end in tlksys_start*/
    while (1) {
#if !TLK_CFG_RTOS_ENABLE
        tlksys_handler();
#if (MCU_CORE_TYPE != MCU_CORE_TL751X)
        tlk_sys_main_loop(); //controller loop not in rtos
#endif
#endif
    }
    return 0;
}

__attribute__((noinline)) void tlkusb_debug_shell_hook(uint8_t *pData, uint16_t dataLen)
{
    tlkapi_send_string_data(APP_LOG_EN, "tlkusb_debug_shell_hook: received", pData, dataLen);
    if (pData[0] != 0x11) {
        return;
    }
    switch (pData[1]) {
#if 0
        case 0x00:{
            void app_rc_debug_api_fake_file(uint32_t size);
            if(dataLen != 6){
                tlk_printf("fake file fail");
                return;
            }
            uint32_t size = pData[2] + pData[3] * 256 + pData[4] * 65536 + pData[5] * 65536 * 256;
            app_rc_debug_api_fake_file(size);
            tlk_printf("fake file ok size[%d]",size);
        }break;
#endif
#if (TLK_RC_CFG_BT_CENTRAL_STREAM)
    case 0x01:
    {
        tlkmdi_btSet_scan(pData[2], pData[3] | (pData[4] << 8));
    } break;
    case 0x02:
    {
#if (TLK_MW_BTINQ_ENABLE)
        tlkapp_user_start_inquiry(pData[2]); //TLKMDI_BTINQ_DTYPE_ENUM
#endif
    } break;
    case 0x03:
    {
#if (TLK_MW_BTINQ_ENABLE)
        tlkapp_user_stop_inquiry();
#endif
    } break;
    case 0x04:
    {
#if (TLK_MW_BTINQ_ENABLE)
        tlkmdi_btinq_printList();
#endif
    } break;
    case 0x05:
    {
#if (TLK_MW_BTINQ_ENABLE)
        tlkapp_user_connect_inquiry_device_by_index(pData[2]);
#endif
    } break;
    case 0x06:
    {
        if (dataLen == 8) {
            uint8_t *pBtAddr = pData + 2;
            tlkmdi_btacl_connect(pBtAddr, 0x244404, 15000);
        }
    } break;
    case 0x07:
    {
        tlkmdi_audio_sendStartEvt(TLKAUD_TYPE_A2DP_OUT, 0x08);
    } break;
    case 0x08:
    {
        tlkmdi_audio_sendCloseEvt(TLKAUD_TYPE_A2DP_OUT, 0x08);
    } break;
    case 0x09:
    {
        extern uint8_t *bth_handle_getBtAddr(uint16_t aclHandle);
        uint8_t         number[5] = {0, 0x01, 0x02, 0x03, 0x04};
        int             ret       = tlkmdi_bthfpag_insertCall(number, 5, false);
        if (ret == TLK_ENONE) {
            ret = tlkmdi_bthfpag_createSco(bth_handle_getBtAddr(0x08));
        } else {
            tlk_printf("tlkmdi_bthfpag_insertCall fail");
        }
    } break;
    case 0x0A:
    {
        tlkmdi_bthfpag_hungupCall();
    } break;
#endif
#if (TLKALG_BBF_PATH_SEL && TLKALG_BBF_ENABLE)
    case 0x40:
        extern uint8_t bbf_func_en;
        if (pData[2] == 0x01) {
            bbf_func_en = 1;
        } else {
            bbf_func_en = 0;
        }
        tlk_printf("bbf_pypass_en = %d", bbf_func_en);
        break;
#endif
    case 0x50:
#if TLKALG_BONE_CODUCTION_EN
        extern int8_t alg_bone_data_switch;
        if (pData[2] > 0x02) {
            alg_bone_data_switch = 0;
        } else {
            alg_bone_data_switch = pData[2];
        }
#endif
        break;
    default:
        break;
    }
}
