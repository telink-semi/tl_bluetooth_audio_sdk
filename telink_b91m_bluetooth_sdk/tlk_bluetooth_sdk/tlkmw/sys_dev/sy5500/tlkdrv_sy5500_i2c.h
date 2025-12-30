/********************************************************************************************************
 * @file    tlkdrv_sy5500_i2c.h
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

/**
 * @brief SY5500 I2C event enumeration definition
 */
typedef enum
{
    /** Enter charging box event */
    TLKDRV_SY5500_I2C_EVT_ENTER_BOX = 0,

    /** Leave charging box event */
    TLKDRV_SY5500_I2C_EVT_LEAVE_BOX,

    /** 5V power on event */
    TLKDRV_SY5500_I2C_EVT_5V_ON,

    /** 5V power off event */
    TLKDRV_SY5500_I2C_EVT_5V_OFF,

    /** 5V power error event */
    TLKDRV_SY5500_I2C_EVT_5V_POWER_ERROR,
} TLKDRV_SY5500_I2C_EVT_ENUM;

/**
 * @brief SY5500 I2C event callback function pointer type
 * @param[in] evtID Event ID
 */
typedef void (*TlkDrvSy5500I2cEvtCB)(uint16_t evtID);

/**
 * @brief Initialize SY5500 I2C interface
 * @param[in] writeRetryTimes Write retry times
 * @param[in] evtCB Event callback function
 */
void tlkdrv_sy5500_i2c_init(uint8_t writeRetryTimes, TlkDrvSy5500I2cEvtCB evtCB);

/**
 * @brief Exit RTX mode
 */
void tlkdrv_sy5500_i2c_exitRTXMode(void);

/**
 * @brief Handle I2C state changes
 */
void tlkdrv_sy5500_i2c_stateChgHandler(void);