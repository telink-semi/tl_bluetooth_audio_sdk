/********************************************************************************************************
 * @file    hal_spi.h
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

enum
{
    SPI_CLK_40MHZ = 0,
    SPI_CLK_30MHZ,
    SPI_CLK_20MHZ,
    SPI_CLK_15MHZ,
    SPI_CLK_10MHZ,
    SPI_CLK_4MHZ,
    SPI_CLK_1MHZ,
    SPI_CLK_250KHZ,
    SPI_CLK_NUM
};

/**
 * @brief       This function selects/deselects the SPI master.
 * @param[in]   en    - enable flag (1 to select, 0 to deselect).
 * @return      none.
 */
void hal_spi_master_sel(int en);

/**
 * @brief       This function performs dummy write operations on SPI master.
 * @param[in]   n    - number of dummy bytes to write.
 * @return      Returns 1 on success, 0 on failure.
 */
int hal_spi_master_dummy(int n);

/**
 * @brief       This function performs byte-level SPI transfer.
 * @param[in]   w     - byte to write.
 * @param[out]  dr    - pointer to store the read byte.
 * @return      none.
 */
void hal_spi_master_byte_trans(uint8_t w, uint8_t *dr);

/**
 * @brief       This function writes data via SPI master.
 * @param[in]   dat    - pointer to data buffer to write.
 * @param[in]   len    - length of data to write.
 * @return      Returns 1 on success, 0 on failure.
 */
int hal_spi_master_write(uint8_t *dat, int len);

/**
 * @brief       This function reads data via SPI master.
 * @param[out]  dat    - pointer to buffer to store read data.
 * @param[in]   len    - length of data to read.
 * @return      Returns 1 on success, 0 on failure.
 */
int hal_spi_master_read(uint8_t *dat, int len);

/**
 * @brief       This function sets the SPI master clock speed.
 * @param[in]   is_low_speed    - speed setting (use SPI_CLK_* enums).
 * @return      Returns 1 on success.
 */
int hal_spi_master_clk_set(int is_low_speed);

/**
 * @brief       This function sets the SPI master dual mode enable/disable.
 * @param[in]   en    - enable/disable.
 * @return      none.
 */
void hal_spi_master_dual_mode_en(uint8_t en);

/**
 * @brief       This function sets the SPI master quad mode enable/disable.
 * @param[in]   en    - enable/disable.
 * @param[in]   io23_level -io2 and io3 level when quad mode disable.
 * @return      none.
 */
void hal_spi_master_quad_mode_en(uint8_t en, uint8_t io23_level);

/**
 * @brief       This function initializes the SPI master.
 * @param[in]   none.
 * @return      none.
 */
void hal_spi_master_init(void);

/**
 * @brief       This function deinitializes the SPI master.
 * @param[in]   none.
 * @return      none.
 */
void hal_spi_master_deinit(void);
