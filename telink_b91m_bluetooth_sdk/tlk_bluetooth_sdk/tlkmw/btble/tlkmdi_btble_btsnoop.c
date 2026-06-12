/********************************************************************************************************
 * @file    tlkmdi_btble_btsnoop.c
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
#include <string.h>
#include "tl_common.h"
#include "common/types.h"
#include "common/utility.h"

#include "tlkmdi_btble_btsnoop.h"

#ifndef BLUETOOTH_SNOOP_LOG_ENABLE
#define BLUETOOTH_SNOOP_LOG_ENABLE 0
#endif

#define BLUETOOTH_SNOOP_ENHANCED_ENABLE 1
#define BLUETOOTH_SNOOP_ENHANCED_WAIT   0

#if (BLUETOOTH_SNOOP_LOG_ENABLE)

#if (BLUETOOTH_SNOOP_ENHANCED_ENABLE)
#include <stdarg.h>
#include <stdio.h>

#include "driver.h"
#include "tlkmw/sys_dev/serial/tlkdrv_serial.h"
#define BT_SNOOP_SERIAL_BAUDRATE 3000000

#define BT_SNOOP_RX_BUFF_NUMBER  1
#define BT_SNOOP_RX_BUFF_SIZE    16
#define BT_SNOOP_TX_BUFF_NUMBER  16
#define BT_SNOOP_TX_BUFF_SIZE    256

__attribute__((aligned(4))) static uint8_t s_uart_send_buffer[BT_SNOOP_TX_BUFF_NUMBER * (BT_SNOOP_TX_BUFF_SIZE + 4)];
__attribute__((aligned(4))) static uint8_t s_uart_receive_buffer[BT_SNOOP_RX_BUFF_NUMBER * (BT_SNOOP_RX_BUFF_SIZE + 4)];

enum
{
    BT_SNOOP_TX_DATA = 0x01, // HCI TX packet
    BT_SNOOP_RX_DATA = 0x02, // HCI RX packet

    BT_SNOOP_RESET = 0x80,
};

static void ble_host_send_btsnoop_data(uint8_t type, const uint8_t *data, uint16_t data_len);

/**
 * @brief       This function is used to handle received serial data
 * @param[in]   pFrame - the pointer to the received frame data
 * @param[in]   frmLen - the length of the received frame
 * @return      none.
 */
static void app_serial_recv(uint8_t *pFrame, uint16_t frmLen)
{
    (void)pFrame;
    (void)frmLen;
}

/**
 * @brief       This function is used to initialize the bluetooth snoop functionality
 * @param       none.
 * @return      none.
 */
void bluetooth_host_snoop_initial(void)
{
    tlkdrv_serial_mount(UART0, BT_SNOOP_SERIAL_BAUDRATE, TLKDEV_SERIAL0_TX_PIN, TLKDEV_SERIAL0_RX_PIN, tlkhal_dma_malloc_ex(1), tlkhal_dma_malloc_ex(1));
    tlkdrv_serial_setTxQFifo(UART0, BT_SNOOP_TX_BUFF_NUMBER, BT_SNOOP_TX_BUFF_SIZE + 4, s_uart_send_buffer, BT_SNOOP_TX_BUFF_NUMBER * (BT_SNOOP_TX_BUFF_SIZE + 4));
    tlkdrv_serial_setRxQFifo(UART0, BT_SNOOP_TX_BUFF_NUMBER, BT_SNOOP_RX_BUFF_SIZE + 4, s_uart_receive_buffer, BT_SNOOP_TX_BUFF_NUMBER * (BT_SNOOP_RX_BUFF_SIZE + 4));
    tlkdrv_serial_regCB(UART0, app_serial_recv);
    tlkdrv_serial_open(UART0);

    ble_host_send_btsnoop_data(BT_SNOOP_RESET, NULL, 0);
}

/**
 * @brief       This function is used to calculate CRC16-CCITT checksum
 * @param[in]   data - the pointer to the data for CRC calculation
 * @param[in]   len - the length of the data
 * @return      the calculated CRC16 value.
 */
static uint16_t crc16_ccitt(const uint8_t *data, uint16_t len)
{
    uint16_t crc = 0xFFFF;
    for (uint16_t i = 0; i < len; i++) {
        crc ^= (uint16_t)data[i] << 8;
        for (int j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

/**
 * @brief       This function is used to send btsnoop data
 * @param[in]   type - the type of the data
 * @param[in]   data - the pointer to the data
 * @param[in]   data_len - the length of the data
 * @return      none.
 */
static void ble_host_send_btsnoop_data(uint8_t type, const uint8_t *data, uint16_t data_len)
{
    tlkdrv_serial_mount(UART0, BT_SNOOP_SERIAL_BAUDRATE, TLKDEV_SERIAL0_TX_PIN, TLKDEV_SERIAL0_RX_PIN, 0, 0);
    tlkdrv_serial_open(UART0);
    uint8_t debug_buffer[data_len + 7];

    uint8_t *p = debug_buffer; // 使用全局缓冲区
    // 包头
    U16_TO_STREAM(p, 0xAA55); // 使用宏定义转换为小端格式
    // 类型
    U8_TO_STREAM(p, type);
    // 长度（小端）
    U16_TO_STREAM(p, data_len);
    // 数据
    STR_TO_STREAM(p, data, data_len);

    // 计算 CRC（从 type 开始，长度为 1 + 2 + data_len）
    uint16_t crc = crc16_ccitt(&debug_buffer[2], 3 + data_len);
    // CRC（小端）
    U16_TO_STREAM(p, crc);

    uint16_t pkg_len = p - debug_buffer; // 包总长度

#if (BLUETOOTH_SNOOP_ENHANCED_WAIT)
    for (unsigned int i = 0; i < pkg_len; i++) {
        uart_send_byte(UART0, debug_buffer[i]);
    }
#else
    tlkdrv_serial_send(UART0, debug_buffer, pkg_len);
#endif
}

/**
 * @brief       This function is used to write HCI TX packet to snoop log
 * @param[in]   packet - the pointer to the HCI TX packet
 * @param[in]   length - the length of the packet
 * @return      none.
 */
void bluetooth_host_snoop_write_hci_tx_packet(const uint8_t *packet, uint16_t length)
{
    ble_host_send_btsnoop_data(BT_SNOOP_TX_DATA, packet, length);
}

/**
 * @brief       This function is used to write HCI RX packet to snoop log
 * @param[in]   packet - the pointer to the HCI RX packet
 * @param[in]   length - the length of the packet
 * @return      none.
 */
void bluetooth_host_snoop_write_hci_rx_packet(const uint8_t *packet, uint16_t length)
{
    ble_host_send_btsnoop_data(BT_SNOOP_RX_DATA, packet, length);
}

/**
 * @brief       This function is used to write log packet to snoop log
 * @param[in]   level - the log level
 * @param[in]   packet - the pointer to the log packet
 * @param[in]   length - the length of the packet
 * @return      none.
 */
void bluetooth_host_snoop_write_log_packet(enum bt_snoop_log_level level, const uint8_t *packet, uint16_t length)
{
    ble_host_send_btsnoop_data(level, packet, length);
}

/**
 * @brief       This function is used to print formatted log to snoop log
 * @param[in]   level - the log level
 * @param[in]   format - the format string
 * @param       ... - variable arguments
 * @return      none.
 */
void bluetooth_host_snoop_log_printf(enum bt_snoop_log_level level, const char *format, ...)
{
    char print_buffer[200];
    int  print_len = 0;

    va_list args;
    va_start(args, format);
    print_len = vsnprintf(print_buffer, sizeof(print_buffer), format, args);
    va_end(args);
    if (print_len < 0) {
        return;
    }
    if ((u32)print_len >= sizeof(print_buffer)) {
        print_len               = sizeof(print_buffer) - 1;
        print_buffer[print_len] = '\0';
    }
    bluetooth_host_snoop_write_log_packet(level, (uint8_t *)print_buffer, (uint16_t)print_len);
}
#else

/**
 * @brief       This function is used to initialize the bluetooth snoop functionality
 * @param       none.
 * @return      none.
 */
void bluetooth_host_snoop_initial(void) {}

/**
 * @brief       This function is used to print formatted log to snoop log
 * @param[in]   level - the log level
 * @param[in]   format - the format string
 * @param       ... - variable arguments
 * @return      none.
 */
void bluetooth_host_snoop_log_printf(enum bt_snoop_log_level level, const char *format, ...)
{
    (void)level;
    (void)format;
}

/**
 * @brief       This function is used to write data to btsnoop
 * @param[in]   header - the pointer to the header string
 * @param[in]   data - the pointer to the data
 * @param[in]   length - the length of the data
 * @return      none.
 */
static void ble_host_write_to_btsnoop(char *header, const uint8_t *data, uint16_t length)
{
    tlkdbg_send_str_data(header, (u8 *)(size_t)data, length);
    tlkdbg_send_str_data("[BT_SNOOP] end of packet.", NULL, 0);
}

/**
 * @brief       This function is used to write HCI TX packet to snoop log
 * @param[in]   packet - the pointer to the HCI TX packet
 * @param[in]   length - the length of the packet
 * @return      none.
 */
void bluetooth_host_snoop_write_hci_tx_packet(const uint8_t *packet, uint16_t length)
{
    ble_host_write_to_btsnoop("[BT_SNOOP] HCI TX Packet", packet, length);
}

/**
 * @brief       This function is used to write HCI RX packet to snoop log
 * @param[in]   packet - the pointer to the HCI RX packet
 * @param[in]   length - the length of the packet
 * @return      none.
 */
void bluetooth_host_snoop_write_hci_rx_packet(const uint8_t *packet, uint16_t length)
{
    ble_host_write_to_btsnoop("[BT_SNOOP] HCI RX Packet", packet, length);
}

/**
 * @brief       This function is used to write log packet to snoop log
 * @param[in]   level - the log level
 * @param[in]   packet - the pointer to the log packet
 * @param[in]   length - the length of the packet
 * @return      none.
 */
void bluetooth_host_snoop_write_log_packet(enum bt_snoop_log_level level, const uint8_t *packet, uint16_t length)
{
    (void)level;
    (void)packet;
    (void)length;
}

#endif

#else
/**
 * @brief       This function is used to initialize the bluetooth snoop functionality
 * @param       none.
 * @return      none.
 */
void bluetooth_host_snoop_initial(void) {}

/**
 * @brief       This function is used to write HCI TX packet to snoop log
 * @param[in]   packet - the pointer to the HCI TX packet
 * @param[in]   length - the length of the packet
 * @return      none.
 */
void bluetooth_host_snoop_write_hci_tx_packet(const uint8_t *packet, uint16_t length)
{
    (void)packet;
    (void)length;
}

/**
 * @brief       This function is used to write HCI RX packet to snoop log
 * @param[in]   packet - the pointer to the HCI RX packet
 * @param[in]   length - the length of the packet
 * @return      none.
 */
void bluetooth_host_snoop_write_hci_rx_packet(const uint8_t *packet, uint16_t length)
{
    (void)packet;
    (void)length;
}

/**
 * @brief       This function is used to write log packet to snoop log
 * @param[in]   level - the log level
 * @param[in]   packet - the pointer to the log packet
 * @param[in]   length - the length of the packet
 * @return      none.
 */
void bluetooth_host_snoop_write_log_packet(enum bt_snoop_log_level level, const uint8_t *packet, uint16_t length)
{
    (void)level;
    (void)packet;
    (void)length;
}

/**
 * @brief       This function is used to print formatted log to snoop log
 * @param[in]   level - the log level
 * @param[in]   format - the format string
 * @param       ... - variable arguments
 * @return      none.
 */
void bluetooth_host_snoop_log_printf(enum bt_snoop_log_level level, const char *format, ...)
{
    (void)level;
    (void)format;
}
#endif