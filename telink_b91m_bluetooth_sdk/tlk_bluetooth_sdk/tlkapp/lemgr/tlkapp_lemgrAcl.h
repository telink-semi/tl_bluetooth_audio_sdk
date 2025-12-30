/********************************************************************************************************
 * @file    tlkapp_lemgrAcl.h
 *
 * @brief   This is the header file for TLSR/TL
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
#ifndef TLKAPP_LEMGR_ACL_H
#define TLKAPP_LEMGR_ACL_H

// struct boundInfom {
//     uint8_t address_type;
//     uint8_t address[6];
// };

// typedef struct {
//     uint8_t            cnt;
//     struct boundInfom item[6];
// } bounfInforms_t;

// struct DisconnectParam {
//     uint16_t Disconnct_handle;
//     uint8_t address_type;
//     uint8_t address[6];
// };

// int tlkapp_lemgr_stopExtScan(void);
// int tlkapp_lemgr_setScanFilter(uint8_t *pData, uint16_t dataLen);

// void get_ScanFilterParam(struct ScanFilterParam *p);

/**
 * @brief   Set auto reconnection flag.
 * @param[in]   pData    - Pointer to the data containing the flag value.
 * @param[in]   dataLen  - Length of the data.
 * @return      Operation status. TLK_ENONE for success.
 */
int tlkapp_lemgr_SetAutoRec(uint8_t *pData, uint16_t dataLen);

/**
 * @brief   Get auto reconnection flag.
 * @return  Current auto reconnection flag value.
 */
unsigned char tlkapp_lemgr_GetAutoRec(void);
// int             tlkapp_lemgr_adv_sameCheck(uint8_t *address, uint8_t type);
// bounfInforms_t *tlkapp_lemgr_getBoundTable(void);
// int             tlkapp_lemgr_delBoundTable(void);
// int             tlkapp_lemgr_delBoundItem(uint8_t *pData, uint16_t dataLen);

/**
 * @brief   Send extended scan data event.
 * @param[in]   address_type  - Address type of the device.
 * @param[in]   address       - Pointer to the device address.
 * @param[in]   name          - Pointer to the device name.
 * @param[in]   name_len      - Length of the device name.
 * @return      None.
 */
void tlkapp_lemgr_sendExtScanDataEvt(uint8_t address_type, const uint8_t *address, uint8_t *name, uint8_t name_len);

/**
 * @brief   Send ACL connection event.
 * @param[in]   handle     - ACL connection handle.
 * @param[in]   status     - Current status.
 * @param[in]   peerRole   - Peer device role.
 * @param[in]   peerType   - Peer device type.
 * @param[in]   pBtAddr    - Pointer to the Bluetooth address.
 * @return      None.
 */
void tlkapp_lemgr_sendAclConnectEvt(uint16_t handle, uint8_t status, uint8_t peerRole, uint8_t peerType, uint8_t *pBtAddr);

/**
 * @brief   Send ACL disconnection event.
 * @param[in]   handle   - ACL connection handle.
 * @param[in]   reason   - Disconnection reason.
 * @param[in]   pBtAddr  - Pointer to the Bluetooth address.
 * @return      None.
 */
void tlkapp_lemgr_sendAclDisconnEvt(uint16_t handle, uint8_t reason, uint8_t *pBtAddr);

#endif //TLKAPP_LEMGR_ACL_H
