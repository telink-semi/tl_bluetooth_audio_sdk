/********************************************************************************************************
 * @file    svc_hid_iso.h
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
// ULL-HID: Ultra Low Latency Human Interface Device Service.

/**
 * @brief      for user add default ULL-HID service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_addUllhidGroup(void);

/**
 * @brief      for user remove default ULL-HID service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_removeUllhidGroup(void);

/**
 * @brief      for user register read or write attribute value callback function in ULL-HID service.
 * @param[in]  writeCback: write attribute value callback function pointer.
 * @param[in]  readCback: read attribute value callback function pointer.
 * @return     none.
 */
void blc_svc_ullhidCbackRegister(atts_w_cb_t writeCback, atts_r_cb_t readCback);
