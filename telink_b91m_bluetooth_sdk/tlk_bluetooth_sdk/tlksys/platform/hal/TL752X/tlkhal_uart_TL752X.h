/********************************************************************************************************
 * @file    tlkhal_uart_TL752X.h
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
#ifndef TLKHAL_UART_TL752X_H_
#define TLKHAL_UART_TL752X_H_

typedef void (*tlkhal_uart_dmaRxDoneCB) (unsigned char port, unsigned char param);
typedef void (*tlkhal_uart_dmaTxDoneCB) (unsigned char port);
typedef void (*tlkhal_uart_noDmaRxDoneCB) (unsigned char port);

void tlkhal_uart_dmaRxDoneRegister(tlkhal_uart_dmaRxDoneCB cb);
void tlkhal_uart_dmaTxDoneRegister(tlkhal_uart_dmaTxDoneCB cb);
void tlkhal_uart_noDmaRxDoneRegister(tlkhal_uart_noDmaRxDoneCB cb);
uint8_t tlkhal_uart_getUartPort(uart_handle_t *huart);

#endif /* TLKHAL_UART_TL752X_H_ */
