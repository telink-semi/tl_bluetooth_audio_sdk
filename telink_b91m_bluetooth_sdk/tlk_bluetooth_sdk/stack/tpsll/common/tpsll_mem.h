/********************************************************************************************************
 * @file    tpsll_mem.h
 *
 * @brief   This is the header file for TLSR/TL
 *
 * @author  Bluetooth Group
 * @date    2026
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

#ifndef TPSLL_MEM_H
#define TPSLL_MEM_H

#include "tl_common.h"

#if (TLK_STK_TPSLL_CONTROLLER_ENABLE)

typedef enum
{
    TPSLL_ROLE_NONE = 0,
    TPSLL_ROLE_TPT,
    TPSLL_ROLE_TPH,
    TPSLL_ROLE_TPDT,
    TPSLL_ROLE_TPMD,
    TPSLL_ROLE_TPD,
    TPSLL_ROLE_MAX,
} tpsll_role_t;

typedef enum
{
    TPSLL_FIFO_ACL_TX = 0,
    TPSLL_FIFO_LMP_TX,
    TPSLL_FIFO_SCO_TX,
    TPSLL_FIFO_ACL_RX,
    TPSLL_FIFO_LMP_RX,
    TPSLL_FIFO_SCO_RX,
    TPSLL_FIFO_MAX,
} tpsll_fifo_id_t;

extern tlk_fifo_t g_tlk_tpsll_acl_msg_txfifo;
extern tlk_fifo_t g_tlk_tpsll_lmp_msg_txfifo;
extern tlk_fifo_t g_tlk_tpsll_sco_msg_txfifo;
extern tlk_fifo_t g_tlk_tpsll_acl_msg_rxfifo;
extern tlk_fifo_t g_tlk_tpsll_lmp_msg_rxfifo;
extern tlk_fifo_t g_tlk_tpsll_sco_msg_rxfifo;

extern int  tlk_tpsll_mem_init(tpsll_role_t role);
extern void tlk_tpsll_fifo_reset(tpsll_fifo_id_t id);
extern void tlk_tpsll_mem_reset(void);

#endif
#endif /* TPSLL_MEM_H */
