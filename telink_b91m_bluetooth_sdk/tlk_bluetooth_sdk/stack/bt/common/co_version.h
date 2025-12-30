/********************************************************************************************************
 * @file    co_version.h
 *
 * @brief   This is the header file for TLSR/TL
 *
 * @author  Bluetooth Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd.
 *          All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRANTY of ANY KIND is provided.
 *
 *******************************************************************************************************/


#ifndef _CO_VERSION_H_
#define _CO_VERSION_H_
/**
 ****************************************************************************************
 * @defgroup CO_VERSION Version Defines
 * @ingroup COMMON
 *
 * @brief Bluetooth Controller Version definitions.
 *
 * @{
 ****************************************************************************************
 */
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h" // SW configuration options


#if (BLE_HW_50_ISO == 0) // BLE 5.0 HW
    /// RWBT SW Major Version
    #define RWBT_SW_VERSION_MAJOR (RW_BT50_VERSION)
    /// RWBT SW Minor Version
    #define RWBT_SW_VERSION_MINOR 0
    /// RWBT SW Build Version
    #define RWBT_SW_VERSION_BUILD 19
    /// RWBT SW Major Version
    #define RWBT_SW_VERSION_SUB_BUILD 0

#else // (BLE_HW_50_ISO == 1)       // BLE 5.0 + ISO HW
    /// RWBT SW Major Version
    #define RWBT_SW_VERSION_MAJOR     (RW_BT50_VERSION)
    /// RWBT SW Minor Version
    #define RWBT_SW_VERSION_MINOR     0xA
    /// RWBT SW Build Version
    #define RWBT_SW_VERSION_BUILD     9
    /// RWBT SW Major Version
    #define RWBT_SW_VERSION_SUB_BUILD 0
#endif // (BLE_HW_50_ISO == 0)


/// RWBT SW Major Version
#define VND_RWBT_SW_VERSION_MAJOR (RW_BT60_VERSION)
/// RWBT SW Minor Version
#define VND_RWBT_SW_VERSION_MINOR 0
/// RWBT SW Build Version
#define VND_RWBT_SW_VERSION_BUILD 19
/// RWBT SW Major Version
#define VND_RWBT_SW_VERSION_SUB_BUILD 0


/// @} CO_VERSION


#endif // _CO_VERSION_H_
