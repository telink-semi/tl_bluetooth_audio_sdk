/********************************************************************************************************
 * @file    ext_uart.h
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
#ifndef DRIVERS_TL322X_EXT_DRIVER_EXT_UART_H_
#define DRIVERS_TL322X_EXT_DRIVER_EXT_UART_H_


#include "common/config/user_config.h"


/**
 * @brief   debug UART port definition, don not change this
 */
#define DBG_UART_PORT0                              0x01
#define DBG_UART_PORT1                              0x02
#define DBG_UART_PORT2                              0x03
#define DBG_UART_PORT3                              0x04
#define DBG_UART_PORT4                              0x05
/**
 * @brief   debug UART port selection, user can change in app_config.h
 */
#ifndef TLKAPI_DEBUG_UART_PORT
    #ifdef MCU_CORE_N22
        #define TLKAPI_DEBUG_UART_PORT                      DBG_UART_PORT4
    #else
        #define TLKAPI_DEBUG_UART_PORT                      DBG_UART_PORT2
    #endif
#endif


/**
 * @brief   define debug UART channel according to UART port selection. don not change
 */
#if (TLKAPI_DEBUG_UART_PORT   == DBG_UART_PORT0)
    #define DEBUG_UART_CHANNEL                      UART0
    #define DEBUG_UART_CHANNEL_IRQ                  IRQ_UART0
#elif (TLKAPI_DEBUG_UART_PORT == DBG_UART_PORT1)
    #define DEBUG_UART_CHANNEL                      UART1
    #define DEBUG_UART_CHANNEL_IRQ                  IRQ_UART1
#elif (TLKAPI_DEBUG_UART_PORT == DBG_UART_PORT2)
    #define DEBUG_UART_CHANNEL                      UART2
    #define DEBUG_UART_CHANNEL_IRQ                  IRQ_UART2
#elif (TLKAPI_DEBUG_UART_PORT == DBG_UART_PORT3)
    #define DEBUG_UART_CHANNEL                      UART3
    #define DEBUG_UART_CHANNEL_IRQ                  IRQ_IRQ_UART3
#elif (TLKAPI_DEBUG_UART_PORT == DBG_UART_PORT4)
    #define DEBUG_UART_CHANNEL                      UART4
    #define DEBUG_UART_CHANNEL_IRQ                  IRQ_UART4
#else
    #error "unsupported UART channel"
#endif


/**
 * @brief   debug UART TX DMA selection, user can change in app_config.h
 */
#ifndef TLKAPI_DEBUG_UART_TX_DMA
    #ifdef MCU_CORE_N22
        #define TLKAPI_DEBUG_UART_TX_DMA                    DMA15
    #else
        #define TLKAPI_DEBUG_UART_TX_DMA                    DMA14
    #endif
#endif


/**
 * @brief   debug UART TX pin selection, user can change in app_config.h
 */
#ifndef TLKAPI_DEBUG_UART_TX_PIN
    #ifdef MCU_CORE_N22
        #define TLKAPI_DEBUG_UART_TX_PIN                    GPIO_FC_PA3
    #else
        #define TLKAPI_DEBUG_UART_TX_PIN                    GPIO_FC_PE0
    #endif
#endif

/**
 * @brief   debug UART RX pin selection, user can change in app_config.h
 */
#ifndef TLKAPI_DEBUG_UART_RX_PIN
#define TLKAPI_DEBUG_UART_RX_PIN                    GPIO_FC_PC5
#endif


/**
 * @brief   debug UART baudrate definition, user can change in app_config.h
 */
#ifndef TLKAPI_DEBUG_UART_BAUDRATE
#define TLKAPI_DEBUG_UART_BAUDRATE                  2000000
#endif



/**
 * @brief   UART IRQ status check, do not change this
 */
#define UART_DEBUG_CHECK_TX_DONE_IRQ_STATUS     uart_get_irq_status(DEBUG_UART_CHANNEL,UART_TXDONE_IRQ_STATUS)
#define UART_DEBUG_CLEAR_TX_DONE                uart_clr_irq_status(DEBUG_UART_CHANNEL, UART_TXDONE_IRQ_STATUS)





/**
 * @brief       Initialize UART debug interface, configuration see TLKAPI_DEBUG_UART_xxx.
 * @return      none
 */
void uart_debug_init(void);

/**
 * @brief       Send UART debug data to output interface.
 * @param[in]   addr - FIFO data pointer.
 * @param[in]   len - FIFO data length.
 * @return      none
 */
void uart_debug_prepare_dma_data(unsigned char * addr, unsigned int len);

#endif /* DRIVERS_TL721X_EXT_DRIVER_EXT_UART_H_ */
