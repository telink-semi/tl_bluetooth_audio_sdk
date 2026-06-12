/********************************************************************************************************
 * @file    app.c
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
#include "tl_common.h"
#include "drivers.h"

#include "tlklib/usb/udb/tlkusb_udb.h"
#include "tlklib/dbg/tlkdbg.h"
#include "tlklib/usb/tlkusb.h"
#include "stack/controller.h"

#include "tlkmw/ble/le_controller/tlkmdi_ble_ctrl.h"
#include "bluetooth_controller.h"
#include "app_config.h"
#if (BLUETOOTH_CONTROLLER_752X_TEMP && MCU_CORE_TYPE == MCU_CORE_TL752X)
#include "drv_cpr.h"
//#include "drv_gpio.h"
#include "drv_uart.h"
#include <stdio.h>
#include <string.h>
//#include "drv_pmu_ex.h"
#include "stack/multiCoreComm/service/service_shareMemory.h"
#include "stack/bt/host/bth/bth_hcicmd.h"
/*************************** macro definition ********************************/
#if (BT_BQB_D25F_TEST == 1)

void bqb_scan_test(void)
{
    char *local_name = "A_TLK_IUT0";
    bth_hci_sendWriteLocalNameCmd((uint8_t *)local_name);
    bth_hci_sendWriteClassOfDeviceCmd(0x2C0404);
    extern int bth_hci_sendEnDUTModeCmd();
    bth_hci_sendEnDUTModeCmd();
    bth_hci_sendWriteScanEnableCmd(3);
}

/**
 * @brief      Hook function called when system task initialization is completed
 * @param[in]  none
 * @return     none
 * @note       Creates a task for BQB scan test
 */
void tlkapp_sys_taskInitCompletedHook(void)
{
    tlkos_task_create((TlkOsTaskEnterCB)bqb_scan_test, "bqb_scan_test", 4096, 1, NULL, NULL, NULL);
}
#elif (BT_EMI_D25F_TEST == 1)


#else
#define MAX_UART_RX_BUF (100)
#define MAX_UART_TX_BUF (100)

uint8_t uart_tx_buf[MAX_UART_TX_BUF];
uint8_t uart_rx_buf[MAX_UART_RX_BUF];

uart_handle_t uart_handle;

volatile unsigned int g_uart_rec_len                     = 0;
char                  uart_tx_start_buf[MAX_UART_TX_BUF] = "uart tx test\n";

dmac_handle_t uart_dmac_tx;

volatile int uart_dma_tx_finish_flag = 0;
volatile int uart_it_rx_finish_flag  = 0;

_attribute_ram_code_sec_ void tlk_dma_irq_handler(void)
{
    drv_dmac_irq_handle(&uart_dmac_tx);
}

_attribute_ram_code_sec_ void tlk_uart1_irq_handler(void)
{
    drv_uart_irq_handler(&uart_handle);
}

/**
 * @brief      UART DMA transmit callback function
 * @param[in]  huart - pointer to UART handle
 * @return     none
 */
void uart_dma_tx_cb(uart_handle_t *huart)
{
    uart_dma_tx_finish_flag = 1;
}

/**
 * @brief      UART interrupt receive callback function
 * @param[in]  huart - pointer to UART handle
 * @return     none
 */
void uart_it_rx_cb(uart_handle_t *huart)
{
    tlkapi_printf(APP_LOG_EN, "[APP][INI] uart init");
    if (huart->rx_callback_arg->type == UART_RX_CALLBACK_COMPLETE) {
        uart_it_rx_finish_flag = 1;
        g_uart_rec_len         = huart->rx_callback_arg->rx_data_len;
    }
    huart->rx_callback_arg->type = 0;
}

/**
 * @brief      Initialize UART communication
 * @param[in]  none
 * @return     none
 */
void uart_init(void)
{
    uart_set_pin(GPIO_PORT_A, GPIO_PIN_0, GPIO_PIN_1, GPIO_ALTERNATE_FUNC_2);

    uart_handle.instance       = UART1;
    uart_handle.init.baudrate  = 3000000;
    uart_handle.init.hwflowctl = UART_HWCONTROL_NONE;
    uart_handle.init.parity    = UART_PARITY_NONE;
    uart_handle.init.stopbits  = UART_STOPBITS_1;

    hal_uart_init(DRV_CPR_UART_MCLK_SRC_DBLR, &uart_handle);

    drv_uart_register_callback(&uart_handle, DRV_UART_IT_RX_CB_ID, uart_it_rx_cb);
    drv_uart_register_callback(&uart_handle, DRV_UART_DMA_TX_CB_ID, uart_dma_tx_cb);

    memset(&uart_dmac_tx, 0, sizeof(uart_dmac_tx));
    uart_dmac_tx.init.channel = CPU_DMAC_CH1;
    uart_set_tx_dma_config(&uart_dmac_tx, &uart_handle);

    drv_hw_int_enable(CPU_DMA_IRQn);
    drv_hw_int_enable(UART1_IRQn);

    drv_uart_transmit(&uart_handle, uart_tx_start_buf, strlen(uart_tx_start_buf), DRV_MAX_DELAY);
    drv_uart_transmitter_empty_waiting(&uart_handle, DRV_MAX_DELAY);
}

/**
 * @brief      Test UART receive functionality
 * @param[in]  none
 * @return     DRV_SUCCESS on success
 */
drv_status_t uart_rx_test(void)
{
    memset(uart_rx_buf, 0, MAX_UART_RX_BUF);
    tlkapi_printf(1, "uart_rx_test");

    uart_it_rx_finish_flag = 0;
    drv_uart_receive_it(&uart_handle, uart_rx_buf, MAX_UART_RX_BUF);

    while (uart_it_rx_finish_flag == 0) {
        __NOP();
    }
#if TLK_STK_MODE == BLE_MODE
    ble_host_sal_hci_send_packet(uart_rx_buf, g_uart_rec_len);
#else
    while (tlk_d25f_hci_send_message(TLK_SHARE_MEMORY_MESSAGE_TYPE_BT, (u8 *)uart_rx_buf, g_uart_rec_len) != 0);
#endif
}

/**
 * @brief      Test UART transmit functionality
 * @param[in]  data - pointer to data to transmit
 * @param[in]  len  - length of data to transmit
 * @return     DRV_SUCCESS on success
 */
drv_status_t uart_tx_test(uint8_t *data, uint32_t len)
{
    uart_dma_tx_finish_flag = 0;
    tlkapi_printf(1, "uart_tx_test");
    drv_uart_transmit_dma(&uart_handle, (uint8_t *)((uint32_t)data + CPU_SYS_BUS), len);
    while (uart_dma_tx_finish_flag == 0) {
        __NOP();
    }
}

/**
 * @brief      UART receive thread function
 * @param[in]  pUsrArg - user argument (unused)
 * @return     none
 */
static void tlk_hci_rx_thread(void *pUsrArg)
{
    while (1) {
        uart_rx_test();
    }
}

/**
 * @brief      Hook function called when system task initialization is completed
 * @param[in]  none
 * @return     none
 * @note       Initializes UART and creates UART receive thread
 */
void tlkapp_sys_taskInitCompletedHook(void)
{
    tlkapi_printf(APP_LOG_EN, "[APP][INI] uart init");
    uart_init();
    tlkos_task_create(tlk_hci_rx_thread, "tlk_hci_tx_thread", 4096, 1, NULL, NULL, NULL);
#if TLK_STK_MODE == BLE_MODE
    tlk_d25f_register_hci_receive_cb(TLK_SHARE_MEMORY_MESSAGE_TYPE_BLE, uart_tx_test);
#else
    tlk_d25f_register_hci_receive_cb(TLK_SHARE_MEMORY_MESSAGE_TYPE_BT, uart_tx_test);
#endif
}
#endif

#else

HCI_TR_UART hci_tr_uart = {.baudrate = HCI_TR_BAUDRATE, .tx_Pin = HCI_TR_TX_PIN, .rx_Pin = HCI_TR_RX_PIN, .HwFlowCtl = 0, .cts_Pin = 0, .rts_Pin = 0};

/////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief     User initialization when MCU power on or wake_up from deepSleep mode
 * @param[in] None.
 * @returns   None.
 */
static void tlk_controller_init(void)
{
#if (MCU_CORE_TYPE != MCU_CORE_TL751X) /*bt controller initial*/
    rf_module_init();

    trng_init();
    /* initialize some basic MCU hardware */
    tlk_sys_init_mcu_hardware();

#if BLUETOOTH_CONTROLLER_MODE == BT_BLE_CONTROLLER_MODE
    controller_init(BT_BLE, HCI_TR_H4, &hci_tr_uart, NULL);
#elif BLUETOOTH_CONTROLLER_MODE == BLE_CONTROLLER_ONLY_MODE
    controller_init(BLE_only, HCI_TR_H4, &hci_tr_uart, NULL);
#else
    controller_init(BT_only, HCI_TR_H4, &hci_tr_uart, NULL);
#endif
    //////////////////////////// basic hardware Initialization  End /////////////////////////////////

    tlk_sch_init();

    tlk_sch_plan_set_base_interval(PLAN_INTERVAL_10MS);

    tlkapi_printf(APP_LOG_EN, "[APP][INI] bluetooth controller init");
#endif
}

/**
 * @brief      Hook function called when system initialization is finished
 * @param[in]  none
 * @return     none
 * @note       Initializes the Bluetooth controller
 */
void tlksys_initFinishedHook(void)
{
    tlk_controller_init();
}
#endif
