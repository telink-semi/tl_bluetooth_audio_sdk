/********************************************************************************************************
 * @file    tlkmw_fs_test.c
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
#define TLK_CFG_FS_TEST_CASE_EN 0

#if TLK_CFG_FS_TEST_CASE_EN && TLK_CFG_FS_ENABLE && TLK_DEV_SERIAL_ENABLE
#include "tlkapi/tlkapi_common.h"
#include "../drv/tlkmw_fs_diskio.h"
#include "../tlkmw_fs.h"
#include "tlkmw/tlkmw.h"
#include "stdlib.h"
#include "stdio.h"

#define TLKMW_FSTEST_SERIAL_PORT        UART1
#define TLKMW_FSTEST_SERIAL_TX_PIN      GPIO_PC6
#define TLKMW_FSTEST_SERIAL_RX_PIN      GPIO_PC7
#define TLKMW_FSTEST_SERIAL_TX_DMA      0
#define TLKMW_FSTEST_SERIAL_RX_DMA      DMA1
#define TLKMW_FSTEST_SERIAL_BAUDRATE    1000000
#define TLKMW_FSTEST_SERIAL_RXBUFF_NUM  16
#define TLKMW_FSTEST_SERIAL_RXBUFF_SIZE 512

__attribute__((aligned(4))) static uint8_t sTlkmwFsTestSerialRxBuffer[TLKMW_FSTEST_SERIAL_RXBUFF_NUM * (TLKMW_FSTEST_SERIAL_RXBUFF_SIZE + 4)];
static TlkOsMutexHandle_t                  sTlkmwTestFsUartMutex = NULL;
static TlkOsFileCtrlHandle_t               sTlkmwFsTestNowfile   = NULL;

static void tlkmw_fs_test_uart_write(void *pData, uint16_t dataLen)
{
    tlkos_recursiveMutex_lock(sTlkmwTestFsUartMutex);
    tlkdrv_serial_send(TLKMW_FSTEST_SERIAL_PORT, (uint8_t *)pData, dataLen);
    tlkos_recursiveMutex_unlock(sTlkmwTestFsUartMutex);
}

static bool tlkmw_fs_test_ack_cmd_create(uint8_t *pData, uint16_t dataLen)
{
    if (sTlkmwFsTestNowfile != NULL) {
        return false;
    }
    sTlkmwFsTestNowfile = tlkmw_fs_mallocFileCtrl();
    if (!sTlkmwFsTestNowfile) {
        return false;
    }
    char buffer[512];
    buffer[0] = '0';
    buffer[1] = ':';
    buffer[2] = '/';
    memcpy(buffer + 3, pData, dataLen);
    memcpy(buffer + 3 + dataLen, ".txt", 5);
    if (tlkmw_fs_createNewFile(sTlkmwFsTestNowfile, buffer) != TLK_ENONE) {
        tlkmw_fs_freeFileCtrl(sTlkmwFsTestNowfile);
        sTlkmwFsTestNowfile = NULL;
        return false;
    }
    return true;
}

static bool tlkmw_fs_test_ack_cmd_write(uint8_t *pData, uint16_t dataLen)
{
    if (sTlkmwFsTestNowfile == NULL) {
        return false;
    }
    return tlkmw_fs_write(sTlkmwFsTestNowfile, pData, dataLen) == TLK_ENONE;
}

static bool tlkmw_fs_test_ack_cmd_close(uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
    if (sTlkmwFsTestNowfile == NULL) {
        return false;
    }
    tlkmw_fs_close(sTlkmwFsTestNowfile);
    tlkmw_fs_freeFileCtrl(sTlkmwFsTestNowfile);
    sTlkmwFsTestNowfile = NULL;
    return true;
}

static bool tlkmw_fs_test_ack_cmd_open_read(uint8_t *pData, uint16_t dataLen)
{
    if (sTlkmwFsTestNowfile != NULL) {
        return false;
    }
    sTlkmwFsTestNowfile = tlkmw_fs_mallocFileCtrl();
    if (!sTlkmwFsTestNowfile) {
        return false;
    }
    char buffer[512];
    buffer[0] = '0';
    buffer[1] = ':';
    buffer[2] = '/';
    memcpy(buffer + 3, pData, dataLen);
    memcpy(buffer + 3 + dataLen, ".txt", 5);
    if (tlkmw_fs_openExist(sTlkmwFsTestNowfile, buffer) != TLK_ENONE) {
        tlkmw_fs_freeFileCtrl(sTlkmwFsTestNowfile);
        sTlkmwFsTestNowfile = NULL;
        return false;
    }
    return true;
}

static bool tlkmw_fs_test_ack_cmd_read(uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
    if (sTlkmwFsTestNowfile == NULL) {
        return false;
    }
    uint8_t  buffer[256];
    uint32_t len = 0;
    if (tlkmw_fs_read(sTlkmwFsTestNowfile, (uint8_t *)buffer, sizeof(buffer), &len) != TLK_ENONE) {
        return false;
    }
    char send_buf[512];
    int  res = snprintf(send_buf, sizeof(send_buf), "rsp:read ok %lu ", len);
    if (res < 0) {
        return false;
    }
    memcpy(send_buf + res, buffer, len);
    memcpy(send_buf + res + len, " \r\n", 3);
    tlkmw_fs_test_uart_write(send_buf, len + res + 3);
    return true;
}

static void tlkmw_fs_test_get_uart_data(uint8_t *pData, uint16_t dataLen)
{
    const char *create_cmd   = "cmd:create ";
    const char *write_cmd    = "cmd:write ";
    const char *close_cmd    = "cmd:close ";
    const char *openread_cmd = "cmd:openForRead ";
    const char *read_cmd     = "cmd:read ";
    if (strncmp((char *)pData, create_cmd, strlen(create_cmd)) == 0) {
        bool res = tlkmw_fs_test_ack_cmd_create(pData + strlen(create_cmd), dataLen - strlen(create_cmd));
        if (res) {
            tlkmw_fs_test_uart_write("rsp:create ok\r\n", strlen("rsp:create ok\r\n"));
        } else {
            tlkmw_fs_test_uart_write("rsp:create fail\r\n", strlen("rsp:create fail\r\n"));
        }
    } else if (strncmp((char *)pData, write_cmd, strlen(write_cmd)) == 0) {
        bool res = tlkmw_fs_test_ack_cmd_write(pData + strlen(write_cmd), dataLen - strlen(write_cmd));
        if (res) {
            tlkmw_fs_test_uart_write("rsp:write ok\r\n", strlen("rsp:write ok\r\n"));
        } else {
            tlkmw_fs_test_uart_write("rsp:write fail\r\n", strlen("rsp:write fail\r\n"));
        }
    } else if (strncmp((char *)pData, close_cmd, strlen(close_cmd)) == 0) {
        bool res = tlkmw_fs_test_ack_cmd_close(pData + strlen(close_cmd), dataLen - strlen(close_cmd));
        if (res) {
            tlkmw_fs_test_uart_write("rsp:close ok\r\n", strlen("rsp:close ok\r\n"));
        } else {
            tlkmw_fs_test_uart_write("rsp:close fail\r\n", strlen("rsp:close fail\r\n"));
        }
    } else if (strncmp((char *)pData, openread_cmd, strlen(openread_cmd)) == 0) {
        bool res = tlkmw_fs_test_ack_cmd_open_read(pData + strlen(openread_cmd), dataLen - strlen(openread_cmd));
        if (res) {
            tlkmw_fs_test_uart_write("rsp:openForRead ok\r\n", strlen("rsp:openForRead ok\r\n"));
        } else {
            tlkmw_fs_test_uart_write("rsp:openForRead fail\r\n", strlen("rsp:openForRead fail\r\n"));
        }
    } else if (strncmp((char *)pData, read_cmd, strlen(read_cmd)) == 0) {
        bool res = tlkmw_fs_test_ack_cmd_read(pData + strlen(read_cmd), dataLen - strlen(read_cmd));
        if (!res) {
            tlkmw_fs_test_uart_write("rsp:fail\r\n", strlen("rsp:fail\r\n"));
        }
    }
}

static void tlkmw_fs_test_uart_shell_init(void)
{
    tlkos_recursiveMutex_create(&sTlkmwTestFsUartMutex);
    tlkdrv_serial_mount(TLKMW_FSTEST_SERIAL_PORT, TLKMW_FSTEST_SERIAL_BAUDRATE, TLKMW_FSTEST_SERIAL_TX_PIN, TLKMW_FSTEST_SERIAL_RX_PIN, TLKMW_FSTEST_SERIAL_TX_DMA,
                        TLKMW_FSTEST_SERIAL_RX_DMA);

    tlkdrv_serial_setRxQFifo(TLKMW_FSTEST_SERIAL_PORT, TLKMW_FSTEST_SERIAL_RXBUFF_NUM, TLKMW_FSTEST_SERIAL_RXBUFF_SIZE + 4, sTlkmwFsTestSerialRxBuffer,
                             TLKMW_FSTEST_SERIAL_RXBUFF_NUM * (TLKMW_FSTEST_SERIAL_RXBUFF_SIZE + 4));

    tlkdrv_serial_regCB(TLKMW_FSTEST_SERIAL_PORT, tlkmw_fs_test_get_uart_data);
    tlkdrv_serial_open(TLKMW_FSTEST_SERIAL_PORT);
    tlkmw_fs_test_uart_write("evt:init ok\r\n", strlen("evt:init ok\r\n"));
}

void tlkmw_fs_test_send_prog_evt(uint32_t page)
{
    char send_buf[512];
    int  res = snprintf(send_buf, sizeof(send_buf), "evt:prog %lu\r\n", page);
    if (res < 0) {
        return;
    }
    tlkmw_fs_test_uart_write(send_buf, res);
}

void tlkmw_fs_test_send_erase_evt(uint32_t block)
{
    char send_buf[512];
    int  res = snprintf(send_buf, sizeof(send_buf), "evt:erase %lu\r\n", block);
    if (res < 0) {
        return;
    }
    tlkmw_fs_test_uart_write(send_buf, res);
}

void tlkmw_fs_test_task(void *arg)
{
    (void)arg;
    tlkos_task_delayMs(1000);
    tlkmw_fs_test_uart_shell_init();
    while (1) {
        tlkdrv_serial_handler(UART0);
        tlkdrv_serial_handler(UART1);
    }
}

void tlkapp_sys_taskInitCompletedHook(void)
{
    tlkos_task_create(tlkmw_fs_test_task, "tlkmw_fs_test_task", 10 * 1024, 1, NULL, NULL, NULL);
}


#endif
