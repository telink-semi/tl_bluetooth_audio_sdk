/********************************************************************************************************
 * @file    mailbox.h
 *
 * @brief   This is the header file for tl322x
 *
 * @author  Driver Group
 * @date    2024
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
 * @brief     This function servers to set mailbox irq mask for d25f.
 * @return    none
 */
static inline void mailbox_set_irq_mask_d25f(void)
{
    BM_SET(reg_mailbox_irq_mask0, BIT(0));
}

/**
 * @brief     This function servers to set mailbox irq mask for n22.
 * @return    none
 */
static inline void mailbox_set_irq_mask_n22(void)
{
    BM_SET(reg_mailbox_irq_mask1, BIT(0));
}

/**
 * @brief     This function servers to clr mailbox irq mask for d25f.
 * @return    none
 */
static inline void mailbox_clr_irq_mask_d25f(void)
{
    BM_CLR(reg_mailbox_irq_mask0, BIT(0));
}

/**
 * @brief     This function servers to clr mailbox irq mask for n22.
 * @return    none
 */
static inline void mailbox_clr_irq_mask_n22(void)
{
    BM_CLR(reg_mailbox_irq_mask1, BIT(0));
}

/**
 * @brief     This function servers to get the mailbox interrupt status from d25f.
 * @return    irq status.
 */
static inline unsigned char mailbox_get_irq_status_d25f(void)
{
    return reg_mailbox_irq_status0;
}

/**
 * @brief     This function servers to get the mailbox interrupt status from n22.
 * @return    irq status.
 */
static inline unsigned char mailbox_get_irq_status_n22(void)
{
    return reg_mailbox_irq_status1;
}

/**
 * @brief     This function servers to clear the mailbox interrupt for d25f.
 * @return    none.
 */
static inline void mailbox_clr_irq_status_d25f(void)
{
    reg_mailbox_irq_status0 = BIT(0);
}

/**
 * @brief     This function servers to clear the mailbox interrupt for n22.
 * @return    none.
 */
static inline void mailbox_clr_irq_status_n22(void)
{
    reg_mailbox_irq_status1 = BIT(0); 
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
 * @brief     This function servers to enable mailbox auto clear message registers after read message for both d25f and n22.
 * @return    none.
 */
static inline void mailbox_auto_read_clr_msg_en(void)
{
    BM_SET(reg_mailbox_auto_clr, BIT(0)); //when this bit is set, msg registers will be cleared after reading last word msg.
}

/**
 * @brief     This function servers to disable mailbox auto clear message registers after read message for both d25f and n22.
 * @return    none.
 */
static inline void mailbox_auto_read_clr_msg_dis(void)
{
    BM_CLR(reg_mailbox_auto_clr, BIT(0));
}

/* --------------------------  n22 mailbox  --------------------------*/
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
