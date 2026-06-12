/********************************************************************************************************
 * @file    software_pa.h
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
#ifndef BLT_PA_H_
#define BLT_PA_H_

#include "../gpio.h"


/**
 * @brief	PA control type
 */
#define PA_TYPE_OFF   0
#define PA_TYPE_TX_ON 1
#define PA_TYPE_RX_ON 2

/**
 * @brief	GPIO to control PA TX EN, user should set it in app_config.h
 */
#ifndef PA_TXEN_PIN
#define PA_TXEN_PIN GPIO_PC2
#endif

/**
 * @brief	GPIO to control PA RX EN, user should set it in app_config.h
 */
#ifndef PA_RXEN_PIN
#define PA_RXEN_PIN GPIO_PC3
#endif
/**
 * @brief	software PA control Callback
 */
typedef void (*rf_pa_callback_t)(int type);
extern rf_pa_callback_t blc_rf_pa_cb;

_attribute_bt_retention_code_ void pa_tx_irq0_handler(void);
_attribute_bt_retention_code_ void pa_rx_irq1_handler(void);

/**
 * @brief	RF software PA initialization
 * @param	tx_en,rx_en - set pin to control PA,except PC2,PC3
 * @return	none
 */
void rf_pa_init(gpio_pin_e tx_en, gpio_pin_e rx_en);


#endif /* BLT_PA_H_ */
