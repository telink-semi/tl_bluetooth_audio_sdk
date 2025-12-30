/********************************************************************************************************
 * @file    tlkmdi_a2dp_to_bis_bt_music.h
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
#ifndef __TLKMDI_A2DP_TO_BIS_BT_MUSIC_H__
#define __TLKMDI_A2DP_TO_BIS_BT_MUSIC_H__

/**
 * @brief       Main loop for A2DP to BIS Bluetooth music processing
 * @param       None
 * @return      None
 */
void a2dp_to_bis_bt_main_loop(void);
/**
 * @brief       Set decoder tick value for A2DP to BIS conversion
 * @param[in]   ticks - Tick value to set
 * @return      None
 */
void a2dp_to_bis_set_dec_ticks(uint32_t ticks);

#endif