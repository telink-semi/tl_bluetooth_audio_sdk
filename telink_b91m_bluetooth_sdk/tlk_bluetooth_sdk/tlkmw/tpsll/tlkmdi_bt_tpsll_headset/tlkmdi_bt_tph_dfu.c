/********************************************************************************************************
 * @file    tlkmdi_bt_tph_dfu.c 
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
#include "tlkmw/tlkmw.h"
#include "tlkmdi_bt_tph_inner.h"

#if (TLK_STK_BT_TPSLL_ENABLE && TLK_MW_OTA_ENABLE)

#define TLKMW_BT_TPH_OTA_SHAKE_INTV 0x07

int tlkmdi_bt_tph_dfu_recv(uint32_t taskID, uint8_t *pData, uint16_t dataLen, void *UserArg)
{
    (void)taskID;
    (void)UserArg;
    tlkmdi_bt_tph_send_dfu_data(pData, dataLen);

    return TLK_ENONE;
}

uint32_t tlkmdi_bt_tph_get_ota_param(uint32_t taskID, uint8_t param_type, void *UserArg)
{
    (void)taskID;
    (void)UserArg;

    if (param_type == TLKMW_OTA_PARAM_MTU_SIZE) {
        return gTlkMdiHeadsetCtrl.acl_mtu;
    } else if (param_type == TLKMW_OTA_PARAM_SHAKE_INTV) {
        return TLKMW_BT_TPH_OTA_SHAKE_INTV;
    } else {
        return 0;
    }
}

int tlkmdi_bt_tph_dfu_repack_version_check(uint8_t *pData, uint16_t dataLen, void *UserArg)
{
    (void)UserArg;
    uint8_t  pBuffer[16] = {0};
    uint16_t buffLen     = 0;

    pBuffer[buffLen++] = 0x01; //OTA
    pBuffer[buffLen++] = tlkmw_ota_common_get_busy_channel() & 0xFF;
    pBuffer[buffLen++] = 0; //length
    pBuffer[buffLen++] = 0; //length
    pBuffer[buffLen++] = TLK_OTA_OPC_VERSION_REQ;
    pBuffer[buffLen++] = 0;
    pBuffer[buffLen++] = 0;

    if (dataLen > 0) {
        tmemcpy(pBuffer + buffLen, pData, dataLen);
        buffLen += dataLen;
    }

    pBuffer[2] = buffLen & 0xFF;
    pBuffer[3] = (buffLen >> 8) & 0xFF;

    tlkmdi_bt_tph_send_dfu_data(pBuffer, buffLen);

    return TLK_ENONE;
}

int tlkmw_ota_version_check(uint8_t *pData, uint16_t dataLen, void *userArg)
{
    (void)pData;
    (void)dataLen;
    (void)userArg;

    //TODO: Distinguish between headset dfu and dongle dfu based on the received version.

    //Headset DFU
    // uint8_t pBuffer[4] = {0};
    // uint16_t buffLen = 0;
    // uint32_t version = 0xFFFFFFFF;//Now do not check version, just return 0xFFFFFFFF.
    // pBuffer[buffLen++] = version 		 & 0xff;
    // pBuffer[buffLen++] = (version >> 8)  & 0xff;
    // pBuffer[buffLen++] = (version >> 16) & 0xff;
    // pBuffer[buffLen++] = (version >> 24) & 0xff;
    // tlk_ota_general_protocol_send_data(TLK_OTA_OPC_VERSION_RSP, pBuffer, buffLen, NULL);

    //Dongle DFU
    uint8_t          channel   = tlkmw_ota_common_get_busy_channel();
    sTlkMwUnitIntf_t interface = {
        .channel       = channel,
        .recv          = tlkmdi_bt_tph_dfu_recv,
        .get_ota_param = tlkmdi_bt_tph_get_ota_param,
    };
    tlkmw_ota_register_chn_interface(&interface);
    tlkmdi_bt_tph_dfu_repack_version_check(pData, dataLen, userArg);

    return TLK_ENONE;
}


#endif
