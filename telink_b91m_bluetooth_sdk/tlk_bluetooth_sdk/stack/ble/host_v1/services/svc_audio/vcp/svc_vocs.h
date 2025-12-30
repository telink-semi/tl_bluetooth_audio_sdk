/********************************************************************************************************
 * @file    svc_vocs.h
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


//VOCS: Volume Offset Control Service

typedef struct
{
    int16_t volumeOffset;
    uint8_t changeCount;
} svc_vocsVolOffState_t;

/**
 * @brief      for user add default VOCS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_addVocsGroup(void);

/**
 * @brief      for user remove default VOCS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_removeVocsGroup(void);

/**
 * @brief      for user register read or write attribute value callback function in VOCS service.
 * @param[in]  readCback: read attribute value callback function pointer.
 * @param[in]  writeCback: write attribute value callback function pointer.
 * @return     none.
 */
void blc_svc_vocsCbackRegister(atts_r_cb_t readCback, atts_w_cb_t writeCback);
