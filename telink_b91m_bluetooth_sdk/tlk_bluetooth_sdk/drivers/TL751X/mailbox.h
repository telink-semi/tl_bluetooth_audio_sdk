/********************************************************************************************************
 * @file    mailbox.h
 *
 * @brief   This is the header file for tl751x
 *
 * @author  Driver Group
 * @date    2024
 *
 * @par     Copyright (c) 2023, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#ifndef MAILBOX_H_
#define MAILBOX_H_
#include "reg_include/register.h"

/**
 * @brief     This function servers to set mailbox irq mask.
 * @param[in] mask - mailbox irq mask.
 * @return    none
 */
static inline void mailbox_set_irq_mask(mailbox_irq_status_e mask)
{
    BM_SET(reg_mailbox_irq_mask, mask);
}

/**
 * @brief     This function servers to clr mailbox irq mask.
 * @param[in] mask - mailbox irq mask.
 * @return    none
 */
static inline void mailbox_clr_irq_mask(mailbox_irq_status_e mask)
{
    BM_CLR(reg_mailbox_irq_mask, mask);
}

/**
 * @brief     This function servers to get the mailbox interrupt status.
 * @param[in] status    - variable of enum to select the mailbox interrupt source.
 * @retval    non-zero      - the interrupt occurred.
 * @retval    zero  - the interrupt did not occur.
 */
static inline unsigned char mailbox_get_irq_status(void)
{
    return reg_mailbox_irq_status;
}

/**
 * @brief     This function servers to clear the mailbox interrupt.
 * @param[in] status  - variable of enum to select the mailbox interrupt source.
 * @return    none.
 */
static inline void mailbox_clr_irq_status(mailbox_irq_status_e status)
{
    reg_mailbox_irq_status = status;
}

/**
 * @brief     This function servers to get message send from dsp to d25f.
 * @param[in] msg_words - message buffer, two words
 * @return    none.     
 */
static inline void mailbox_d25f_get_dsp_msg(unsigned int *msg_words)
{
    msg_words[0] = reg_msg_dsp_to_d25f_word0;
    msg_words[1] = reg_msg_dsp_to_d25f_word1;
}

/**
 * @brief     This function servers to set message send from d25f to dsp.
 * @param[in] msg_words - message buffer, two words
 * @return    none.     
 */
static inline void mailbox_d25f_set_dsp_msg(unsigned int *msg_words)
{
    reg_msg_d25f_to_dsp_word0 = msg_words[0];
    reg_msg_d25f_to_dsp_word1 = msg_words[1];
}

/**
 * @brief     This function servers to get message send from n22 to d25f.
 * @param[in] msg_words - message buffer, two words
 * @return    none.     
 */
static inline void mailbox_d25f_get_n22_msg(unsigned int *msg_words)
{
    msg_words[0] = reg_msg_n22_to_d25f_word0;
    msg_words[1] = reg_msg_n22_to_d25f_word1;
}

/**
 * @brief     This function servers to set message send from d25f to n22.
 * @param[in] msg_words - message buffer, two words
 * @return    none.     
 */
static inline void mailbox_d25f_set_n22_msg(unsigned int *msg_words)
{
    reg_msg_d25f_to_n22_word0 = msg_words[0];
    reg_msg_d25f_to_n22_word1 = msg_words[1];
}

/**
 * @brief     This function servers to enable mailbox auto clear message registers after read message.
 * @return    none.     
 */
static inline void mailbox_auto_read_clr_msg_en(void)
{
    BM_SET(reg_mailbox_irq_mask, BIT(6)); //when this bit is set, msg registers will be cleared after reading last word msg.
}

/**
 * @brief     This function servers to disable mailbox auto clear message registers after read message.
 * @return    none.     
 */
static inline void mailbox_auto_read_clr_msg_dis(void)
{
    BM_CLR(reg_mailbox_irq_mask, BIT(6));
}

/* --------------------------  n22 mailbox  --------------------------*/
/**
 * @brief     This function servers to get message send from n22 to d25f.
 * @param[in] msg_words - message buffer, two words
 * @return    none.     
 */
static inline void mailbox_n22_get_dsp_msg(unsigned int *msg_words)
{
    msg_words[0] = reg_msg_dsp_to_n22_word0;
    msg_words[1] = reg_msg_dsp_to_n22_word1;
}

/**
 * @brief     This function servers to set message send from n22 to dsp.
 * @param[in] msg_words - message buffer, two words
 * @return    none.     
 */
static inline void mailbox_n22_set_dsp_msg(unsigned int *msg_words)
{
    reg_msg_n22_to_dsp_word0 = msg_words[0];
    reg_msg_n22_to_dsp_word1 = msg_words[1];
}

/**
 * @brief     This function servers to get message send from d25f to n22.
 * @param[in] msg_words - message buffer, two words
 * @return    none.     
 */
static inline void mailbox_n22_get_d25f_msg(unsigned int *msg_words)
{
    msg_words[0] = reg_msg_d25f_to_n22_word0;
    msg_words[1] = reg_msg_d25f_to_n22_word1;
}

/**
 * @brief     This function servers to set message send from n22 to d25f.
 * @param[in] msg_words - message buffer, two words
 * @return    none.     
 */
static inline void mailbox_n22_set_d25f_msg(unsigned int *msg_words)
{
    reg_msg_n22_to_d25f_word0 = msg_words[0];
    reg_msg_n22_to_d25f_word1 = msg_words[1];
}
#endif
