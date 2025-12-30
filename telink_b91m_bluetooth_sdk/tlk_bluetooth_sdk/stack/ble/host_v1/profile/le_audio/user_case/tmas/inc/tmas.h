/********************************************************************************************************
 * @file    tmas.h
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
// TMAS: Telephony and Media Audio Service

struct tmap_role_tag
{
    uint16_t cg_support  : 1; /** < Call Gateway support */
    uint16_t ct_support  : 1; /** < Call Terminal support */
    uint16_t ums_support : 1; /** < Unicast Media Sender support */
    uint16_t umr_support : 1; /** < Unicast Media Receiver support */
    uint16_t bms_support : 1; /** < Broadcast Media Sender support */
    uint16_t bmr_support : 1; /** < Broadcast Media Receiver support */
    uint16_t reserved    : 10;
};
