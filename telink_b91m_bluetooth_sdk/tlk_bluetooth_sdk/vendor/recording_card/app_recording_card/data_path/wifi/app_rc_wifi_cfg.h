/********************************************************************************************************
 * @file    app_rc_wifi_cfg.h
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
#pragma once

//power ctrl
#if (TLKHW_TYPE == TLKHW_TL751X_EVK_C1T368A87_V1_0 || TLKHW_TYPE == TLKHW_TL751X_EVK_C1T368A87_V1_2)
#define APP_WIFI_POWER_CTRL GPIO_PA3
#elif (TLKHW_TYPE == TLKHW_TL751X_EVK_C1T368A20_V1_0)
#define APP_WIFI_POWER_CTRL GPIO_PA3
//just for compile ok,not support wifi
#elif (TLKHW_TYPE == BOARD_721X_EVK_C1T315A87_V1_0)
#define APP_WIFI_POWER_CTRL GPIO_PA1
#else
#error "must set cfg"
#endif

//spi interface
#define APP_WIFI_SPI_ERR_RESEND_EN 1
#define APP_WIFI_SPI_ERR_SIMU      0
#define APP_WIFI_SPI_CLK_HZ        24000000
#if (TLKHW_TYPE == TLKHW_TL751X_EVK_C1T368A87_V1_0 || TLKHW_TYPE == TLKHW_TL751X_EVK_C1T368A87_V1_2)
#define APP_WIFI_SPI_MOSI_PIN      GPIO_FC_PB0
#define APP_WIFI_SPI_MISO_PIN      GPIO_FC_PB2
#define APP_WIFI_SPI_CSN_PIN       GPIO_FC_PA6
#define APP_WIFI_SPI_CLK_PIN       GPIO_FC_PA5
#define APP_WIFI_SPI_FLOW_CTRL_PIN GPIO_PB5
#elif (TLKHW_TYPE == TLKHW_TL751X_EVK_C1T368A20_V1_0)
#define APP_WIFI_SPI_MOSI_PIN      GPIO_FC_PB0
#define APP_WIFI_SPI_MISO_PIN      GPIO_FC_PB2
#define APP_WIFI_SPI_CSN_PIN       GPIO_FC_PA6
#define APP_WIFI_SPI_CLK_PIN       GPIO_FC_PA5
#define APP_WIFI_SPI_FLOW_CTRL_PIN GPIO_PB5
//just for compile ok,not support wifi
#elif (TLKHW_TYPE == BOARD_721X_EVK_C1T315A87_V1_0)
#define APP_WIFI_SPI_MOSI_PIN      LSPI_MOSI_IO0_PE2_PIN
#define APP_WIFI_SPI_MISO_PIN      LSPI_MISO_IO1_PE3_PIN
#define APP_WIFI_SPI_CSN_PIN       LSPI_CSN_PE6_PIN
#define APP_WIFI_SPI_CLK_PIN       LSPI_CLK_PE1_PIN
#define APP_WIFI_SPI_FLOW_CTRL_PIN GPIO_PA0
#else
#error "must set cfg"
#endif

//uart interface
#define APP_WIFI_UART_PORT           UART0
#define APP_WIFI_UART_BAUDRATE       115200
#define APP_WIFI_UART_RX_BUFFER_NUM  4
#define APP_WIFI_UART_RX_BUFFER_SIZE 256

#if (MCU_CORE_TYPE == MCU_CORE_TL721X)
#define APP_WIFI_UART_TX_PIN GPIO_PB0
#define APP_WIFI_UART_RX_PIN GPIO_PB1
#else
#define APP_WIFI_UART_TX_PIN GPIO_PA0
#define APP_WIFI_UART_RX_PIN GPIO_PA1
#endif

#define APP_WIFI_NAME_MAKE_BY_BLE_MAC 1
