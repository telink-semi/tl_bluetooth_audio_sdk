/********************************************************************************************************
 * @file    emi_general.h
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
#ifndef TLKDRV_B92_EXT_DRIVER_EMI_EMI_GENERAL_H_
#define TLKDRV_B92_EXT_DRIVER_EMI_EMI_GENERAL_H_
#include "drivers.h"
#if ((BT_INTERNAL_MODE == BT_RFTEST_EMI)) // && (MCU_CORE_TYPE == MCU_CORE_B92)
/**
 * @brief Generate EMI binary that support setting
 * @note  When compiling sram bin, you need to delete (NOLOAD) after the sbss and bss segments in ram_boot.link.
 *        At the same time, delete the AT instruction content after sdk_version segment.
 */
//#define MCU_STARTUP_FLASH           1

/**
 * @brief Define where the configuration data will be saved in the binary file.
 */
#define VERSION_ADDR         (0x02)
#define PA_SETTING_ADDR      (0x04)
#define GENERAL_SETTING_ADDR (0x16)
#if (CHIP_TYPE == MCU_CORE_TL751X)
#define SRAM_BASE_ADDR 0x00020000
#else
#define SRAM_BASE_ADDR 0xc0000000
#endif

#define MAX_RF_CHANNEL           80
#define ACCESS_CODE              0x29417671
#define SWITCH_POWER_MODE        0x00 /**< Power mode of the chip.(LDO_LDO:0x00;DCDC_LDO:0x01;DCDC_DCDC:0x03)*/
#define SWITCH_INTERNAL_CAP      1    /**< Capacitor selection: 0 for internal cap, 1 for external cap. */
#define SWITCH_CALI_POSITION     0    /**< Calibration position: 0 for FLASH, 1 for OTP. */
#define SWITCH_GPIO_VOLTAGE      0    /**< The configuration of GPIO voltage. (0:GPIO voltage 3.3V 1:GPIO voltage 1.8V)*/
#define PA_FUNCTION_ENABLE       0    /**< PA enable flag (0: disabled,1: enabled). */
#define PA_BYPASS_ENABLE         0    /**< PA bypass enable flag (0: disabled,1: enabled).*/
#define SWIRE_THROUGH_USB_ENABLE 1    /**< Enable Swire through USB(0: disabled,1: enabled) */
#define POWER_SLICE_EN           0    /**< Power slice enable flag (0: disabled,1: enabled)*/

#define EMI_TX_PACKET_CNT        0 /**< Tx send packet quantity mode selection. (0:Unlimit,1:1000)*/
#define EMI_RUN_STATE            1 /**< Current operational state. */
#define EMI_TEST_CMD_NOW         1 /**< Current test command being executed. */
#define EMI_POWER_LEVEL          0 /**< If POWER_SLICE_EN is 0:tx power level;If POWER_SLICE_EN is 1: scale << 8 | slice. */
#define EMI_RF_CHN               2 /**< RF channel number.Default: 2 (Frequency channel 2402 MHz) */
#define EMI_RF_MODE              0 /**< RF mode setting.(BR_DH1,BR_DH3,BR_DH5,EDR2_DH1,EDR2_DH3,EDR2_DH5,EDR3_DH1,EDR3_DH3,EDR3_DH5) */
#define EMI_HOPPING_EN           0 /**< Frequency hopping enable flag. (0:disable, 1:enable) */
#define EMI_PKT_TONE_EN          0 /**< Packet tone enable flag.   (0:disable, 1:enable)*/
#define EMI_PKT_TYPE             0 /**< The type of data sent (0:pbrs9  1:0x55  2:0x0f  3:0x00  4:0xff) */

#define RSSI_ADDR                (SRAM_BASE_ADDR + 0x4)
#define TX_PACKET_MODE_ADDR      (SRAM_BASE_ADDR + 0x5)
#define RUN_STATUE_ADDR          (SRAM_BASE_ADDR + 0x6)
#define TEST_COMMAND_ADDR        (SRAM_BASE_ADDR + 0x7)
#define POWER_ADDR               (SRAM_BASE_ADDR + 0x8)
#define CHANNEL_ADDR             (SRAM_BASE_ADDR + 0x9)
#define RF_MODE_ADDR             (SRAM_BASE_ADDR + 0xa)
#define EMI_CONFIG_FLAG_ADDR     (SRAM_BASE_ADDR + 0xb)
#define RX_PACKET_NUM_ADDR       (SRAM_BASE_ADDR + 0xc)
#define RX_PACKET_TYPE_ADDR      (SRAM_BASE_ADDR + 0xd)
#define TX_POWER_SLICE           (SRAM_BASE_ADDR + 0x8)
#define TX_POWER_SCALE           (SRAM_BASE_ADDR + 0x10)

/**
 * @brief This structure defines various parameters related to EMI test
 */
typedef struct
{
    signed char   g_rssi_val;                 /**< Received Signal Strength Indicator (RSSI) value. */
    unsigned char g_tx_packet_cnt;            /**< Tx send packet quantity mode selection. 0:Unlimit,1:1000*/
    unsigned char g_run_state;                /**< Current operational state. */
    unsigned char g_test_cmd_now;             /**< Current test command being executed. */
    unsigned char g_power_level;              /**< If g_power_slice_en is 0:tx power level;If g_power_slice_en is 1: tx power slice. */
    unsigned char g_chn;                      /**< RF channel number.Default: 2 (Frequency channel 2402 MHz) */
    unsigned char g_rf_mode;                  /**< RF mode setting.(BR_DH1,BR_DH3,BR_DH5,EDR2_DH1,EDR2_DH3,EDR2_DH5,EDR3_DH1,EDR3_DH3,EDR3_DH5) */
    unsigned char g_hop_en               : 1; /**< Frequency hopping enable flag. */
    unsigned char g_adaptive_en          : 1; /**< Adaptive test enable flag. (0: disabled,1: enabled)*/
    unsigned char g_pkt_tone_en          : 1; /**< Packet tone enable flag. (0: disabled,1: enabled)*/
    unsigned char g_power_scale_slice_en : 1; /**< Power slice enable flag (0: disabled,1: enabled)*/
    unsigned char                        : 4; /**< Reserved bits. */
    unsigned char g_pkt_type;                 /**< The type of data sent (0:pbrs9  1:0x55  2:0x0f  3:0x00  4:0xff)*/
    unsigned char g_scale;                    /**< tx power slice)*/
    unsigned char g_slice;                    /**< tx power scale)*/
} __attribute__((packed)) emi_cmd_t;

extern emi_cmd_t emi_cmd_now;
#endif
void emi_start();
void emi_loop_server();

#endif /* TLKDRV_B92_EXT_DRIVER_EMI_EMI_GENERAL_H_ */
