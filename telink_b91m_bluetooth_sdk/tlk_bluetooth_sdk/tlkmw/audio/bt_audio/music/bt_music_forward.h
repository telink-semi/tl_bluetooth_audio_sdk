/********************************************************************************************************
 * @file    bt_music_forward.h
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
#ifndef __BT_MUSIC_FORWARD_H__
#define __BT_MUSIC_FORWARD_H__

void     bt_music_forward_switch_in(uint16_t handle);
void     bt_music_forward_switch_out(uint16_t handle);
void     bt_music_forward_a2dp_buff_init(uint8_t *p_a2dp_buff);
uint8_t  bt_music_forward_copy_data_to_a2dp_buffer(uint8_t *p_a2dp_buff, uint16_t len);
uint8_t *bt_music_forward_get_data_from_a2dp_buffer(uint16_t *p_len);
uint8_t *bt_music_forward_read_data_from_a2dp_buffer(void);
uint16_t bt_music_forward_get_available_frames(void);
uint16_t bt_music_forward_get_free_frames(void);
void     bt_music_forward_reset_a2dp_buffer(void);
void     bt_music_forward_a2dp_tx_evt_handler(void);

#endif
