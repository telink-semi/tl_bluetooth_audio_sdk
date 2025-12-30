/********************************************************************************************************
 * @file    tlkdrv_sy5500_i2c_reg.h
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

/*
note:this headfile is private,only can be included in sy5500 .c file.
*/

/**
 * @brief SY5500 I2C slave address
 */
#define TLKDEV_SY5500_I2C_SLAVE_ADDR 0x16

/**
 * @brief SY5500 chip status definitions
 */
#define _SY5500_ST_CHIP_Standby        0x00 /*!< Standby mode */
#define _SY5500_ST_CHIP_ShipMode       0x01 /*!< Shipping mode */
#define _SY5500_ST_CHIP_TRX            0x02 /*!< Transceiver mode */
#define _SY5500_ST_CHIP_ChargeOFF      0x03 /*!< Charging disabled */
#define _SY5500_ST_CHIP_Charge_Trickle 0x04 /*!< Trickle charging */
#define _SY5500_ST_CHIP_Charge_CC      0x05 /*!< Constant current charging */
#define _SY5500_ST_CHIP_Charge_CV      0x06 /*!< Constant voltage charging */
#define _SY5500_ST_CHIP_Charge_FULL    0x07 /*!< Charging completed */
#define _SY5500_ST_CHIP_IS_CHARGING    0x04 /*!< Charging in progress */

/**
 * @brief SY5500 register address definitions
 */
#define _ADD_SY5500_ST_10           0x10 /*!< Status register 10 */
#define _ADD_SY5500_ST_11           0x11 /*!< Status register 11 */
#define _ADD_SY5500_ST_20           0x20 /*!< Status register 20 */

#define _ADD_SY5500_IO_CaseCheck_40 0x40 /*!< IO detection register 40 */
#define _ADD_SY5500_RstDelay_set_41 0x41 /*!< Reset delay setting register 41 */
#define _ADD_SY5500_PWKSet_42       0x42 /*!< PWK setting register 42 */
#define _ADD_SY5500_VinPullDown_43  0x43 /*!< Vin pull-down setting register 43 */
#define _ADD_SY5500_I2C_CMD_44      0x44 /*!< I2C command register 44 */

#define _ADD_SY5500_VER_WP_50       0x50 /*!< Version write protection register 50 */
#define _ADD_SY5500_VER_WP_60       0x60 /*!< Version write protection register 60 */
#define _ADD_SY5500_VER_WP_61       0x61 /*!< Version write protection register 61 */
#define _ADD_SY5500_VER_WP_62       0x62 /*!< Version write protection register 62 */
#define _ADD_SY5500_WPEN_72         0x72 /*!< Write protection enable register 72 */
#define _ADD_SY5500_WPEN_73         0x73 /*!< Write protection enable register 73 */

/**
 * @brief Write protection clear keys
 */
#define _KEY_CLR_SY5500_WPEN_73 0x39 /*!< Key to clear WPEN_73 */
#define _KEY_SET_SY5500_WPEN_73 0x00 /*!< Key to set WPEN_73 */

/**
 * @brief Write protection clear keys
 */
#define _KEY_CLR_SY5500_WPEN_72 0x97 /*!< Key to clear WPEN_72 */
#define _KEY_SET_SY5500_WPEN_72 0x00 /*!< Key to set WPEN_72 */

/**
 * @brief SY5500 I2C command definitions
 */
#define _SY5500_I2C_CMD_EnableCHG   0x01 /*!< Enable charging command */
#define _SY5500_I2C_CMD_DisableCHG  0x02 /*!< Disable charging command */
#define _SY5500_I2C_CMD_ShipmodeON  0x03 /*!< Enter shipping mode command */
#define _SY5500_I2C_CMD_Restart     0x04 /*!< Restart command */
#define _SY5500_I2C_CMD_ExitTRXmode 0x05 /*!< Exit transceiver mode command */
#define _SY5500_I2C_CMD_ClrRst      0x06 /*!< Clear reset command */
#define _SY5500_I2C_CMD_ClrIrq      0x07 /*!< Clear interrupt command */

/**
 * @brief Register write control structure
 */
typedef struct
{
    uint8_t reg;         /*!< Register address */
    uint8_t val;         /*!< Register value */
    uint8_t retryTimes;  /*!< Retry times */
    uint8_t haveChanged; /*!< Changed flag */
} regs_write_t;

/**
 * @brief Register cache structure
 */
typedef struct
{
    uint8_t state[3]; /*!< Status register cache */
} regs_cache_t;