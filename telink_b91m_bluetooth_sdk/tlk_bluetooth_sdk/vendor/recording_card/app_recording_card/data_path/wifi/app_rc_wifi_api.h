/********************************************************************************************************
 * @file    app_rc_wifi_api.h
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

typedef enum
{
    WIFI_SPI_CMD_GET_STATE      = 0X80,
    WIFI_SPI_CMD_SET_NAME       = 0X82,
    WIFI_SPI_CMD_GET_DFU_STATUS = 0x8B,
    WIFI_SPI_CMD_START_DFU      = 0x8C,
    WIFI_SPI_CMD_STOP_DFU       = 0x8D,
    WIFI_SPI_CMD_STOP_SEND_DATA = 0x8F,
} WIFI_SPI_CMD_ENUM;

typedef void (*rc_wifi_spi_send_finished_cb_t)(uint8_t isSuccess, uint8_t *pAckData, uint16_t dataLen);

int app_rc_wifi_get_name(uint8_t *buffer, uint32_t bufferLen);

int app_rc_wifi_spi_send_data(uint8_t *pData, uint16_t dataLen, rc_wifi_spi_send_finished_cb_t cb);

int app_rc_wifi_spi_send_cmd(uint8_t cmd, uint8_t *pData, uint16_t dataLen, rc_wifi_spi_send_finished_cb_t cb);

void app_rc_wifi_spi_cleanFrameNum(void);

void app_rc_wifi_core_power_on(void);

void app_rc_wifi_core_power_off(void);

void app_rc_wifi_spi_init(void);

int app_rc_wifi_uart_sendData(uint8_t *pData, uint16_t dataLen);

//weak function need overwrite
void app_rc_wifi_uart_getDataDeal(uint8_t *pData, uint16_t dataLen);

void app_rc_wifi_startOtaDataCopy(void);

bool app_rc_wifi_isInOta(void);

bool app_rc_wifi_is_connected_to_app(void);
