/********************************************************************************************************
 * @file    tlkdrv_sy5500_uart.h
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

/**
 * @brief SY5500 UART command enumeration definition
 */
typedef enum
{
    /** Heartbeat packet command */
    TLKDRV_SY5500_UART_CMD_HEART_BEAT = 0x5ABC,

    /** Open box command */
    TLKDRV_SY5500_UART_CMD_OPEN_BOX = 0x5555,

    /** Close box command */
    TLKDRV_SY5500_UART_CMD_CLOSE_BOX = 0x5565,

    /** 3-second button command */
    TLKDRV_SY5500_UART_CMD_3S_PRESS = 0X5585,

    /** 10-second button command */
    TLKDRV_SY5500_UART_CMD_10S_PRESS = 0X55C9,

    /** Get MAC address command */
    TLKDRV_SY5500_UART_CMD_GET_MAC = 0X55C0,
} TLKDRV_SY5500_UART_CMD_ENUM;

/**
 * @brief Initialize SY5500 UART interface
 * @param[in] recCB Receive callback function
 */
void tlkdrv_sy5500_uart_init(TlkDrvSerialRecvCB recCB);

/**
 * @brief Send data to SY5500 via UART
 * @param[in] pData Pointer to data to be sent
 * @param[in] dataLen Length of data to be sent
 */
void tlkdrv_sy5500_sendUartData(uint8_t *pData, uint16_t dataLen);