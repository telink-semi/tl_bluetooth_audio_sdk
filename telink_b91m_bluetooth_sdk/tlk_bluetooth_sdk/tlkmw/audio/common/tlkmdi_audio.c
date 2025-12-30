/********************************************************************************************************
 * @file    tlkmdi_audio.c
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
 * @brief       This function is used to initialize audio module
 * @param[in]   None.
 * @return      None.
 */
void tlkmdi_audio_init(void)
{
#if (TLK_CFG_RTOS_ENABLE)
    tlkmdi_audio_use_rtos();
#endif

#if (TLK_DEV_CODEC_ENABLE)
    tlkmw_codec_init();
#endif
    tlkmdi_audio_debug_init();
    tlkmdi_audmem_init();

    #if TLK_MW_DSP_COMM_ENABLE
    tlkmw_dsp_init();
    #endif

    #if TLKALG_ASRC_441TO48_16BIT_TEMP_ENABLE
    bt_music_441to48_init();
    #endif

    #if TLKALG_EQ_ENABLE
    tlkalg_eq_read_para_from_flash(0);
    #endif
}

/**
 * @brief       This function is used to send audio start event
 * @param[in]   audChn - Audio channel
 * @param[in]   handle - Connection handle
 * @return      TLK_ENONE indicates success; other value indicates failure
 */
int tlkmdi_audio_sendStartEvt(uint8_t audChn, uint16_t handle)
{
    return tlkmdi_audio_sendStartEvtEx(audChn, handle, 0Xff);
}

/**
 * @brief       This function is used to send audio start event with priority
 * @param[in]   audChn - Audio channel
 * @param[in]   handle - Connection handle
 * @param[in]   priority - Priority level
 * @return      TLK_ENONE indicates success; other value indicates failure
 */
int tlkmdi_audio_sendStartEvtEx(uint8_t audChn, uint16_t handle, uint8_t priority)
{
    uint8_t buffer[4];
    buffer[0] = audChn;
    buffer[1] = priority;
    buffer[2] = (handle & 0x00FF);
    buffer[3] = (handle & 0xFF00) >> 8;
    return tlksys_sendMsg(TLKSYS_TASKID_AUDIO, TLKSYS_AUD_MSGID_START_EVT, buffer, 4);
}

/**
 * @brief       This function is used to send audio close event
 * @param[in]   audChn - Audio channel
 * @param[in]   handle - Connection handle
 * @return      TLK_ENONE indicates success; other value indicates failure
 */
int tlkmdi_audio_sendCloseEvt(uint8_t audChn, uint16_t handle)
{
    return tlkmdi_audio_sendCloseEvtEx(audChn,handle,0);
}

/**
 * @brief       This function is used to send audio close event with delete option
 * @param[in]   audChn - Audio channel
 * @param[in]   handle - Connection handle
 * @param[in]   isDelete - Delete flag
 * @return      TLK_ENONE indicates success; other value indicates failure
 */
int tlkmdi_audio_sendCloseEvtEx(uint8_t audChn, uint16_t handle, uint8_t isDelete)
{
    uint8_t buffer[4];
    buffer[0] = audChn;
    buffer[1] = isDelete;
    buffer[2] = (handle & 0x00FF);
    buffer[3] = (handle & 0xFF00) >> 8;
    return tlksys_sendMsg(TLKSYS_TASKID_AUDIO, TLKSYS_AUD_MSGID_CLOSE_EVT, buffer, 4);
}

/**
 * @brief       This function is used to send debug message output event
 * @param[in]   None.
 * @return      None.
 */
_always_inline void tlkmdi_audio_sendDebugMsgOutput(void)
{
    tlksys_task_setEvtFromIsr(TLKSYS_TASKID_AUDIO,TLKSYS_TASK_EVT_AUD_DEBUG);
}

/**
 * @brief       This function is used to send TX power change event
 * @param[in]   None.
 * @return      None.
 */
_always_inline void tlkmdi_audio_sendChangeTxPower(void)
{
    tlksys_task_setEvt(TLKSYS_TASKID_AUDIO,TLKSYS_TASK_EVT_AUD_MAIN);
}

#endif // #if (TLK_MW_AUDIO_ENABLE)
