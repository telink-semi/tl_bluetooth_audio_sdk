/********************************************************************************************************
 * @file    tlkmdi_btspp.h
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
#ifndef TLKMDI_BTSPP_H
#define TLKMDI_BTSPP_H

/**
 * @brief       This function is used to initialize the SPP module
 * @param       none.
 * @return      TLK_ENONE if success, otherwise error code.
 */
int tlkmdi_btspp_init(void);

/**
 * @brief       This function is used to receive SPP data
 * @param[in]   aclHandle - the ACL connection handle
 * @param[in]   rfcHandle - the RFCOMM handle
 * @param[in]   pData - the pointer to the data buffer
 * @param[in]   dataLen - the length of the data
 * @return      none.
 */
void tlkmdi_btspp_recv_data(uint16_t aclHandle, uint8_t rfcHandle, uint8_t *pData, uint16_t dataLen);

#endif // #ifndef TLKMDI_BTSPP_H
