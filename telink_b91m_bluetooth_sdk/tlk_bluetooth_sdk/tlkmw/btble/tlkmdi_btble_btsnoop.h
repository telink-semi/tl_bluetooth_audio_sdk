/********************************************************************************************************
 * @file    tlkmdi_btble_btsnoop.h
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
enum bt_snoop_log_level
{
    BT_SNOOP_MSG_BASE = 0x10,
    BT_SNOOP_MSG_LOG,
    BT_SNOOP_MSG_WAR,
    BT_SNOOP_MSG_ERR,
};

/**
 * @brief       This function is used to initialize the bluetooth snoop functionality
 * @param       none.
 * @return      none.
 */
void bluetooth_host_snoop_initial(void);

/**
 * @brief       This function is used to write HCI TX packet to snoop log
 * @param[in]   packet - the pointer to the HCI TX packet
 * @param[in]   length - the length of the packet
 * @return      none.
 */
void bluetooth_host_snoop_write_hci_tx_packet(const uint8_t *packet, uint16_t length);

/**
 * @brief       This function is used to write HCI RX packet to snoop log
 * @param[in]   packet - the pointer to the HCI RX packet
 * @param[in]   length - the length of the packet
 * @return      none.
 */
void bluetooth_host_snoop_write_hci_rx_packet(const uint8_t *packet, uint16_t length);

/**
 * @brief       This function is used to write log packet to snoop log
 * @param[in]   level - the log level
 * @param[in]   packet - the pointer to the log packet
 * @param[in]   length - the length of the packet
 * @return      none.
 */
void bluetooth_host_snoop_write_log_packet(enum bt_snoop_log_level level, const uint8_t *packet, uint16_t length);

/**
 * @brief       This function is used to print formatted log to snoop log
 * @param[in]   level - the log level
 * @param[in]   format - the format string
 * @param       ... - variable arguments
 * @return      none.
 */
void bluetooth_host_snoop_log_printf(enum bt_snoop_log_level level, const char *format, ...);