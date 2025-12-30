/********************************************************************************************************
 * @file    tlkmdi_bthfp.h
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
#ifndef TLKMDI_BTHFP_H
#define TLKMDI_BTHFP_H

/**
 * @brief       This function initializes the HFP module
 * @param[in]   none.
 * @return      TLK_ENONE is success, others is failure.
 */
int tlkmdi_bthfp_init(void);

/**
 * @brief       This function destroys the HFP module by handle
 * @param[in]   aclHandle   - ACL connection handle
 * @return      none.
 */
void tlkmdi_bthfp_destroy(uint16_t aclHandle);

#endif // TLKMDI_BTHFP_H
