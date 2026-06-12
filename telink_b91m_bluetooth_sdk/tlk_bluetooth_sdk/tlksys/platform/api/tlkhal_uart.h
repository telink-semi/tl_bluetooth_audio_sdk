/********************************************************************************************************
 * @file    tlkhal_uart.h
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

typedef struct
{
    uint8_t  port;
    uint8_t  txDma; //0 means no use
    uint8_t  rxDma; //0 means no use
    uint8_t  futureUse;
    uint16_t txPin; //0 means no use
    uint16_t rxPin; //0 means no use
    uint32_t baudRate;
#if (MCU_CORE_TL752X_TEMP)
    uart_handle_t *pUartHandle;
    dmac_handle_t *pRxDmaHandle;
    dmac_handle_t *pTxDmaHandle;
#endif
} tlkhal_uart_cfg_t;

/**
 * @brief  Open and configure UART
 * @param[in] pCfg : Pointer to UART configuration structure
 * @returns  None.
 */
void tlkhal_uart_open(const tlkhal_uart_cfg_t *const pCfg);

/**
 * @brief  Close UART
 * @param[in] pCfg : Pointer to UART configuration structure
 * @returns  None.
 */
void tlkhal_uart_close(const tlkhal_uart_cfg_t *const pCfg);

/**
 * @brief  Clear UART TX done status
 * @param[in] port : UART port number
 * @param[in] futureUse : Reserved for future use
 * @returns  None.
 */
void tlkhal_uart_clrTxDoneStatus(uint8_t port, void *futureUse);

/**
 * @brief  Get UART TX done status
 * @param[in] port : UART port number
 * @param[in] futureUse : Reserved for future use
 * @returns  TX done status.
 */
uint32_t tlkhal_uart_getTxDoneStatus(uint8_t port, void *futureUse);

/**
 * @brief  Clear UART RX done status
 * @param[in] port : UART port number
 * @param[in] futureUse : Reserved for future use
 * @returns  None.
 */
void tlkhal_uart_clrRxDoneStatus(uint8_t port, void *futureUse);

/**
 * @brief  Get UART RX done status
 * @param[in] port : UART port number
 * @param[in] futureUse : Reserved for future use
 * @returns  RX done status.
 */
uint32_t tlkhal_uart_getRxDoneStatus(uint8_t port, void *futureUse);
