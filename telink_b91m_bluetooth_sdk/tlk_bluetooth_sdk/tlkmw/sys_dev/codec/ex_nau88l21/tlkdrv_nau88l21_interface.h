/********************************************************************************************************
 * @file    tlkdrv_nau88l21_interface.h
 *
 * @brief   This is the header file for TLSR/TL
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
#ifndef NAU8821_INTERFACE_H_
#define NAU8821_INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "../tlkdrv_i2s_ex_codec_config.h"
#if (I2S_CODEC_SEL == I2S_CODEC_CHIP_NAU88L21)
#include "../tlkdrv_i2s_master_tl721x.h"

#include "tl_common.h"
#include "driver.h"

#if defined(MCU_CORE_TL721X)

#define I2C_CLK_SPEED    400000 //i2c clock 400K.

#define I2C_GPIO_SDA_PIN GPIO_FC_PB7
#define I2C_GPIO_SCL_PIN GPIO_FC_PC0

    typedef struct
    {
        uint8_t vref_impedance;
        uint8_t micbias_voltage;
    } tlkdrv_nau8821_config_t;

    extern tlkdrv_nau8821_config_t  nau8821_sys_clk_config;
    extern i2s_pin_config_t         hal_nau8821_i2s_pin_config;
    extern audio_i2s_config_t       hal_nau8821_i2s_config;
    extern audio_i2s_input_output_t hal_nau8821_i2s_input;
    extern audio_i2s_input_output_t hal_nau8821_i2s_output;

    extern void    hal_nau8821_i2c_init(void);
    extern void    hal_nau8821_i2c_write(uint8_t addr, uint16_t reg, uint16_t value);
    extern uint8_t hal_nau8821_i2c_read(uint8_t addr, uint16_t reg, uint16_t *data, uint8_t len);
    extern uint8_t hal_nau8821_i2c_update_bits(uint8_t addr, uint16_t reg, uint16_t mask, uint16_t value);

    extern void hal_nau8821_i2s_init(unsigned int *mic_buff, unsigned int *spk_buff, unsigned int mic_buff_size, unsigned int spk_buff_size);

    //#define regmap_write               hal_nau8821_i2c_write
    //#define regmap_read                hal_nau8821_i2c_read
    //#define regmap_update_bits         hal_nau8821_i2c_update_bits
    //#define ussleep(ms)                delay_ms(ms) //vTaskDelay(pdMS_TO_TICKS(ms));

#endif ///#if (TLKHW_TYPE == BOARD_721X_EVK_C1T315A20_V2)

#ifdef __cplusplus
}
#endif

#endif
#endif /* TLKMW_SYS_DEV_CODEC_TLKDRV_nau8821_INTERFACE_H_ */
