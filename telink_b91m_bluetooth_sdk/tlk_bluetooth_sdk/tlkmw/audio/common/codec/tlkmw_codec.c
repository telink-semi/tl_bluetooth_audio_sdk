/********************************************************************************************************
 * @file    tlkmw_codec.c
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
#include "tlkmw/sys_dev/codec/tlkdrv_codec.h"
#include "tlkmw_codec_cfg.h"

#if (TLK_DEV_CODEC_ENABLE)

/**
 * @brief  initialize codec module
 * @param[in]  none
 * @returns none
 */
void tlkmw_codec_init(void)
{
    tlkdrv_codec_power_on();
}

#endif
