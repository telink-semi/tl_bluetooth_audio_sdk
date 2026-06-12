/********************************************************************************************************
 * @file    tpsll_hci_al.h
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
#ifndef TPSLL_HCI_ABSTRACTION_LAYER_H_
#define TPSLL_HCI_ABSTRACTION_LAYER_H_

#include "stack/tpsll/host/tpsll_hostevent.h"

void tph_host_hal_set_dongle_mac_existed(uint8_t en);

void tph_host_hal_set_dongle_mac(uint8_t *mac);

void tph_host_hal_set_local_mac(uint8_t *mac);

void tph_host_hal_start_disconn(uint8_t reason);

void tph_host_hal_get_ac_chn_from_mac(uint8_t *mac, uint8_t *ac, uint8_t *chn);

void tph_host_hal_set_setup_ac_chn(uint32_t setup_ac, uint8_t setup_chn);

void tph_host_hal_set_gen_mac(uint8_t *mac);

void tph_host_hal_start_connection_setup(uint8_t setup_mode);

void tph_host_hal_exit_connection_setup(void);

void tph_host_hal_send_pdu_msg(uint8_t msgType, uint8_t *pData, uint8_t dataLen, void (*cb)(uint8_t, uint8_t, uint8_t));

void tpd_host_hal_set_local_mac(uint8_t *mac);

void tpmd_host_hal_set_local_mac(uint8_t *mac);

#endif
