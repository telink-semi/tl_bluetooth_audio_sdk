/********************************************************************************************************
 * @file    tlkapp_host_tpt_msg.c
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
#if (TLKSTK_BTTPSLL_TWS_ENABLE)

/**
 * @brief       Handles the sending of key events for Tpsll TWS.
 * @param[in]   pData    - Pointer to the data buffer.
 * @param[in]   dataLen  - Length of the data.
 * @return      none.
 */
static void tlkapp_host_tpt_recvSendKeyDeal(uint8_t *pData,uint16_t dataLen)
{
    (void) dataLen;
    uint8_t key = pData[0];
    tlkmdi_bt_tpt_sendHidkey(0XFFF0,key);
}

/**
 * @brief       Handles messages for the Tpsll TWS module.
 * @param[in]   msgID    - The message ID.
 * @param[in]   pData    - Pointer to the data buffer.
 * @param[in]   dataLen  - Length of the data.
 * @return      Returns TLK_ENONE on success, or an error code on failure.
 */
int tlkapp_host_tpt_msgHandle(uint16_t msgID, uint8_t *pData, uint16_t dataLen)
{
    (void) pData;
    (void) dataLen;
    switch(msgID){

        case TLKSYS_TPT_MSGID_3S_PAIR:
        	tlkmdi_bt_tpt_pair_start_req(TPT_HOST_HEADSET_SETUP_MODE_3S,pData);
            break;
        case TLKSYS_TPT_MSGID_10S_PAIR:
        	tlkmdi_bt_tpt_pair_start_req(TPT_HOST_HEADSET_SETUP_MODE_10S,pData);
            break;
        case TLKSYS_TPT_MSGID_ENTER_LOW_LATENCY_MODE:
        	tlkmdi_bt_tpt_pair_start_req(TPT_HOST_HEADSET_SETUP_MODE_ULTRA_LOW_LATENCY,pData);
            break;           
        case TLKSYS_TPT_MSGID_START_HANDOVER:
            tlkmdi_bt_tpt_handover_start();
            break;
        case TLKSYS_TPT_MSGID_SEND_KEY:
            tlkapp_host_tpt_recvSendKeyDeal(pData,dataLen);
            break;
        case TLKSYS_TPT_MSGID_SHUT_DOWN:
            tlksys_pm_setChn(TLKSYS_PM_CHN_SYS,0,0);
            tlkmdi_bt_tpt_shut_down();
            break;
        default:
            return -TLK_ENOSUPPORT;
    }
    return TLK_ENONE;
}


#endif // #if (TLK_STK_BT_ENABLE)
