/********************************************************************************************************
 * @file    tlkmdi_btspp.c
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
#if (TLKBTP_CFG_SPP_ENABLE)
#include "tlkmw/bt/tlkmw_bt.h"

#include "stack/bt/host/btp/btp_stdio.h"
#include "tlkmw/tlkmw.h"

/**
 * @brief       This function is used to receive SPP data
 * @param[in]   aclHandle - the ACL connection handle
 * @param[in]   rfcHandle - the RFCOMM handle
 * @param[in]   pData - the pointer to the data buffer
 * @param[in]   dataLen - the length of the data
 * @return      none.
 */
__attribute__((weak)) void tlkmdi_btspp_recv_data(uint16_t aclHandle, uint8_t rfcHandle, uint8_t *pData, uint16_t dataLen)
{
    tlkmw_userctrl_pushDataToTask(aclHandle | (rfcHandle << 16), 0, pData, dataLen);
}

/**
 * @brief       This function is used to send OTA data over SPP
 * @param[in]   taskID - the task ID
 * @param[in]   pData - the pointer to the data to send
 * @param[in]   dataLen - the length of the data
 * @param[in]   UserArg - user argument
 * @return      the result of btp_spp_sendData().
 */
int tlkmdi_btspp_otaSendData(uint32_t taskID, uint8_t *pData, uint16_t dataLen, void *UserArg)
{
    (void)UserArg;
    return btp_spp_sendData(taskID & 0xFFFF, NULL, 0, pData, dataLen);
}

uint32_t tlkmdi_btspp_get_ota_param(uint32_t taskID, uint8_t param_type, void *UserArg)
{
    (void)taskID;
    (void)UserArg;

    if (param_type == TLKMW_OTA_PARAM_MTU_SIZE) {
        btp_spp_item_t *pItem = btp_spp_getConnItem(taskID & 0xFFFF);
        if (pItem != NULL) {
            return pItem->mtuSize;
        }
    }

    return 0;
}

/**
 * @brief       This function is used to initialize the SPP module
 * @param       none.
 * @return      TLK_ENONE if success, otherwise error code.
 */
int tlkmdi_btspp_init(void)
{
    sTlkMwUnitIntf_t interface = {
        .channel       = TLKMW_OTA_TRANS_CHN_BT_SPP,
        .send          = tlkmdi_btspp_otaSendData,
        .recv          = NULL,
        .get_ota_param = tlkmdi_btspp_get_ota_param,
    };
    tlkmw_ota_register_chn_interface(&interface);
    btp_spp_regDataCB(tlkmdi_btspp_recv_data);

    tlkmdi_bt_sppTestInit(); //TODO: need clean.

    return TLK_ENONE;
}


#endif // #if (TLKBTP_CFG_SPP_ENABLE)