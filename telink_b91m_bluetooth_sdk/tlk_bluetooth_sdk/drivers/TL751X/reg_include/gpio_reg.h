/********************************************************************************************************
 * @file    gpio_reg.h
 *
 * @brief   This is the header file for tl751x
 *
 * @author  Driver Group
 * @date    2023
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
#ifndef GPIO_REG_H_
#define GPIO_REG_H_
#include "soc.h"
/*******************************      gpio registers: 0x140c00      ******************************/
#define GPIO_BASE_ADDR            0x140c00
#define reg_gpio_pa_setting1      REG_ADDR32(GPIO_BASE_ADDR)
#define reg_gpio_pa_in            REG_ADDR8(GPIO_BASE_ADDR)
#define reg_gpio_pa_ie            REG_ADDR8(GPIO_BASE_ADDR + 0x01)
#define reg_gpio_pa_oen           REG_ADDR8(GPIO_BASE_ADDR + 0x02)
#define reg_gpio_pa_setting2      REG_ADDR32(GPIO_BASE_ADDR + 0x04)
#define reg_gpio_pa_pol           REG_ADDR8(GPIO_BASE_ADDR + 0x04)
#define reg_gpio_pa_ds            REG_ADDR8(GPIO_BASE_ADDR + 0x05)
#define reg_gpio_pa_gpio          REG_ADDR8(GPIO_BASE_ADDR + 0x06)

#define reg_gpio_pa_pd            REG_ADDR8(GPIO_BASE_ADDR + 0x0a)
#define reg_gpio_pa_pu            REG_ADDR8(GPIO_BASE_ADDR + 0x0b)
#define reg_gpio_pa_out_set_clear REG_ADDR16(GPIO_BASE_ADDR+ 0x0c)
#define reg_gpio_pa_out_set       REG_ADDR8(GPIO_BASE_ADDR + 0x0c)
#define reg_gpio_pa_out_clear     REG_ADDR8(GPIO_BASE_ADDR + 0x0d)
#define reg_gpio_pa_out_toggle    REG_ADDR8(GPIO_BASE_ADDR + 0x0e)


#define reg_gpio_pb_setting1      REG_ADDR32(GPIO_BASE_ADDR + 0x10)
#define reg_gpio_pb_in            REG_ADDR8(GPIO_BASE_ADDR + 0x10)
#define reg_gpio_pb_ie            REG_ADDR8(GPIO_BASE_ADDR + 0x11)
#define reg_gpio_pb_oen           REG_ADDR8(GPIO_BASE_ADDR + 0x12)
#define reg_gpio_pb_setting2      REG_ADDR32(GPIO_BASE_ADDR + 0x14)
#define reg_gpio_pb_pol           REG_ADDR8(GPIO_BASE_ADDR + 0x14)
#define reg_gpio_pb_ds            REG_ADDR8(GPIO_BASE_ADDR + 0x15)
#define reg_gpio_pb_gpio          REG_ADDR8(GPIO_BASE_ADDR + 0x16)

#define reg_gpio_pb_pd            REG_ADDR8(GPIO_BASE_ADDR + 0x1a)
#define reg_gpio_pb_pu            REG_ADDR8(GPIO_BASE_ADDR + 0x1b)
#define reg_gpio_pb_out_set_clear REG_ADDR16(GPIO_BASE_ADDR + 0x1c)
#define reg_gpio_pb_out_set       REG_ADDR8(GPIO_BASE_ADDR + 0x1c)
#define reg_gpio_pb_out_clear     REG_ADDR8(GPIO_BASE_ADDR + 0x1d)
#define reg_gpio_pb_out_toggle    REG_ADDR8(GPIO_BASE_ADDR + 0x1e)

#define reg_gpio_pc_setting1      REG_ADDR32(GPIO_BASE_ADDR + 0x20)
#define reg_gpio_pc_in            REG_ADDR8(GPIO_BASE_ADDR + 0x20)
#define reg_gpio_pc_ie            REG_ADDR8(GPIO_BASE_ADDR + 0x21)
#define reg_gpio_pc_oen           REG_ADDR8(GPIO_BASE_ADDR + 0x22)
#define reg_gpio_pc_setting2      REG_ADDR32(GPIO_BASE_ADDR + 0x24)
#define reg_gpio_pc_pol           REG_ADDR8(GPIO_BASE_ADDR + 0x24)
#define reg_gpio_pc_ds            REG_ADDR8(GPIO_BASE_ADDR + 0x25)
#define reg_gpio_pc_gpio          REG_ADDR8(GPIO_BASE_ADDR + 0x26)
#define reg_gpio_pc_pu            REG_ADDR8(GPIO_BASE_ADDR + 0x2b)
#define reg_gpio_pc_out_set_clear REG_ADDR16(GPIO_BASE_ADDR+ 0x2c)
#define reg_gpio_pc_out_set       REG_ADDR8(GPIO_BASE_ADDR + 0x2c)
#define reg_gpio_pc_out_clear     REG_ADDR8(GPIO_BASE_ADDR + 0x2d)
#define reg_gpio_pc_out_toggle    REG_ADDR8(GPIO_BASE_ADDR + 0x2e)

#define reg_gpio_pd_setting1      REG_ADDR32(GPIO_BASE_ADDR + 0x30)
#define reg_gpio_pd_in            REG_ADDR8(GPIO_BASE_ADDR + 0x30)
#define reg_gpio_pd_ie            REG_ADDR8(GPIO_BASE_ADDR + 0x31)
#define reg_gpio_pd_oen           REG_ADDR8(GPIO_BASE_ADDR + 0x32)
#define reg_gpio_pd_oen           REG_ADDR8(GPIO_BASE_ADDR + 0x32)
#define reg_gpio_pd_setting2      REG_ADDR32(GPIO_BASE_ADDR + 0x34)
#define reg_gpio_pd_pol           REG_ADDR8(GPIO_BASE_ADDR + 0x34)
#define reg_gpio_pd_ds            REG_ADDR8(GPIO_BASE_ADDR + 0x35)
#define reg_gpio_pd_gpio          REG_ADDR8(GPIO_BASE_ADDR + 0x36)
#define reg_gpio_pd_pu            REG_ADDR8(GPIO_BASE_ADDR + 0x3b)
#define reg_gpio_pd_out_set_clear REG_ADDR16(GPIO_BASE_ADDR + 0x3c)
#define reg_gpio_pd_out_set       REG_ADDR8(GPIO_BASE_ADDR + 0x3c)
#define reg_gpio_pd_out_clear     REG_ADDR8(GPIO_BASE_ADDR + 0x3d)
#define reg_gpio_pd_out_toggle    REG_ADDR8(GPIO_BASE_ADDR + 0x3e)


#define reg_gpio_pe_setting1      REG_ADDR32(GPIO_BASE_ADDR + 0x40)
#define reg_gpio_pe_in            REG_ADDR8(GPIO_BASE_ADDR + 0x40)
#define reg_gpio_pe_ie            REG_ADDR8(GPIO_BASE_ADDR + 0x41)
#define reg_gpio_pe_oen           REG_ADDR8(GPIO_BASE_ADDR + 0x42)
#define reg_gpio_pe_setting2      REG_ADDR32(GPIO_BASE_ADDR + 0x44)
#define reg_gpio_pe_pol           REG_ADDR8(GPIO_BASE_ADDR + 0x44)
#define reg_gpio_pe_ds            REG_ADDR8(GPIO_BASE_ADDR + 0x45)
#define reg_gpio_pe_gpio          REG_ADDR8(GPIO_BASE_ADDR + 0x46)
#define reg_gpio_pe_pd            REG_ADDR8(GPIO_BASE_ADDR + 0x4a)
#define reg_gpio_pe_pu            REG_ADDR8(GPIO_BASE_ADDR + 0x4b)
#define reg_gpio_pe_out_set_clear REG_ADDR16(GPIO_BASE_ADDR + 0x4c)
#define reg_gpio_pe_out_set       REG_ADDR8(GPIO_BASE_ADDR + 0x4c)
#define reg_gpio_pe_out_clear     REG_ADDR8(GPIO_BASE_ADDR + 0x4d)
#define reg_gpio_pe_out_toggle    REG_ADDR8(GPIO_BASE_ADDR + 0x4e)

#define reg_gpio_pf_setting1      REG_ADDR32(GPIO_BASE_ADDR + 0x50)
#define reg_gpio_pf_in            REG_ADDR8(GPIO_BASE_ADDR + 0x50)
#define reg_gpio_pf_ie            REG_ADDR8(GPIO_BASE_ADDR + 0x51)
#define reg_gpio_pf_oen           REG_ADDR8(GPIO_BASE_ADDR + 0x52)
#define reg_gpio_pf_setting2      REG_ADDR32(GPIO_BASE_ADDR + 0x54)
#define reg_gpio_pf_pol           REG_ADDR8(GPIO_BASE_ADDR + 0x54)
#define reg_gpio_pf_ds            REG_ADDR8(GPIO_BASE_ADDR + 0x55)
#define reg_gpio_pf_gpio          REG_ADDR8(GPIO_BASE_ADDR + 0x56)
#define reg_gpio_pf_pu            REG_ADDR8(GPIO_BASE_ADDR + 0x5b)
#define reg_gpio_pf_out_set_clear REG_ADDR16(GPIO_BASE_ADDR + 0x5c)
#define reg_gpio_pf_out_set       REG_ADDR8(GPIO_BASE_ADDR + 0x5c)
#define reg_gpio_pf_out_clear     REG_ADDR8(GPIO_BASE_ADDR + 0x5d)
#define reg_gpio_pf_out_toggle    REG_ADDR8(GPIO_BASE_ADDR + 0x5e)

#define reg_gpio_pg_setting1      REG_ADDR32(GPIO_BASE_ADDR + 0x60)
#define reg_gpio_pg_in            REG_ADDR8(GPIO_BASE_ADDR + 0x60)
#define reg_gpio_pg_ie            REG_ADDR8(GPIO_BASE_ADDR + 0x61)
#define reg_gpio_pg_oen           REG_ADDR8(GPIO_BASE_ADDR + 0x62)
#define reg_gpio_pg_setting2      REG_ADDR32(GPIO_BASE_ADDR + 0x64)
#define reg_gpio_pg_pol           REG_ADDR8(GPIO_BASE_ADDR + 0x64)
#define reg_gpio_pg_ds            REG_ADDR8(GPIO_BASE_ADDR + 0x65)
#define reg_gpio_pg_gpio          REG_ADDR8(GPIO_BASE_ADDR + 0x66)
#define reg_gpio_pg_pd            REG_ADDR8(GPIO_BASE_ADDR + 0x6a)
#define reg_gpio_pg_pu            REG_ADDR8(GPIO_BASE_ADDR + 0x6b)
#define reg_gpio_pg_out_set_clear REG_ADDR16(GPIO_BASE_ADDR + 0x6c)
#define reg_gpio_pg_out_set       REG_ADDR8(GPIO_BASE_ADDR + 0x6c)
#define reg_gpio_pg_out_clear     REG_ADDR8(GPIO_BASE_ADDR + 0x6d)
#define reg_gpio_pg_out_toggle    REG_ADDR8(GPIO_BASE_ADDR + 0x6e)

#define reg_gpio_ph_setting1      REG_ADDR32(GPIO_BASE_ADDR + 0x70)
#define reg_gpio_ph_in            REG_ADDR8(GPIO_BASE_ADDR + 0x70)
#define reg_gpio_ph_ie            REG_ADDR8(GPIO_BASE_ADDR + 0x71)
#define reg_gpio_ph_oen           REG_ADDR8(GPIO_BASE_ADDR + 0x72)
#define reg_gpio_ph_setting2      REG_ADDR32(GPIO_BASE_ADDR + 0x74)
#define reg_gpio_ph_pol           REG_ADDR8(GPIO_BASE_ADDR + 0x74)
#define reg_gpio_ph_ds            REG_ADDR8(GPIO_BASE_ADDR + 0x75)
#define reg_gpio_ph_gpio          REG_ADDR8(GPIO_BASE_ADDR + 0x76)
#define reg_gpio_ph_pu            REG_ADDR8(GPIO_BASE_ADDR + 0x7b)
#define reg_gpio_ph_out_set_clear REG_ADDR16(GPIO_BASE_ADDR + 0x7c)
#define reg_gpio_ph_out_set       REG_ADDR8(GPIO_BASE_ADDR + 0x7c)
#define reg_gpio_ph_out_clear     REG_ADDR8(GPIO_BASE_ADDR + 0x7d)
#define reg_gpio_ph_out_toggle    REG_ADDR8(GPIO_BASE_ADDR + 0x7e)


#define reg_gpio_pi_setting1      REG_ADDR32(GPIO_BASE_ADDR + 0x80)
#define reg_gpio_pi_in            REG_ADDR8(GPIO_BASE_ADDR + 0x80)
#define reg_gpio_pi_ie            REG_ADDR8(GPIO_BASE_ADDR + 0x81)
#define reg_gpio_pi_oen           REG_ADDR8(GPIO_BASE_ADDR + 0x82)
#define reg_gpio_pi_setting2      REG_ADDR32(GPIO_BASE_ADDR + 0x84)
#define reg_gpio_pi_ds            REG_ADDR8(GPIO_BASE_ADDR + 0x85)
#define reg_gpio_pi_gpio          REG_ADDR8(GPIO_BASE_ADDR + 0x86)
#define reg_gpio_pi_setting3      REG_ADDR32(GPIO_BASE_ADDR + 0x88)
#define reg_gpio_pi_pd            REG_ADDR8(GPIO_BASE_ADDR + 0x8a)
#define reg_gpio_pi_pu            REG_ADDR8(GPIO_BASE_ADDR + 0x8b)
#define reg_gpio_pi_out_set_clear REG_ADDR16(GPIO_BASE_ADDR + 0x8c)
#define reg_gpio_pi_out_set       REG_ADDR8(GPIO_BASE_ADDR + 0x8c)
#define reg_gpio_pi_out_clear     REG_ADDR8(GPIO_BASE_ADDR + 0x8d)
#define reg_gpio_pi_out_toggle    REG_ADDR8(GPIO_BASE_ADDR + 0x8e)

#define areg_gpio_pana_setting    0x13d
#define areg_gpio_pana_pu         0x13e
#define areg_gpio_pana_out_set    0x13f
#define areg_gpio_pana_out_clear  0x140
#define areg_gpio_pana_out_toggle 0x141

#define reg_gpio_pana_in          REG_ADDR8(GPIO_BASE_ADDR + 0xa0)

#define reg_gpio_in(i)            REG_ADDR8(GPIO_BASE_ADDR + ((i >> 8) << 4))
#define reg_gpio_ie(i)            REG_ADDR8(GPIO_BASE_ADDR + 0x01 + ((i >> 8) << 4))
#define reg_gpio_oen(i)           REG_ADDR8(GPIO_BASE_ADDR + 0x02 + ((i >> 8) << 4))
#define reg_gpio_pol(i)           REG_ADDR8(GPIO_BASE_ADDR + 0x04 + ((i >> 8) << 4))
#define reg_gpio_ds(i)            REG_ADDR8(GPIO_BASE_ADDR + 0x05 + ((i >> 8) << 4))
#define reg_gpio_func(i)          REG_ADDR8(GPIO_BASE_ADDR + 0x06 + ((i >> 8) << 4))
#define reg_gpio_pd(i)            REG_ADDR8(GPIO_BASE_ADDR + 0x0a + ((i >> 8) << 4))
#define reg_gpio_pu(i)            REG_ADDR8(GPIO_BASE_ADDR + 0x0b + ((i >> 8) << 4))
#define reg_gpio_out_set_clear(i) REG_ADDR16(GPIO_BASE_ADDR + 0x0c + ((i >> 8) << 4))
#define reg_gpio_out_set(i)       REG_ADDR8(GPIO_BASE_ADDR + 0x0c + ((i >> 8) << 4))
#define reg_gpio_out_clear(i)     REG_ADDR8(GPIO_BASE_ADDR + 0x0d + ((i >> 8) << 4))
#define reg_gpio_out_toggle(i)    REG_ADDR8(GPIO_BASE_ADDR + 0x0e + ((i >> 8) << 4))
#define reg_gpio_irq_en(i, j)     REG_ADDR8((GPIO_BASE_ADDR + 0x108 + (j)) + ((i >> 8) << 3))
#define reg_gpio_irq0_en(i)       REG_ADDR8(GPIO_BASE_ADDR + 0x108 + ((i >> 8) << 3))
#define reg_gpio_irq1_en(i)       REG_ADDR8(GPIO_BASE_ADDR + 0x109 + ((i >> 8) << 3))
#define reg_gpio_irq2_en(i)       REG_ADDR8(GPIO_BASE_ADDR + 0x10a + ((i >> 8) << 3))
#define reg_gpio_irq3_en(i)       REG_ADDR8(GPIO_BASE_ADDR + 0x10b + ((i >> 8) << 3))
#define reg_gpio_irq4_en(i)       REG_ADDR8(GPIO_BASE_ADDR + 0x10c + ((i >> 8) << 3))
#define reg_gpio_irq5_en(i)       REG_ADDR8(GPIO_BASE_ADDR + 0x10d + ((i >> 8) << 3))
#define reg_gpio_irq6_en(i)       REG_ADDR8(GPIO_BASE_ADDR + 0x10e + ((i >> 8) << 3))
#define reg_gpio_irq7_en(i)       REG_ADDR8(GPIO_BASE_ADDR + 0x10f + ((i >> 8) << 3))

#define reg_gpio_func_mux(i)      REG_ADDR8(GPIO_BASE_ADDR + 0xb0 + ((i >> 8) << 3) + BIT_LOW_BIT(i))

#define reg_gpio_irq_ctrl         REG_ADDR8(GPIO_BASE_ADDR + 0x102)

enum
{
    FLD_GPIO_CORE_WAKEUP_EN    = BIT(0),
    FLD_GPIO_CORE_INTERRUPT_EN = BIT(1),
};

#define reg_gpio_irq_level    REG_ADDR8(GPIO_BASE_ADDR + 0x104)
#define reg_gpio_irq_src_mask REG_ADDR8(GPIO_BASE_ADDR + 0x105)
#define reg_gpio_irq_clr      REG_ADDR8(GPIO_BASE_ADDR + 0x106)


#endif
