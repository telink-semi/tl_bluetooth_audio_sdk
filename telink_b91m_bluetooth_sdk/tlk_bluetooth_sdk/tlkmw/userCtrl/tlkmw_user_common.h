/********************************************************************************************************
 * @file    tlkmw_user_common.h
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
#ifndef TLKMW_USER_COMMON_H
#define TLKMW_USER_COMMON_H

enum
{
    BINX_D25F = 0x01,
    BINX_N22  = 0x02,
    BINX_DSP  = 0x03,
    BINX_TONE = 0x04,

    BINX_CUSTOMER = 0x0f,
    BINX_MAX,
};

typedef union
{
    uint32_t crc32;
    uint8_t  sha256[20];
    uint8_t  resv[32];
} BinInfoLabelNum_t1;

typedef struct
{
    uint32_t           dfu_bin_addr;
    uint32_t           dfu_bin_size;
    BinInfoLabelNum_t1 dfu_bin_crc;
    uint32_t           dfu_bin_offset;
    uint32_t           dfu_bin_trs_end;
    uint32_t           dfu_wait_reboot;
} dfuOutImgCtrl_t;

/**
 * @brief      Get the startup address of N22 core from flash
 * @param[in]  none
 * @param[out] none
 * @return     uint32_t - N22 core startup address
 */
uint32_t tlkmw_getN22StartUpAddrFromFlash(void);

/**
 * @brief      Get the startup address of DSP core from flash
 * @param[in]  none
 * @param[out] none
 * @return     uint32_t - DSP core startup address
 */
uint32_t tlkmw_getDSPStartUpAddrFromFlash(void);
#endif
