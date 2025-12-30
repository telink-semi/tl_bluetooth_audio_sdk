/********************************************************************************************************
 * @file    mailbox_reg.h
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
#ifndef MAILBOX_REG_H
#define MAILBOX_REG_H
/*******************************      mailbox registers: 0x101C00    ******************************/
/*******************************    m0:D25F , m1:N22 ******************************/
#define MAILBOX_BASE_ADDR         0x80101c00

#define reg_msg_d25f_to_n22_word0 REG_ADDR32(MAILBOX_BASE_ADDR)
#define reg_msg_d25f_to_n22_word1 REG_ADDR32(MAILBOX_BASE_ADDR + 0x04)

#define reg_msg_n22_to_d25f_word0 REG_ADDR32(MAILBOX_BASE_ADDR + 0x08)
#define reg_msg_n22_to_d25f_word1 REG_ADDR32(MAILBOX_BASE_ADDR + 0x0c)

#define reg_mailbox_irq_mask0      REG_ADDR8(MAILBOX_BASE_ADDR + 0x30)

#define reg_mailbox_irq_mask1      REG_ADDR8(MAILBOX_BASE_ADDR + 0x31)

#define reg_mailbox_auto_clr       REG_ADDR8(MAILBOX_BASE_ADDR + 0x33)

#define reg_mailbox_irq_status0    REG_ADDR8(MAILBOX_BASE_ADDR + 0x34)

#define reg_mailbox_irq_status1    REG_ADDR8(MAILBOX_BASE_ADDR + 0x35)

#endif
