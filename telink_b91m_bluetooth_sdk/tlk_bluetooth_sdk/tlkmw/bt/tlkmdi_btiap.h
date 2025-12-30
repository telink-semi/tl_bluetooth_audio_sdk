/********************************************************************************************************
 * @file    tlkmdi_btiap.h
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
#ifndef TLKMDI_BTIAP_H
#define TLKMDI_BTIAP_H

typedef struct
{
    uint8_t  detFlag;
    uint16_t aclHandle;
    uint8_t  nameLen;
    uint8_t  name[32];
    uint8_t  bdaddr[6];
} tlkmdi_btiap_t;

/**
 * @brief       This function initializes the IAP module
 * @param[in]   none
 * @return      0 if success, otherwise error code
 */
int tlkmdi_btiap_init(void);

/**
 * @brief       This function sets the BD address for IAP
 * @param[in]   bdaddr    - pointer to the BD address
 * @return      none
 */
void tlkmdi_btiap_setAddr(uint8_t bdaddr[6]);

/**
 * @brief       This function sets the device name for IAP
 * @param[in]   pName     - pointer to the name
 * @param[in]   nameLen   - length of the name
 * @return      none
 */
void tlkmdi_btiap_setName(uint8_t *pName, uint8_t nameLen);

/**
 * @brief       This function sets ACL handle for IAP connection
 * @param[in]   isConn     - connection status
 * @param[in]   aclHandle  - ACL handle
 * @return      none
 */
void tlkmdi_btiap_setAclHandle(bool isConn, uint16_t aclHandle);


/**
 * @brief       This function handles IAP events
 * @param[in]   none
 * @return      none
 */
void tlkmdi_btiap_handler(void);

#endif // TLKMDI_BTIAP_H