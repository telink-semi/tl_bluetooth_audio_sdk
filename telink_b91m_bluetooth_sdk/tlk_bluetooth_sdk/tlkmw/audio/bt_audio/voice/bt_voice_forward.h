/********************************************************************************************************
 * @file    bt_voice_forward.h
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
#ifndef __BT_VOICE_FORWARD_H__
#define __BT_VOICE_FORWARD_H__

void bt_voice_forward_switch_in(uint16_t handle);
void bt_voice_forward_switch_out(uint16_t handle);
void bt_voice_forward_process_uplink(uint8_t sco_queue_id);
void bt_voice_forward_process_downlink(uint8_t sco_queue_id);
void bt_voice_forward_audio_irq_task(void);
void bt_voice_forward_sco_rx_evt_handler(uint8_t queue_id);
void bt_voice_forward_main_loop(void);
void bt_voice_forward_set_sco_rx_evt(uint8_t queue_id);
void bt_voice_forward_clear_sco_rx_evt(uint8_t queue_id);
void bt_voice_forward_sco_timer_init(void);

#endif
