/********************************************************************************************************
 * @file    gpio.c
 *
 * @brief   This is the source file for tl751x
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
#include "gpio.h"

/**********************************************************************************************************************
 *                                            local constants                                                       *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *                                              local macro                                                        *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *                                             local data type                                                     *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *                                              global variable                                                       *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                              local variable                                                     *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                          local function prototype                                               *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                         global function implementation                                             *
 *********************************************************************************************************************/

/**
 * @brief      This function serves to set the gpio-mux function.
 * @param[in]  pin      - the pin needs to set.
 * @param[in]  function - the function need to set.
 * @return     none.
 */
void gpio_set_mux_function(gpio_func_pin_e pin, gpio_func_e function)
{
    reg_gpio_func_mux(pin) = function;
}

/**
 * @brief      This function enable the input function of a pin.
 * @param[in]  pin - the pin needs to set the input function.
 * @return     none.
 */
void gpio_input_en(gpio_pin_e pin)
{
    unsigned char bit = pin & 0xff;
    if (((pin >> 8) & 0xff) == GPIO_GROUP_ANA) {
        analog_write_reg8(areg_gpio_pana_setting, (analog_read_reg8(areg_gpio_pana_setting)) | (bit));
    } else {
        BM_SET(reg_gpio_ie(pin), bit);
    }
}

/**
 * @brief      This function disable the input function of a pin.
 * @param[in]  pin - the pin needs to set the input function.
 * @return     none.
 */
void gpio_input_dis(gpio_pin_e pin)
{
    unsigned char bit = pin & 0xff;
    if (((pin >> 8) & 0xff) == GPIO_GROUP_ANA) {
        analog_write_reg8(areg_gpio_pana_setting, (analog_read_reg8(areg_gpio_pana_setting)) & (~bit));
    } else {
        BM_CLR(reg_gpio_ie(pin), bit);
    }
}

/**
 * @brief      This function set the input function of a pin.
 * @param[in]  pin - the pin needs to set the input function
 * @param[in]  value - enable or disable the pin's input function(1: enable,0: disable )
 * @return     none
 */
void gpio_set_input(gpio_pin_e pin, unsigned char value)
{
    if (value) {
        gpio_input_en(pin);
    } else {
        gpio_input_dis(pin);
    }
}

/**
 * @brief      This function servers to set the specified GPIO into High-impedance state and also enable the pull-down resistor.
 *             To prevent power leakage, you need to call gpio_shutdown(GPIO_ALL) (set all gpio to high resistance, except SWS and MSPI)
 *             as front as possible in the program, and then initialize the corresponding GPIO according to the actual using situation.
 * @param[in]  pin  - select the specified GPIO.Only support GPIO_GROUPA ~GPIO_GROUPI,GPIO_GPOUPANA and GPIO_ALL.
 * @return     none.
 */
void gpio_shutdown(gpio_pin_e pin)
{
    unsigned short group = pin & 0xf00;
    unsigned char  bit   = pin & 0xff;
    switch (group) {
    case GPIO_GROUPA:
        reg_gpio_pa_oen |= bit;            // disable output
        reg_gpio_pa_gpio |= (bit & 0x7f);
        reg_gpio_pa_ie &= ((~bit) | 0x80); //disable input
        // set 100K pull down(except SWS)
        //A<3:0>
        analog_write_reg8(0x080, 0xaa);
        //A<7:4>
        analog_write_reg8(0x081, 0x6a);//SWS set 1M pull up
        break;
    case GPIO_GROUPB:
        reg_gpio_pb_oen |= bit;
        reg_gpio_pb_gpio |= bit;
        reg_gpio_pb_ie &= (~bit);
        // set 100K pull down
        //B<3:0>
        analog_write_reg8(0x082, 0xaa);
       //B<7:4>
        analog_write_reg8(0x083, 0xaa);
        break;
    case GPIO_GROUPC:
        reg_gpio_pc_oen |= bit;
        reg_gpio_pc_gpio |= bit;
        reg_gpio_pc_ie &= (~bit);
        // set 100K pull down
        //C<3:0>
        analog_write_reg8(0x084, 0xaa);
        //C<7:4>
        analog_write_reg8(0x085, 0xaa);
        break;
    case GPIO_GROUPD:
        reg_gpio_pd_oen |= bit;
        reg_gpio_pd_gpio |= bit;
        reg_gpio_pd_ie &= (~bit);
        // set 100K pull down
        //D<3:0>
        analog_write_reg8(0x086, 0xaa);
        //D<7:4>
        analog_write_reg8(0x087, 0xaa);
        break;

    case GPIO_GROUPE:
        reg_gpio_pe_oen |= bit;
        reg_gpio_pe_gpio |= bit;
        reg_gpio_pe_ie &= (~bit);
        // set 100K pull down
        //E<3:0>
        analog_write_reg8(0x088, 0xaa);
        //E<7:4>
        analog_write_reg8(0x089, 0xaa);
        break;
    case GPIO_GROUPF:
        reg_gpio_pf_oen |= bit;
        reg_gpio_pf_gpio |= bit;
        reg_gpio_pf_ie &= (~bit);
        // set 100K pull down
        //F<3:0>
        analog_write_reg8(0x08a, 0xaa);
        //F<7:4>
        analog_write_reg8(0x08b, 0xaa);
        break;

    case GPIO_GROUPG:
        reg_gpio_pg_oen |= bit;
        reg_gpio_pg_gpio |= bit;
        reg_gpio_pg_ie &= (~bit);
        // set 100K pull down
        //G<3:0>
         analog_write_reg8(0x08c, 0xaa);
         //G<6:4>
         analog_write_reg8(0x08d, 0x2a);
        break;
    case GPIO_GROUPH://GPIO_GROUPH has no pull down resistor
        reg_gpio_ph_oen |= bit;
        reg_gpio_ph_gpio |= bit;
        reg_gpio_ph_ie &= (~bit);
        break;
    case GPIO_GROUPI://GPIO_GROUPI has no pull down resistor
        reg_gpio_pi_oen |= bit;
        reg_gpio_pi_gpio |= bit;
        reg_gpio_pi_ie &= (~bit);
        break;
    case GPIO_GPOUPANA:
        analog_write_reg8(areg_gpio_pana_setting, (analog_read_reg8(areg_gpio_pana_setting)) | (bit << 2));//ANA0/ANA1 output disable
        analog_write_reg8(areg_gpio_pana_setting, (analog_read_reg8(areg_gpio_pana_setting)) & (~bit));//ANA0/ANA1 input disable
        // set 100K pull down
        //ANA<1:0>
        analog_write_reg8(0x0aa, (analog_read_reg8(0xaa) & 0x0f) | 0xa0);
        break;
    case GPIO_ALL:
        //as gpio
        reg_gpio_pa_gpio = 0x7f;
        reg_gpio_pb_gpio = 0xff;
        reg_gpio_pc_gpio = 0xff;
        reg_gpio_pd_gpio = 0xff;
        reg_gpio_pe_gpio = 0xff;
        reg_gpio_pf_gpio = 0xff;
        reg_gpio_pg_gpio = 0xff;
        reg_gpio_ph_gpio = 0xc0;//PH0~PH5 for MSPI
        reg_gpio_pi_gpio = 0xff;

        //output disable
        reg_gpio_pa_oen = 0xff;
        reg_gpio_pb_oen = 0xff;
        reg_gpio_pc_oen = 0xff;
        reg_gpio_pd_oen = 0xff;
        reg_gpio_pe_oen = 0xff;
        reg_gpio_pf_oen = 0xff;
        reg_gpio_pg_oen = 0xff;
        reg_gpio_ph_oen = 0xc0;//PH0~PH5 for MSPI
        reg_gpio_pi_oen = 0xff;

        //disable input
        reg_gpio_pa_ie = 0x80; //SWS
        reg_gpio_pb_ie = 0x00;
        reg_gpio_pc_ie = 0x00;
        reg_gpio_pd_ie = 0x00;
        reg_gpio_pe_ie = 0x00;
        reg_gpio_pf_ie = 0x00;
        reg_gpio_pg_ie = 0x00;
        reg_gpio_ph_ie = 0x3f;//PH0~PH5 for MSPI
        reg_gpio_pi_ie = 0x00;
        analog_write_reg8(areg_gpio_pana_setting, (analog_read_reg8(areg_gpio_pana_setting) & 0xf0) | 0x0c);
        analog_write_reg8(areg_gpio_pana_out_clear, 0x03);
       
        // set 100K pull down(except SWS)
        //A<3:0>
        analog_write_reg8(0x080, 0xaa);
        //A<7:4>
        analog_write_reg8(0x081, 0x6a); ///SWS set 1M pull up
        //B<3:0>
        analog_write_reg8(0x082, 0xaa);
        //B<7:4>
        analog_write_reg8(0x083, 0xaa);
        //C<3:0>
        analog_write_reg8(0x084, 0xaa);
        //C<7:4>
        analog_write_reg8(0x085, 0xaa);
        //D<3:0>
        analog_write_reg8(0x086, 0xaa);
        //D<7:4>
        analog_write_reg8(0x087, 0xaa);
        //E<3:0>
        analog_write_reg8(0x088, 0xaa);
        //E<7:4>
        analog_write_reg8(0x089, 0xaa);
        //F<3:0>
        analog_write_reg8(0x08a, 0xaa);
        //F<7:4>
        analog_write_reg8(0x08b, 0xaa);
        //G<3:0>
        analog_write_reg8(0x08c, 0xaa);
        //G<6:4>
        analog_write_reg8(0x08d, 0x2a);
       // excluding PH and PI as there is no pull down resistor
        //ANA<1:0>
        analog_write_reg8(0x0aa, (analog_read_reg8(0xaa) & 0x0f) | 0xa0);
        break;
    }
}

/**
 * @brief     This function set a pin's IRQ , here you can choose from 8 interrupts for flexible configuration, each interrupt is independent.
 * @param[in] irq           - there are 8 types of irq to choose.(irq0/irq1/irq2/irq3/irq4/irq5/irq6/irq7)
 * @param[in] pin           - the pin needs to enable its IRQ.
 * @param[in] trigger_type  - gpio interrupt type.
 *                            0: rising edge.
 *                            1: falling edge.
 *                            2: high level.
 *                            3: low level.
 * @return    none.
 */

void gpio_set_irq(gpio_irq_num_e irq, gpio_pin_e pin, gpio_irq_trigger_type_e trigger_type)
{
    /*
        When selecting pull-up resistance and rising edge to trigger gpio interrupt, gpio_irq_en should be placed before setting gpio_set_irq,
        otherwise an interrupt will be triggered by mistake.
     */
    gpio_irq_en(pin, irq);
    gpio_clr_irq_status((gpio_irq_e)BIT(irq)); //must clear cause to unexpected interrupt.
    switch (trigger_type) {
    case INTR_RISING_EDGE:
        BM_CLR(reg_gpio_pol(pin), pin & 0xff);
        BM_CLR(reg_gpio_irq_level, BIT(irq));
        break;
    case INTR_FALLING_EDGE:
        BM_SET(reg_gpio_pol(pin), pin & 0xff);
        BM_CLR(reg_gpio_irq_level, BIT(irq));
        break;
    case INTR_HIGH_LEVEL:
        BM_CLR(reg_gpio_pol(pin), pin & 0xff);
        BM_SET(reg_gpio_irq_level, BIT(irq));
        break;
    case INTR_LOW_LEVEL:
        BM_SET(reg_gpio_pol(pin), pin & 0xff);
        BM_SET(reg_gpio_irq_level, BIT(irq));
        break;
    }
    if (irq == GPIO_IRQ0) {
        reg_gpio_irq_ctrl |= FLD_GPIO_CORE_INTERRUPT_EN; //Only GPIO_IRQ0 needs to enable FLD_GPIO_CORE_INTERRUPT_EN.
    }
}

/**
 * @brief     This function set a pin's pull-up/down resistor.
 * @param[in] pin - the pin needs to set its pull-up/down resistor.
 * @param[in] up_down_res - the type of the pull-up/down resistor.
 * @return    none.
 */
void gpio_set_up_down_res(gpio_pin_e pin, gpio_pull_type_e up_down_res)
{
    // PA[3:0]               PA[7:4]         PB[3:0]          PB[7:4]         PC[3:0]         PC[7:4]
    // sel: ana_0x80<7:0>    ana_0x81<7:0>   ana_0x82<7:0>    ana_0x83<7:0>   ana_0x84<7:0>   ana_0x85<7:0>
    // PD[3:0]               PD[7:4]         PE[3:0]          PE[7:4]         PF[3:0]         PF[7:4]
    // sel: ana_0x86<7:0>    ana_0x87<7:0>   ana_0x88<7:0>    ana_0x89<7:0>   ana_0x8a<7:0>   ana_0x8b<7:0>
    // PG[3:0]               PG[6:4]
    // sel: ana_0x8c<7:0>    ana_0x8d<5:0>
    unsigned char r_val = up_down_res & 0x03;

    unsigned char base_ana_reg = 0;
    if ((pin >> 8) < 8) {
        base_ana_reg = 0x80 + ((pin >> 8) << 1) + ((pin & 0xf0) ? 1 : 0);
    } else if((pin >> 8) == 0x0a){// pin ana0/1
        base_ana_reg = 0xaa;
        pin = (pin << 2) & 0xff;
    }
    else{
        return;
    }
    unsigned char shift_num, mask_not;

    if (pin & 0x11) {
        shift_num = 0;
        mask_not  = 0xfc;
    } else if (pin & 0x22) {
        shift_num = 2;
        mask_not  = 0xf3;
    } else if (pin & 0x44) {
        shift_num = 4;
        mask_not  = 0xcf;
    } else if (pin & 0x88) {
        shift_num = 6;
        mask_not  = 0x3f;
    } else {
        return;
    }
    analog_write_reg8(base_ana_reg, (analog_read_reg8(base_ana_reg) & mask_not) | (r_val << shift_num));
}

/**
 * @brief     This function set pin's  pull-down register.
 * @param[in] pin - the pin needs to set its pull-down register.
 * @return    none.
 * @attention  This function sets the digital pull-down, it will not work after entering low power consumption.
 */
void gpio_set_digital_pulldown(gpio_pin_e pin)
{
    unsigned char bit = pin & 0xff;
    if (((pin >> 8) & 0xff) == GPIO_GROUP_ANA) {
        analog_write_reg8(areg_gpio_pana_setting, (analog_read_reg8(areg_gpio_pana_setting)) | (bit << 6));
    } else {
        BM_SET(reg_gpio_pd(pin), bit);
    }
}

/**
 * @brief     This function set pin's  pull-up register.
 * @param[in] pin - the pin needs to set its pull-up register.
 * @return    none.
 * @attention  This function sets the digital pull-up, it will not work after entering low power consumption.
 */
void gpio_set_digital_pullup(gpio_pin_e pin)
{
    unsigned char bit = pin & 0xff;
    if (((pin >> 8) & 0xff) == GPIO_GROUP_ANA) {
        analog_write_reg8(areg_gpio_pana_pu, (analog_read_reg8(areg_gpio_pana_pu)) | bit);
    } else {
        BM_SET(reg_gpio_pu(pin), bit);
    }
}

/**
 * @brief     This function disable pin's  pull-down register.
 * @param[in] pin - the pin needs to disable its pull-down register.
 * @return    none.
 */
void gpio_digital_pulldown_dis(gpio_pin_e pin)
{
    unsigned char bit = pin & 0xff;
    if (((pin >> 8) & 0xff) == GPIO_GROUP_ANA) {
        analog_write_reg8(areg_gpio_pana_setting, (analog_read_reg8(areg_gpio_pana_setting)) & (~(bit << 6)));
    } else {
        BM_CLR(reg_gpio_pd(pin), bit);
    }
}

/**
 * @brief     This function disable pin's  pull-up register.
 * @param[in] pin - the pin needs to disable its pull-up register.
 * @return    none.
 */
void gpio_digital_pullup_dis(gpio_pin_e pin)
{
    unsigned char bit = pin & 0xff;
    if (((pin >> 8) & 0xff) == GPIO_GROUP_ANA) {
        analog_write_reg8(areg_gpio_pana_pu, (analog_read_reg8(areg_gpio_pana_pu)) & (~bit));
    } else {
        BM_CLR(reg_gpio_pu(pin), bit);
    }
}

/**
 * @brief      This function serves to set the TCK of DSP_JTAG function.
 * @param[in]  pin      - the pin needs to set.
 * @return     none.
 */
void gpio_set_dsp_jtag_tck_function(gpio_pin_e pin)
{
    gpio_input_en(pin);
    BM_CLR(reg_gpio_func(pin), pin & 0xff);
    reg_gpio_func_mux(pin) = DSP_TCK_I;
}

/**
 * @brief      This function serves to set the TMS of DSP_JTAG function.
 * @param[in]  pin      - the pin needs to set.
 * @return     none.
 */
void gpio_set_dsp_jtag_tms_function(gpio_pin_e pin)
{
    gpio_input_en(pin);
    BM_CLR(reg_gpio_func(pin), pin & 0xff);
    reg_gpio_func_mux(pin) = DSP_TMS_I;
}

/**
 * @brief      This function serves to set the TDO of DSP_JTAG function.
 * @param[in]  pin      - the pin needs to set.
 * @return     none.
 */
void gpio_set_dsp_jtag_tdo_function(gpio_pin_e pin)
{
    gpio_input_en(pin);
    BM_CLR(reg_gpio_func(pin), pin & 0xff);
    reg_gpio_func_mux(pin) = DSP_TDO_IO;
}

/**
 * @brief      This function serves to set the TDI of DSP_JTAG function.
 * @param[in]  pin      - the pin needs to set.
 * @return     none.
 */
void gpio_set_dsp_jtag_tdi_function(gpio_pin_e pin)
{
    gpio_input_en(pin);
    BM_CLR(reg_gpio_func(pin), pin & 0xff);
    reg_gpio_func_mux(pin) = DSP_TDI_I;
}

/**
 * @brief     This function is used to enable the JTAG function of the dsp.
 * @param[in] dsp_jtag_pin - the pin selected as the DSP_JTAG.
 * @return    none.
 */
void dsp_jtag_enable(dsp_jtag_pin_st *dsp_jtag_pin)
{
    gpio_set_dsp_jtag_tck_function(dsp_jtag_pin->tck);
    gpio_set_dsp_jtag_tms_function(dsp_jtag_pin->tms);
    gpio_set_dsp_jtag_tdo_function(dsp_jtag_pin->tdo);
    gpio_set_dsp_jtag_tdi_function(dsp_jtag_pin->tdi);
}

/**
 * @brief     This function set probe clk output.
 * @param[in] pin
 * @param[in] sel_clk
 * @return    none.
 */
void gpio_set_probe_clk_function(gpio_func_pin_e pin, probe_clk_sel_e sel_clk)
{
    reg_probe_clk_sel = (reg_probe_clk_sel & 0xe0) | sel_clk; //probe_clk_sel_e
    gpio_set_mux_function(pin, DBG_PROBE_CLK);                //sel probe_clk function
    gpio_function_dis((gpio_pin_e)pin);
}

/**
 * @brief      This function set the pin's driving strength at strong.
 * @param[in]  pin - the pin needs to set the driving strength
 * @return     none
 */
void gpio_ds_en(gpio_pin_e pin)
{
    unsigned char  bit   = pin & 0xff;
    if (((pin >> 8) & 0xff) == GPIO_GROUP_ANA) {
        analog_write_reg8(areg_gpio_pana_setting, (analog_read_reg8(areg_gpio_pana_setting)) | (bit << 4));
    } else {
        BM_SET(reg_gpio_ds(pin), bit);
    }
}

/**
  * @brief      This function set the pin's driving strength.
  * @param[in]  pin - the pin needs to set the driving strength at poor.
  * @return     none
  */
void gpio_ds_dis(gpio_pin_e pin)
{
    unsigned char  bit   = pin & 0xff;
    if (((pin >> 8) & 0xff) == GPIO_GROUP_ANA) {
        analog_write_reg8(areg_gpio_pana_setting, (analog_read_reg8(areg_gpio_pana_setting)) & (~(bit << 4)));
    } else {
        BM_CLR(reg_gpio_ds(pin), bit);
    }
}
/**********************************************************************************************************************
  *                                         local function implementation                                             *
  *********************************************************************************************************************/
