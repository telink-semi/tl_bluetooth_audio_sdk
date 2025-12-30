/********************************************************************************************************
 * @file    tlkusb_desc.h
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
#ifndef TLKUSB_DESC_H
#define TLKUSB_DESC_H

#include "common/types.h"

/**
 * @brief       This function gets the language descriptor length based on the mode.
 * @param[in]   mode    - the USB mode.
 * @return      length of the language descriptor.
 */
uint16_t tlkusb_getLanguageLens(uint8_t mode);

/**
 * @brief       This function gets the product descriptor length based on the mode.
 * @param[in]   mode    - the USB mode.
 * @return      length of the product descriptor.
 */
uint16_t tlkusb_getProductLens(uint8_t mode);

/**
 * @brief       This function gets the vendor descriptor length based on the mode.
 * @param[in]   mode    - the USB mode.
 * @return      length of the vendor descriptor.
 */
uint16_t tlkusb_getVendorLens(uint8_t mode);

/**
 * @brief       This function gets the serial descriptor length based on the mode.
 * @param[in]   mode    - the USB mode.
 * @return      length of the serial descriptor.
 */
uint16_t tlkusb_getSerialLens(uint8_t mode);

/**
 * @brief       This function gets the device descriptor length based on the mode.
 * @param[in]   mode    - the USB mode.
 * @return      length of the device descriptor.
 */
uint16_t tlkusb_getDeviceLens(uint8_t mode);

/**
 * @brief       This function gets the configuration descriptor length based on the mode.
 * @param[in]   mode    - the USB mode.
 * @return      length of the configuration descriptor.
 */
uint16_t tlkusb_getConfigLens(uint8_t mode);

/**
 * @brief       This function gets the language descriptor based on the mode.
 * @param[in]   mode    - the USB mode.
 * @return      pointer to the language descriptor.
 */
uint8_t *tlkusb_getLanguageDesc(uint8_t mode);

/**
 * @brief       This function gets the vendor descriptor based on the mode.
 * @param[in]   mode    - the USB mode.
 * @return      pointer to the vendor descriptor.
 */
uint8_t *tlkusb_getVendorDesc(uint8_t mode);

/**
 * @brief       This function gets the product descriptor based on the mode.
 * @param[in]   mode    - the USB mode.
 * @return      pointer to the product descriptor.
 */
uint8_t *tlkusb_getProductDesc(uint8_t mode);

/**
 * @brief       This function gets the serial descriptor based on the mode.
 * @param[in]   mode    - the USB mode.
 * @return      pointer to the serial descriptor.
 */
uint8_t *tlkusb_getSerialDesc(uint8_t mode);

/**
 * @brief       This function gets the device descriptor based on the mode.
 * @param[in]   mode    - the USB mode.
 * @return      pointer to the device descriptor.
 */
uint8_t *tlkusb_getDeviceDesc(uint8_t mode);

/**
 * @brief       This function gets the configuration descriptor based on the mode.
 * @param[in]   mode    - the USB mode.
 * @return      pointer to the configuration descriptor.
 */
uint8_t *tlkusb_getConfigDesc(uint8_t mode);


#endif // TLKUSB_DESC_H
