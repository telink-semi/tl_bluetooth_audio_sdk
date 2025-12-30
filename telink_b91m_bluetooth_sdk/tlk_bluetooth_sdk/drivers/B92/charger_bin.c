/********************************************************************************************************
 * @file    charger_bin.c
 *
 * @brief   This is the source file for B92
 *
 * @author  Driver Group
 * @date    2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "charger_bin.h"

unsigned int read_done  = 0;
unsigned int write_done = 0;

/**
 * @brief       This function serves the initialization of charger module's communication function.
 * @return      none.
 */
void charger_bin_init(void)
{
    analog_write_reg8(0x06, 0xfd);                          //0x06<1>Power up of low current comparator
    analog_write_reg8(0x0b, 0x77);                          //0x0b<3>choose BG ref
    analog_write_reg8(0x0d, 0x37);                          //0x0d<7>turn on BG ref current  0x0d<6:4>vref=872mV  0x0d<2:0>comp_ain<7>
    analog_write_reg8(0x22, analog_read_reg8(0x22) | 0x07); //<2>BLAS //<1>TX   //<0>RX
    analog_write_reg8(0x69, 0x40);                          //clear reset

                                                            //  write_sram8(0x801402C2, 0x05);      //speed 156
    write_sram8(0x801402C2, 0x07);                                          //speed 286
    write_sram8(0x80140376, 60);                                            //gpio function select pf6
    write_sram8(0x80140377, 60);                                            //gpio function select pf7
    write_sram8(0x8014032e, read_sram8(0x8014032e) & 0x3f);

    write_reg8(REG_CHG_BASE + 0x01, read_reg8(REG_CHG_BASE + 0x01) & 0xdf); //oen_o = 0

    charger_set_irq_mask(FLD_CHG_MASK_RD_DONE | FLD_CHG_MASK_WR_DONE);
    charger_clr_irq_status(FLD_CHG_RD_DONE_IRQ | FLD_CHG_WR_DONE_IRQ);
}

/**
 * @brief       This function serves to judge send data completion flag.
 * @return      none.
 */
void charger_bin_mst_wait_wr_idle(void)
{
    while (write_done == 0) {
    }
    write_done = 0;
}

/**
 * @brief       This function serves to judge receive data completion flag.
 * @return      0-fail, 1-success.
 */
unsigned char charger_bin_mst_wait_rd_idle(void)
{
    for (int i = 0; i < 1000000; i++) {
        if (read_done != 0) {
            read_done = 0;
            return 1;
        }
    }
    return 0;
}

/**
 * @brief       This function serves to write one byte of data to the slave.
 * @param[in]   cmd - control command.
 * @param[in]   data - data to be sent.
 * @return      none.
 */
void charger_bin_mst_wr_byte(unsigned char cmd, unsigned char data)
{
    charger_write_data(data);
    if (cmd == 1) {
        charger_control(FLD_CHG_CONTROL_CMD | FLD_CHG_CONTROL_WR_CMD);
    } else {
        charger_control(FLD_CHG_CONTROL_WR_CMD);
    }
    charger_bin_mst_wait_wr_idle();
}

/**
 * @brief       This function serves to read one byte of data from slave.
 * @return      Data received.
 */
unsigned int charger_bin_mst_rd_byte(void)
{
    charger_control(FLD_CHG_CONTROL_RD_CMD);
    if (charger_bin_mst_wait_rd_idle()) {
        return charger_read_data();
    } else {
        return 0xff;
    }
}
