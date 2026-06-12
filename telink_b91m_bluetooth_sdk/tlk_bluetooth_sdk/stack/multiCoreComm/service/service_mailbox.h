/********************************************************************************************************
 * @file    service_mailbox.h
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
#pragma once

typedef enum
{
    TLK_MESSAGE_FROM_D25F_TO_N22_WAKE_UP = 0,
    TLK_MESSAGE_FROM_D25F_TO_N22_LOOP_MASK,
    TLK_MESSAGE_FROM_D25F_TO_N22_MAX,
} tlk_mailbox_from_d25f_to_n22_e;

typedef enum
{
    TLK_MESSAGE_FROM_N22_TO_D25F_SYNC_DATA_READY = 0,
    TLK_MESSAGE_FROM_N22_TO_D25F_REQ_HANDLE_SYNC_DATA,
    TLK_MESSAGE_FROM_N22_TO_D25F_SM_DATA_READY,
    TLK_MESSAGE_FROM_N22_TO_D25F_STIMER_START_EVT,
    TLK_MESSAGE_FROM_N22_TO_D25F_HANDOVER_MASK_SET_EVT,
    TLK_MESSAGE_FROM_N22_TO_D25F_HANDOVER_MASK_CLEAR_EVT,
    TLK_MESSAGE_FROM_N22_TO_D25F_HANDOVER_INFO_EXTRACT,
    TLK_MESSAGE_FROM_N22_TO_D25F_BCST_INT,
    TLK_MESSAGE_FROM_N22_TO_D25F_BCST_REF,
    TLK_MESSAGE_FROM_N22_TO_D25F_MAX,
} tlk_mailbox_from_n22_to_d25f_e;

typedef enum
{
    TLK_SM_DATA_READY_TYPE_HCI = 0,
    TLK_SM_DATA_READY_TYPE_LOG,
    TLK_SM_DATA_READY_TYPE_MAX_NUM,
} tlk_sm_data_ready_type_e;

typedef void (*tlk_mailbox_receive_cb_f)(uint8_t *);

/**
 * @brief      This function servers to init mailbox service.
 * @param[in]  none
 * @return     none
 */
void tlk_mailbox_service_init(void);

/**
 * @brief      This function servers to send cmd
 * @param[in]  cmd   - in d25f,search in tlk_mailbox_from_d25f_to_n22_e.
 *                     in n22, search in tlk_mailbox_from_n22_to_d25f_e.
 * @param[in]  data  - data corresponding to cmd,7 bytes at most.
 * @return     none
 */
void tlk_mailbox_send_data(uint8_t cmd, uint8_t *data);

void tlk_mailbox_send_dataEx(uint8_t cmd, uint8_t *data, uint32_t maxBlockTime);

/**
 * @brief      This function servers to register rx callback in mailbox.
 * @param[in]  cmd - in d25f,search in tlk_mailbox_from_n22_to_d25f_e.
 *                   in n22, search in tlk_mailbox_from_d25f_to_n22_e.
 * @param[in]  cb  - receive callback corresponding to cmd.
 * @return     none
 */
void tlk_mailbox_register_message_cb(uint8_t cmd, tlk_mailbox_receive_cb_f cb);
