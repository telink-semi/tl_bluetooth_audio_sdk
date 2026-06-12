/********************************************************************************************************
 * @file    tlkapp_lemgrAcl.c
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
#if (TLKAPP_LEMGR_ENABLE)
#include "tlkapp/tlkapp.h"
#include "stack/ble/ble.h"
#define MAX_ADV_PARAM_SAVE (32)


unsigned char le_auto_connect_flag = 1;

/**
 * @brief   Send ACL connection event to application.
 * @param[in]   handle     - ACL connection handle.
 * @param[in]   status     - Current status.
 * @param[in]   peerRole   - Peer device role.
 * @param[in]   peerType   - Peer device type.
 * @param[in]   pBtAddr    - Pointer to the Bluetooth address.
 * @return      None.
 */
void tlkapp_lemgr_sendAclConnectEvt(uint16_t handle, uint8_t status, uint8_t peerRole, uint8_t peerType, uint8_t *pBtAddr)
{
    uint8_t buffLen;
    uint8_t buffer[32];
    buffLen           = 0;
    buffer[buffLen++] = status;            //Status
    buffer[buffLen++] = peerRole + 1;      //Role
    buffer[buffLen++] = (handle & 0x00FF); //Handle
    buffer[buffLen++] = (handle & 0xFF00) >> 8;
    buffer[buffLen++] = peerType;          //
    tmemcpy(buffer + buffLen, pBtAddr, 6); //MAC
    buffLen += 6;
    tlkapp_lemgr_sendCommEvt(TLKPRT_COMM_EVTID_LE_CONNECT, buffer, buffLen);
}

/**
 * @brief   Send ACL disconnection event to application.
 * @param[in]   handle   - ACL connection handle.
 * @param[in]   reason   - Disconnection reason.
 * @param[in]   pBtAddr  - Pointer to the Bluetooth address.
 * @return      None.
 */
void tlkapp_lemgr_sendAclDisconnEvt(uint16_t handle, uint8_t reason, uint8_t *pBtAddr)
{
    (void)pBtAddr;
    uint8_t buffLen;
    uint8_t buffer[32];
    buffLen = 0;

    buffer[buffLen++] = (handle & 0x00FF); //Handle
    buffer[buffLen++] = (handle & 0xFF00) >> 8;
    buffer[buffLen++] = reason; //reason
    // tmemcpy(buffer+buffLen, pBtAddr, 6); //MAC
    // buffLen += 6;
    tlkapp_lemgr_sendCommEvt(TLKPRT_COMM_EVTID_LE_DISCONN, buffer, buffLen);
}

/**
 * @brief   Send extended scan data event.
 * @param[in]   address_type  - Address type of the device.
 * @param[in]   address       - Pointer to the device address.
 * @param[in]   name          - Pointer to the device name.
 * @param[in]   name_len      - Length of the device name.
 * @return      None.
 */
void tlkapp_lemgr_sendExtScanDataEvt(uint8_t address_type, const uint8_t *address, uint8_t *name, uint8_t name_len)
{
    uint8_t buffLen;
    uint8_t buffer[32];
    buffLen           = 0;
    buffer[buffLen++] = address_type;      //address_type
    tmemcpy(buffer + buffLen, address, 6); //MAC address
    buffLen += 6;
    tmemcpy(buffer + buffLen, name, name_len);
    buffLen += name_len;

    buffer[buffLen++] = '\0';

    //    static unsigned int debug_tick=0;
    //    if(clock_time_exceed(debug_tick,10*1000))
    //    {
    //        debug_tick = stimer_get_tick();
    //        // tlkapi_send_string_data(APP_LOG_EN, "sendExtScanDataEvt", buffer,buffLen);
    //        tlkapp_lemgr_sendCommEvt(TLKPRT_COMM_EVTID_LE_EXT_SCAN_Data, buffer, buffLen);
    //    }
    tlkapp_lemgr_sendCommEvt(TLKPRT_COMM_EVTID_LE_EXT_SCAN_Data, buffer, buffLen);
}

/**
 * @brief   Set auto reconnection flag.
 * @param[in]   pData    - Pointer to the data containing the flag value.
 * @param[in]   dataLen  - Length of the data.
 * @return      Operation status. TLK_ENONE for success.
 */
int tlkapp_lemgr_SetAutoRec(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    le_auto_connect_flag = pData[0];
    return TLK_ENONE;
}

/**
 * @brief   Get auto reconnection flag.
 * @return  Current auto reconnection flag value.
 */
unsigned char tlkapp_lemgr_GetAutoRec(void)
{
    return le_auto_connect_flag;
}

#endif //TLKAPP_LEMGR_ENABLE
