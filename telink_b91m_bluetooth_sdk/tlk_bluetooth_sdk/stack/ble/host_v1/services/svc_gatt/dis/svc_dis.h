/********************************************************************************************************
 * @file    svc_dis.h
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

// DIS: Device Information Service.

struct dis_pnp
{
    uint8_t  vidSrc;
    uint16_t vid;
    uint16_t pid;
    uint16_t ver;
} __attribute__((packed));

/**
 * @brief      for user add default DIS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_addDisGroup(void);

/**
 * @brief      for user remove default DIS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_removeDisGroup(void);

/**
 * @brief      for user update PNP ID.
 * @param[in]  vid: vendor ID.
 * @param[in]  pid: product ID.
 * @param[in]  ver: version.
 * @return     none.
 * @note Default using Bluetooth SIG assigned Company ID.
 */
void blc_svc_dis_update_pnp_id(uint16_t vid, uint16_t pid, uint16_t ver);
