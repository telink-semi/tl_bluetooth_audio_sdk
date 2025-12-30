/********************************************************************************************************
 * @file    prf_basic.h
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
/*
 * It is used to implement profiles commonly used before Bluetooth LE Core v5.2,
 * usually these are the Basic services of LE devices, so they are also called basic profiles.
 * all profile: Generic Attribute Service(GATT), Generic Access Profile Service(GAP), Device Information Service(DIS),
 * Battery Service(BAS), Scan Parameters Service(ScPS).
 */

enum
{
    BASIC_SERVICE_ID_START = PRF_BASIC_SERVICE_ID_START - 1,
    SERVICE_ID_GATT,
    SERVICE_ID_GAP,
    SERVICE_ID_BAS,
    SERVICE_ID_DIS,
    SERVICE_ID_SCPS,
};
