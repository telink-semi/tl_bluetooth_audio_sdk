/********************************************************************************************************
 * @file    tlkdrv_sy5500_cfg.h
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

/*
note:this headfile is private,only can be included in sy5500 .c file.
*/

/**
 * @brief SY5500 serial port configuration
 */
#define TLKDEV_SY5500_SERIAL_PORT        UART0                     /*!< Serial port number */
#define TLKDEV_SY5500_SERIAL_RTX_PIN     TLKDEV_SERIAL0_RX_PIN     /*!< Serial RTX pin */
#define TLKDEV_SY5500_SERIAL_TX_DMA      (tlkhal_dma_malloc_ex(1)) /*!< Serial transmit DMA channel */
#define TLKDEV_SY5500_SERIAL_RX_DMA      (tlkhal_dma_malloc_ex(1)) /*!< Serial receive DMA channel */
#define TLKDEV_SY5500_SERIAL_BAUDRATE    9600                      /*!< Serial baud rate */
#define TLKDEV_SY5500_SERIAL_RXBUFF_NUM  4                         /*!< Serial receive buffer count */
#define TLKDEV_SY5500_SERIAL_RXBUFF_SIZE 256                       /*!< Serial receive buffer size */
#define TLKDEV_SY5500_SERIAL_TXBUFF_NUM  8                         /*!< Serial transmit buffer count */
#define TLKDEV_SY5500_SERIAL_TXBUFF_SIZE 32                        /*!< Serial transmit buffer size */

/**
 * @brief SY5500 I2C configuration
 */
#define TLKDEV_SY5500_I2C_SPEED            400000              /*!< I2C speed, 400KHz */
#define TLKDEV_SY5500_I2C_PIN_SDA          TLKDRV_I2C_GPIO_SDA /*!< I2C data pin */
#define TLKDEV_SY5500_I2C_PIN_SCL          TLKDRV_I2C_GPIO_SCL /*!< I2C clock pin */
#define TLKDEV_SY5500_I2C_PIN_IRQ          GPIO_PB5            /*!< I2C interrupt request pin */

#define TLKDEV_SY5500_I2C_TX_DMA_CHN       (tlkhal_dma_malloc())                 /*!< I2C transmit DMA channel */
#define TLKDEV_SY5500_I2C_RX_DMA_CHN       (tlkhal_dma_malloc())                 /*!< I2C receive DMA channel */
#define TLKDEV_SY5500_I2C_RX_DMA_STATE_BIT (1 << (TLKDEV_SY5500_I2C_RX_DMA_CHN)) /*!< I2C receive DMA status bit */

/**
 * @brief Debug log enable configuration
 */
#define TLKDEV_SY5500_DEBUG_LOG_EN 0 /*!< Debug log enable flag */