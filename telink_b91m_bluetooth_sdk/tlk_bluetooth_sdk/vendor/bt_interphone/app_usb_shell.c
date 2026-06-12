/********************************************************************************************************
 * @file    app_usb_shell.c
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
#include "stack/bt/host/bt_hci.h"
#include "stack/bt/host/tlkstk_stdio.h"
#include "tlkmw/tlkmw.h"
#include "tlkapp/tlkapp.h"

#if AUDIO_HD_HAC_EN
#include "tlkmw/sys_dev/tlkmw_sysdev.h"
#endif
void tlksys_initFinishedHook(void)
{
#if TLKHW_TYPE == TLKHW_TL751X_EVK_C1T368A110_V1_0
    gpio_function_en(GPIO_PA6);
    gpio_output_en(GPIO_PA6);
    gpio_input_dis(GPIO_PA6);
    gpio_set_high_level(GPIO_PA6);

    gpio_function_en(GPIO_PG2 | GPIO_PG3);
    gpio_output_dis(GPIO_PG2 | GPIO_PG3);
    gpio_input_en(GPIO_PG2 | GPIO_PG3);
#endif
}

__attribute__((noinline)) void tlkusb_debug_shell_hook(uint8_t *pData, uint16_t dataLen)
{
    tlkapi_send_string_data(APP_LOG_EN, "tlkusb_debug_shell_hook: received", pData, dataLen);
#if TLK_CFG_PRODUCT_TEST_ENABLE
    void app_usb_shell_pt_demo(uint8_t * pdata, uint16_t len);
    app_usb_shell_pt_demo(pData, dataLen);
#endif
    if (pData[0] != 0x11) {
        return;
    }
    if (pData[1] == 0x01) //BT
    {
        switch (pData[2]) {
        case 0x05:
#if TLK_MW_DSP_COMM_ENABLE
            extern uint32_t tlk_GetDspVersion(void);
            tlk_printf("DSP VERSION = %04x", tlk_GetDspVersion());
#endif
            break;
        case 0x07:
            if (pData[3]) {
                //extern void mic_loopback_test_start(void);
                //mic_loopback_test_start();
                tlkapi_printf(APP_LOG_EN, "mic_loopback_test : ON");
            } else {
                //extern void mic_loopback_test_end(void);
                //mic_loopback_test_end();
                tlkapi_printf(APP_LOG_EN, "mic_loopback_test : OFF");
            }
            break;
        case 0x06:
            extern int      tlkmdi_bthfag_disconnSco(uint8_t * pBtAddr);
            extern uint16_t btp_hfp_getAgHandle(void);
            uint16_t        handleAgSco = btp_hfp_getAgHandle();
            extern uint8_t *bth_handle_getBtAddr(uint16_t aclHandle);
            tlkmdi_bthfag_disconnSco(bth_handle_getBtAddr(handleAgSco));
            break;
        case 0x08:
            tlkmdi_btSet_scan(pData[3], pData[4] | pData[5] << 8);
            break;
        case 0x09:

            extern uint16_t btp_hfp_getAgHandle(void);
            uint16_t        hadleAg = btp_hfp_getAgHandle();
            extern uint8_t *bth_handle_getBtAddr(uint16_t aclHandle);
            extern int      tlkmdi_bthfpag_createSco(uint8_t * pBtAddr);
            tlkapi_printf(1, "tlkmdi_bthfpag_createSco ");
            // int rett = tlkmdi_bthfpag_createSco(bth_handle_getBtAddr(aclhandle));
            int rett = tlkmdi_bthfpag_createSco(bth_handle_getBtAddr(hadleAg));
            tlkapi_printf(1, "tlkmdi_bthfpag_createSco :%d", rett);

            break;
        case 0x10:
            tlkapi_trace(0xffffffff, "<TEST>", "bth_hci_sendLocalVerInfoCmd SHELL");
            extern int bth_hci_sendLocalVerInfoCmd(uint8_t * pData, uint8_t dataLen);
            //         buffer[buffLen++] = pData[0];   ///HCI version number
            // buffer[buffLen++] = pData[1]|(pData[2]<<8);///HCI revision number
            // buffer[buffLen++] = pData[3];///LMP version
            // buffer[buffLen++] = pData[6]|(pData[7]<<8);///HCI revision number
            uint8_t localData[6] = {0x01, 0x02, 0x00, 0x03, 0x05, 0x00};
            bth_hci_sendLocalVerInfoCmd(localData, sizeof(localData));
            break;
            break;
        case 0x12:
            extern int bth_hci_sendReadLocalVerInfoCmd();
            bth_hci_sendReadLocalVerInfoCmd();
            break;
        case 0x13:
            uint8_t    pbuffer[6] = {0};
            uint8_t    bufferLen  = 0;
            extern int bth_hci_sendLocalVerInfoCmd(uint8_t * pData, uint8_t dataLen);
            extern int bth_hci_sendReadLocalVerInfoCmd();
            tlkapi_trace(0xffffffff, "<TEST>", "bth_hci_sendLocalVerInfoCmd SHELL1");
            bth_hci_sendReadLocalVerInfoCmd();
            tlkapi_trace(0xffffffff, "<TEST>", "bth_hci_sendLocalVerInfoCmd SHELL2");

            extern bt_local_version_t sTlkBtlocalVersionParam;
            sTlkBtlocalVersionParam.lmp_subver = 0x0f;
            pbuffer[bufferLen++]               = sTlkBtlocalVersionParam.hci_ver;
            pbuffer[bufferLen++]               = sTlkBtlocalVersionParam.hci_rev;
            pbuffer[bufferLen++]               = sTlkBtlocalVersionParam.hci_rev >> 8;
            pbuffer[bufferLen++]               = sTlkBtlocalVersionParam.lmp_ver;
            pbuffer[bufferLen++]               = sTlkBtlocalVersionParam.lmp_subver;
            pbuffer[bufferLen++]               = sTlkBtlocalVersionParam.lmp_subver >> 8;
            tlkapi_array(0xffffffff, "<TEST>", "bth_hci_sendLocalVerInfoCmd SHELL2", pbuffer, 6);
            bth_hci_sendLocalVerInfoCmd(pbuffer, 6);

            break;

        case 0x11:
        {
            if (dataLen > 4) {
                extern int btp_hfphf_redial(uint16_t aclHandle);
                uint16_t   aclHandle = pData[2] | (pData[3] << 8);
                btp_hfphf_redial(aclHandle);
                tlkapi_trace(0xffffffff, "<test>", "headset redial [%d]", aclHandle);
            }
        } break;


        default:
            break;
        }
    }

    if (pData[1] == 0x10) ///Audio
    {
        switch (pData[2]) {
#if AUDIO_HD_HAC_EN
        case 0x02:
        {
            int ppm_tmp = 0;
            if (pData[4] & 0x80) {
                ppm_tmp = 0xFFFF0000;
                ppm_tmp |= (pData[4] << 8);
                ppm_tmp |= pData[3];
            } else {
                ppm_tmp = (pData[4] << 8);
                ppm_tmp |= pData[3];
            }
            tlkdrv_hac_set_ppm(g_hd_hac_asrc_buffer, ppm_tmp);
            tlkapi_send_string_data(APP_LOG_EN, "tlkdrv_hac_set_ppm", &ppm_tmp, 2);
        } break;
        case 0x03:
        {
            int ppm_tmp = tlkdrv_hac_get_ppm(g_hd_hac_asrc_buffer);
            tlkapi_send_string_data(APP_LOG_EN, "tlkdrv_hac_get_ppm", &ppm_tmp, 2);
        } break;
        case 0x04:
            if (audio_hd_anc_enable) {
                audio_hd_anc_enable = 0;
            } else {
                audio_hd_anc_enable = 1;
            }
            tlkapi_send_string_data(APP_LOG_EN, "audio MCLK switch:184.32M = %d ", &audio_hd_anc_enable, 4);
            break;
#endif
        //sidetone
        case 0x20:
            tlkapi_trace(0xffffffff, "<test>", "open sidetone");
            tlkmdi_audio_sendStartEvt(TLKAUD_TYPE_SIDETONE, 0xffff);
            break;

        case 0x21:
            tlkapi_trace(0xffffffff, "<test>", "close sidetone");
            tlkmdi_audio_sendCloseEvt(TLKAUD_TYPE_SIDETONE, 0xffff);
            break;
        case 0x22:
            tlkapi_trace(0xffffffff, "<test>", "sidetone TLKDRV_CODEC_CHANNEL_LEFT");
            tlkmdi_sidetone_set_mic_chanl(TLKDRV_CODEC_CHANNEL_LEFT);
            break;

        case 0x23:
            tlkapi_trace(0xffffffff, "<test>", "sidetone TLKDRV_CODEC_CHANNEL_RIGHT");
            tlkmdi_sidetone_set_mic_chanl(TLKDRV_CODEC_CHANNEL_RIGHT);
            break;
        case 0x24:
            tlkapi_printf(APP_LOG_EN, "start interphone by mesh");
            tlkmdi_interphone_api_mesh_enable();
            break;
        case 0x25:
            tlkapi_printf(APP_LOG_EN, "close interphone by mesh");
            tlkmdi_interphone_api_mesh_disable();
            break;
        case 0x26:
            tlkapp_audioScheduler_debug();
            break;
        case 0x27:
            tlkapi_printf(APP_LOG_EN, "bt_voice_set_interphone_master");
            bt_voice_set_interphone_master();
            break;
        case 0x28:
            tlkapi_printf(APP_LOG_EN, "bt_voice_clear_interphone_master");
            bt_voice_clear_interphone_master();
            break;
        }
    }
}
