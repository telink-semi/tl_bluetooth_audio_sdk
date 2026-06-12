/********************************************************************************************************
 * @file    tlkmdi_audio_btif_volume.h
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
#ifndef TLKMDI_AUDIO_BT_INTERFACE_VOLUME_H
#define TLKMDI_AUDIO_BT_INTERFACE_VOLUME_H

/**
 * @brief   Type definition for volume change callback function
 */
typedef void (*TlkMdiAudBtifVolChgCB)(uint16_t handle, uint8_t vol);

/**
 * @brief       Register callback for voice volume change events
 * @param[in]   cb - Callback function pointer
 * @return      None
 */
void tlkmdi_audio_btif_regVoiceVolChgCB(TlkMdiAudBtifVolChgCB cb);

/**
 * @brief       Register callback for music volume change events
 * @param[in]   cb - Callback function pointer
 * @return      None
 */
void tlkmdi_audio_btif_regMusicVolChgCB(TlkMdiAudBtifVolChgCB cb);

/**
 * @brief       Get volume step value for device
 * @param[in]   handle - ACL connection handle
 * @param[in]   isMusic - Music or voice flag
 * @param[out]  step - Pointer to store volume step value
 * @param[out]  isIos - Pointer to store iOS device flag
 * @return      TLK_ENONE: success; Others: failure
 */
int tlkmdi_audio_btif_getVolumeStep(uint16_t handle, uint8_t isMusic, uint8_t *step, uint8_t *isIos);

/**
 * @brief       Operate volume (increase or decrease)
 * @param[in]   handle - ACL connection handle
 * @param[in]   isInc - Increase flag (1: increase, 0: decrease)
 * @param[in]   isMusic - Music or voice flag
 * @return      TLK_ENONE: success; Others: failure
 */
int tlkmdi_audio_btif_VolumeOperate(uint16_t handle, uint8_t isInc, uint8_t isMusic);

#endif // TLKMDI_AUDIO_BT_INTERFACE_VOLUME_H
