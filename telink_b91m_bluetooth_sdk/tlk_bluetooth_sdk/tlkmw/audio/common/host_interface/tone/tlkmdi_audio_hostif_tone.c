/********************************************************************************************************
 * @file    tlkmdi_audio_hostif_tone.c
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
#include "drivers.h"
#include "tlkapi/tlkapi.h"
#include "tlkmw/tlkmw.h"
#if (TLK_MW_AUDIO_ENABLE)

/**
 * @brief Check whether the tone playing request needs to be refused.
 *        In this implementation, if device is not in ear, tone playing will be refused.
 * @param[in] tone_id - The ID of the tone to be played.
 * @return true - The tone playing request should be refused.
 * @return false - The tone playing request can be accepted.
 */
bool tlkmdi_audio_hostif_tone_isNeedRefuse(uint8_t tone_id)
{
    (void)tone_id;
    //not in ear,so refuse play tone
    return !tlkmdi_bt_tpt_isInEar();
}

/**
 * @brief Check whether active tone playing is supported.
 *        In this implementation, slave device in TWS mode doesn't support active tone playing.
 * @param[in] tone_id - The ID of the tone to be played.
 * @return true - Active tone playing is supported.
 * @return false - Active tone playing is not supported.
 */
bool tlkmdi_audio_hostif_tone_isSupportActivePlay(uint8_t tone_id)
{
    (void)tone_id;
#if TLKSTK_BTTPSLL_TWS_ENABLE
    if (tlkmdi_bt_tpt_isSlave()) {
        return false;
    }
#endif
    return true;
}

/**
 * @brief Check whether synchronization is needed for tone playing.
 *        In this implementation, synchronization is needed only in TWS mode.
 * @return true - Tone playing requires synchronization.
 * @return false - Tone playing does not require synchronization.
 */
bool tlkmdi_audio_hostif_tone_isNeedSyncPlay(void)
{
#if TLKSTK_BTTPSLL_TWS_ENABLE
    return true;
#else
    return false;
#endif
}

/**
 * @brief Request synchronized tone playing.
 *        In this implementation, request tone sync in TWS mode.
 * @param[in] tone_id - The ID of the tone to be played.
 * @return 0 - Success.
 * @return TLK_ENOSUPPORT - Not supported.
 */
int tlkmdi_audio_hostif_tone_requestSyncPlay(uint8_t tone_id)
{
    (void)tone_id;
#if TLKSTK_BTTPSLL_TWS_ENABLE
    return tlkmdi_bt_tpt_requestToneSync(tone_id);
#else
    return -TLK_ENOSUPPORT;
#endif
}

/**
 * @brief Register callback function for tone synchronization.
 *        In this implementation, register tone sync callback in TWS mode.
 * @param[in] cb - Callback function for tone synchronization.
 * @return 0 - Success.
 * @return TLK_ENOSUPPORT - Not supported.
 */
int tlkmdi_audio_hostif_tone_regSyncCB(TlkMdiAudHostifToneSyncCB cb)
{
    (void)cb;
#if TLKSTK_BTTPSLL_TWS_ENABLE
    return tlkmdi_bt_tpt_regToneSyncCB(cb);
#else
    return -TLK_ENOSUPPORT;
#endif
}

#endif // #if (TLK_MW_AUDIO_ENABLE)
