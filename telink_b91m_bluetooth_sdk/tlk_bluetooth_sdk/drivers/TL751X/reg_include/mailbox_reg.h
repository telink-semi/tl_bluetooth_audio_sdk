/********************************************************************************************************
 * @file    mailbox_reg.h
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
#ifndef MAILBOX_REG_H
#define MAILBOX_REG_H
/*******************************      mailbox registers: 0x80101C00    ******************************/
/*******************************    m0:D25F , m1:N22 , m2:DSP ******************************/
#define MAILBOX_BASE_ADDR         0x80101C00

#define reg_msg_d25f_to_dsp_word0 REG_ADDR32(MAILBOX_BASE_ADDR)
#define reg_msg_d25f_to_dsp_word1 REG_ADDR32(MAILBOX_BASE_ADDR + 0x04)

#define reg_msg_dsp_to_d25f_word0 REG_ADDR32(MAILBOX_BASE_ADDR + 0x08)
#define reg_msg_dsp_to_d25f_word1 REG_ADDR32(MAILBOX_BASE_ADDR + 0x0c)

#define reg_msg_d25f_to_n22_word0 REG_ADDR32(MAILBOX_BASE_ADDR + 0x10)
#define reg_msg_d25f_to_n22_word1 REG_ADDR32(MAILBOX_BASE_ADDR + 0x14)

#define reg_msg_n22_to_d25f_word0 REG_ADDR32(MAILBOX_BASE_ADDR + 0x18)
#define reg_msg_n22_to_d25f_word1 REG_ADDR32(MAILBOX_BASE_ADDR + 0x1c)

#define reg_msg_n22_to_dsp_word0  REG_ADDR32(MAILBOX_BASE_ADDR + 0x20)
#define reg_msg_n22_to_dsp_word1  REG_ADDR32(MAILBOX_BASE_ADDR + 0x24)

#define reg_msg_dsp_to_n22_word0  REG_ADDR32(MAILBOX_BASE_ADDR + 0x28)
#define reg_msg_dsp_to_n22_word1  REG_ADDR32(MAILBOX_BASE_ADDR + 0x2c)

#define reg_mailbox_irq_mask      REG_ADDR8(MAILBOX_BASE_ADDR + 0x30)

#define reg_mailbox_irq_status    REG_ADDR8(MAILBOX_BASE_ADDR + 0x31)

typedef enum
{
    FLD_MAILBOX_D25F_TO_DSP_IRQ = BIT(0), /*when d25f write two words into d25f_to_dsp register, dsp will generate an interrupt and this interrupt flag bit will be set.
                                            After dsp read two words, the hardware will automatically clear the interrupt flag bit (this function is hardware fixed line, not register configurable)*/

    FLD_MAILBOX_DSP_TO_D25F_IRQ = BIT(1), /*when dsp  write two words into dsp_to_d25f register, d25f will generate an interrupt and this interrupt flag bit will be set.
                                            After d25f read two words, the hardware will automatically clear the interrupt flag bit (this function is hardware fixed line, not register configurable)*/

    FLD_MAILBOX_D25F_TO_N22_IRQ = BIT(2), /*when d25f write two words into d25f_to_n22 register, n22 will generate an interrupt and this interrupt flag bit will be set.
                                            After n22 read two words, the hardware will automatically clear the interrupt flag bit (this function is hardware fixed line, not register configurable)*/

    FLD_MAILBOX_N22_TO_D25F_IRQ = BIT(3), /*when n22 write two words into n22_to_d25f register, d25f will generate an interrupt and this interrupt flag bit will be set.
                                            After d25f read two words, the hardware will automatically clear the interrupt flag bit (this function is hardware fixed line, not register configurable)*/

    FLD_MAILBOX_N22_TO_DSP_IRQ = BIT(4),  /*when n22 write two words into n22_to_dsp register, dsp will generate an interrupt and this interrupt flag bit will be set.
                                            After dsp read two words, the hardware will automatically clear the interrupt flag bit (this function is hardware fixed line, not register configurable)*/

    FLD_MAILBOX_DSP_TO_N22_IRQ = BIT(5),  /*when dsp write two words into dsp_to_n22 register, n22 will generate an interrupt and this interrupt flag bit will be set.
                                            After n22 read two words, the hardware will automatically clear the interrupt flag bit (this function is hardware fixed line, not register configurable)*/
} mailbox_irq_status_e;
#endif
