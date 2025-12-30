/********************************************************************************************************
 * @file    svc_micp.c
 *
 * @brief   This is the source file for TLSR/TL
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

#include "common/types.h"

#include "../../../l2cap/att/inc/ble_att_service.h"

#include "../aics/svc_aics.h"
#include "../micp/svc_mics.h"

/**
 * @brief      for user add default MICP service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_addMicpGroup(void)
{
    blc_svc_addMicsGroup();
    blc_svc_addAicsGroup();
}

/**
 * @brief      for user remove default MICP service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_removeMicpGroup(void)
{
    blc_svc_removeMicsGroup();
}

/**
 * @brief      for user register read or write attribute value callback function in MICP service.
 * @param[in]  readCback: read attribute value callback function pointer.
 * @param[in]  writeCback: write attribute value callback function pointer.
 * @return     none.
 */
void blc_svc_micpCbackRegister(atts_r_cb_t readCback, atts_w_cb_t writeCback)
{
    blc_svc_micsCbackRegister(readCback, writeCback);
}
