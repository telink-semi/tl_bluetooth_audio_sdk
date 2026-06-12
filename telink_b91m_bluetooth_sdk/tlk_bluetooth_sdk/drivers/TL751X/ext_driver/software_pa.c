/********************************************************************************************************
 * @file    software_pa.c
 *
 * @brief   This is the source file for TLSR/TL
 *
 * @author  Bluetooth Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "compiler.h"
#include "software_pa.h"
#include "../gpio.h"
//#include "driver_internal/ext_lib_internal.h"
#include "drivers.h"

_attribute_data_retention_sec_ rf_pa_callback_t blc_rf_pa_cb = 0;

#if (!BT_BASEBAND_SIGNAL_LOOPBACK_VCD)
gpio_pin_e gpio_pa_tx, gpio_pa_rx;

_attribute_ram_code_ void app_rf_pa_handler(int type)
{
#if (PA_ENABLE)
    if (type == PA_TYPE_TX_ON) {
        gpio_set_low_level(PA_RXEN_PIN);
        gpio_set_high_level(PA_TXEN_PIN);
    } else if (type == PA_TYPE_RX_ON) {
        gpio_set_low_level(PA_TXEN_PIN);
        gpio_set_high_level(PA_RXEN_PIN);
    } else {
        gpio_set_low_level(PA_RXEN_PIN);
        gpio_set_low_level(PA_TXEN_PIN);
    }
#else
    (void)type;
#endif
}

_attribute_bt_retention_code_ void pa_tx_irq0_handler(void)
{
    if (gpio_get_level(PA_TXEN_PIN)) {
        gpio_set_level(gpio_pa_tx, 1);
        gpio_set_level(gpio_pa_rx, 0);
        BM_SET(reg_gpio_pol(PA_TXEN_PIN), PA_TXEN_PIN & 0xff);
    } else {
        BM_CLR(reg_gpio_pol(PA_TXEN_PIN), PA_TXEN_PIN & 0xff);
    }
    gpio_clr_irq_status(GPIO_IRQ_IRQ6);
}
#if MCU_CORE_N22
CLIC_ISR_REGISTER(pa_tx_irq0_handler, IRQ_GPIO_SRC6)
#endif
_attribute_bt_retention_code_ void pa_rx_irq1_handler(void)
{
    if (gpio_get_level(PA_RXEN_PIN)) {
        gpio_set_level(gpio_pa_tx, 0);
        gpio_set_level(gpio_pa_rx, 1);
        BM_SET(reg_gpio_pol(PA_RXEN_PIN), PA_RXEN_PIN & 0xff);
    } else {
        BM_CLR(reg_gpio_pol(PA_RXEN_PIN), PA_RXEN_PIN & 0xff);
    }
    gpio_clr_irq_status(GPIO_IRQ_IRQ7);
}
#if MCU_CORE_N22
CLIC_ISR_REGISTER(pa_rx_irq1_handler, IRQ_GPIO_SRC7)
#endif
/**
 * @brief	RF software PA initialization
 * @param	none
 * @return	none
 */
void rf_pa_init(gpio_pin_e tx_en, gpio_pin_e rx_en)
{
    gpio_pa_tx = tx_en;
    gpio_pa_rx = rx_en;
    gpio_function_en(tx_en);
    gpio_set_output_en(tx_en, 1);
    gpio_set_level(tx_en, 0);

    gpio_function_en(rx_en);
    gpio_set_output_en(rx_en, 1);
    gpio_set_level(rx_en, 0);

    gpio_set_output_en(PA_TXEN_PIN, 1);
    gpio_set_input_en(PA_TXEN_PIN, 1);
#if MCU_CORE_N22
    gpio_irq_en(PA_TXEN_PIN, GPIO_IRQ6);

    gpio_set_irq(GPIO_IRQ6, PA_TXEN_PIN, INTR_RISING_EDGE);
    gpio_set_irq_mask(GPIO_IRQ_IRQ6);
    clic_set_priority(GPIO_IRQ_IRQ6, 1);
    clic_interrupt_enable(IRQ_GPIO_SRC6);
#else
//    gpio_irq_en(PA_TXEN_PIN, GPIO_IRQ0);
//
//    gpio_set_irq(GPIO_IRQ0, PA_TXEN_PIN, INTR_RISING_EDGE);
//    plic_set_priority(GPIO_IRQ_IRQ0, 1);
//    plic_interrupt_enable(IRQ_GPIO_IRQ0);
#endif

    gpio_set_output_en(PA_RXEN_PIN, 1);
    gpio_set_input_en(PA_RXEN_PIN, 1);
#if MCU_CORE_N22
    gpio_irq_en(PA_RXEN_PIN, GPIO_IRQ7);

    gpio_set_irq(GPIO_IRQ7, PA_RXEN_PIN, INTR_RISING_EDGE);
    gpio_set_irq_mask(GPIO_IRQ_IRQ7);
    clic_set_priority(GPIO_IRQ_IRQ7, 1);
    clic_interrupt_enable(IRQ_GPIO_SRC7);
#else
//    gpio_irq_en(PA_RXEN_PIN, GPIO_IRQ1);
//
//    gpio_set_irq(GPIO_IRQ1, PA_RXEN_PIN, INTR_RISING_EDGE);
//    plic_set_priority(GPIO_IRQ_IRQ1, 1);
//    plic_interrupt_enable(IRQ_GPIO_IRQ1);
#endif
    //Modem set
    void sub_wr(unsigned int addr, unsigned char value, unsigned char e, unsigned char s);
    sub_wr(CSEMDIGADDR + 0x012, 0x10, 5, 0); //dig_gpios_gpio_5_conf for tx_status
    sub_wr(CSEMDIGADDR + 0x013, 0x11, 5, 0); //dig_gpios_gpio_6_conf for rx_status

    //set pc[2]/pc[3]
    write_reg8(GPIO_BASE_ADDR + 0xc2, 0x53); //pc2_fs
    write_reg8(GPIO_BASE_ADDR + 0xc3, 0x53); //pc3_fs

    sub_wr(GPIO_BASE_ADDR + 0x26, 0, 3, 2); //pc[3:2]
}
#endif
