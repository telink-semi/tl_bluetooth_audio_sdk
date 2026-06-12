/********************************************************************************************************
 * @file    app_emi_bqb.c
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
#include "tlkapi/tlkapi.h"
#include "stack/bt/host/bth/bth_stdio.h"
#include "tlkmw/tlkmw.h"
#include "tlkmw/sys_dev/serial/tlkdrv_serial.h"
#include "./emi/emi_general.h"

#define BT_RFTEST_BQB    2

#define BT_RFTEST_EMI    3

#define BT_INTERNAL_MODE BT_RFTEST_EMI

#if (TLK_DEV_SERIAL_ENABLE)


#define APP_COMM_SERIAL_BAUDRATE   115200 // 1000000//921600//115200
#define APP_COMM_SERIAL_RBUFF_NUMB 4
#define APP_COMM_SERIAL_RBUFF_SIZE 64
#define APP_COMM_SERIAL_SBUFF_NUMB 4
#define APP_COMM_SERIAL_SBUFF_SIZE 64

__attribute__((aligned(4))) static uint8_t sAPPCommRecvBuffer[APP_COMM_SERIAL_RBUFF_NUMB * (APP_COMM_SERIAL_RBUFF_SIZE + 4)];
__attribute__((aligned(4))) static uint8_t sAPPCommSendBuffer[APP_COMM_SERIAL_SBUFF_NUMB * (APP_COMM_SERIAL_SBUFF_SIZE + 4)];

void app_emi_exit(void)
{
    tlkmdi_tinySql_setWorkMode(TLK_WORK_MODE_NORMAL);
    core_interrupt_disable();
    tlkmdi_tinySql_save();
    sys_reboot();
}

void app_serial_send(u8 *data, uint16_t len)
{
    tlkdrv_serial_send(UART0, data, len);
}

void app_serial_recv(u8 *data, uint16_t len)
{
    tlkdrv_serial_send(UART0, data, len);
    if (data[0] != 0x11) {
        return;
    }
    if (data[1] == 0x01) //EMI
    {
        switch (data[2]) {
        case 0x00:
        {
            app_emi_exit();
        } break;
        case 0x01:
        {
            write_sram8(RUN_STATUE_ADDR, 1);
        } break;
        case 0x02:
        {
            write_sram8(CHANNEL_ADDR, data[3]);
            write_sram8(RUN_STATUE_ADDR, 0);
        } break;
        case 0x03:
        {
            write_sram8(POWER_ADDR, data[3]);
            write_sram8(RUN_STATUE_ADDR, 0);
        } break;
        default:
            break;
        }
    }

    if (data[1] == 0x02) //BQB
    {
        switch (data[2]) {
        case 0x00:
        {
            app_emi_exit();
        } break;
        default:
            break;
        }
    }
}

void app_serial_init(void)
{
    //    tlkdrv_serial_close(UART1);
    //    tlkdrv_serial_unmount(UART1);
    //    tlkdrv_serial_close(UART0);
    //    tlkdrv_serial_unmount(UART0);
    //    tlkdrv_serial_mount(UART1, APP_COMM_SERIAL_BAUDRATE, TLKDEV_SERIAL1_TX_PIN, TLKDEV_SERIAL1_RX_PIN, 0, 0);
    //    tlkdrv_serial_setTxFifo(UART1, sAPPCommSendBuffer, 128);
    //    tlkdrv_serial_setRxFifo(UART1, sAPPCommRecvBuffer, 128);
    //    tlkdrv_serial_regCB(UART1, app_serial_recv);
    //    tlkdrv_serial_open(UART1);
    //
    tlkdrv_serial_mount(UART0, APP_COMM_SERIAL_BAUDRATE, TLKDEV_SERIAL0_TX_PIN, TLKDEV_SERIAL0_RX_PIN, tlkhal_dma_malloc_ex(1), tlkhal_dma_malloc_ex(1));

    tlkdrv_serial_setTxQFifo(UART0, APP_COMM_SERIAL_SBUFF_NUMB, APP_COMM_SERIAL_SBUFF_SIZE + 4, sAPPCommSendBuffer, APP_COMM_SERIAL_SBUFF_NUMB * (APP_COMM_SERIAL_SBUFF_SIZE + 4));

    tlkdrv_serial_setRxQFifo(UART0, APP_COMM_SERIAL_RBUFF_NUMB, APP_COMM_SERIAL_RBUFF_SIZE + 4, sAPPCommRecvBuffer, APP_COMM_SERIAL_RBUFF_NUMB * (APP_COMM_SERIAL_RBUFF_SIZE + 4));


    tlkdrv_serial_regCB(UART0, app_serial_recv);
    tlkdrv_serial_open(UART0);
}
#endif

u32 start_tick = 0;

static void app_emi_init(void)
{
    sys_n22_clk_dis();
    sleep_ms(100);
    sys_n22_clk_en();
    emi_start();
#if (TLK_DEV_SERIAL_ENABLE)
    app_serial_init();
#endif

    start_tick = stimer_get_tick();
}

static void app_emi_main(void)
{
    emi_loop_server();

#if (TLK_DEV_SERIAL_ENABLE)
    tlkdrv_serial_handler(UART0);
#endif
}

static void app_bqb_init(void)
{
#if (TLK_DEV_SERIAL_ENABLE)
    app_serial_init();
#endif
    char *local_name = "A_TLK_IUT0";
    bth_hci_sendWriteLocalNameCmd((uint8_t *)local_name);
    bth_hci_sendWriteClassOfDeviceCmd(0x2C0404);
    bth_hci_sendEnDUTModeCmd();
    bth_hci_sendWriteScanEnableCmd(3);
}

static void app_bqb_main(void)
{
#if (TLK_DEV_SERIAL_ENABLE)
    tlkdrv_serial_handler(UART0);
#endif
}

void tlk_test_mode_hook(uint8_t mode)
{
    if (mode == TLK_WORK_MODE_TEST_EMI) {
        app_emi_init();
        while (1) {
            app_emi_main();
        }
    } else if (mode == TLK_WORK_MODE_TEST_BQB) {
        app_bqb_init();
        while (1) {
            app_bqb_main();
        }
    }
}
