/********************************************************************************************************
 * @file    tlkmdi_anc.c
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
#include "tlkmw/tlkmw.h"
#include "tlkmdi_anc.h"
#include "tlkmdi_anc_handler.h"
#include "sch/tlkmw_anc_sch.h"

#if TLKALG_ANC_ENABLE

extern tlkmdi_anc_ctl g_anc_ctr;
tlkmdi_anc_t          s_anc_env = {0};

/**
 * @brief       Control BT music for ANC
 * @param[in]   handle - Handle value
 * @param[in]   isStart - Start flag
 * @return      None
 */
void tlkmdi_anc_btmusic_control(uint16_t handle, uint8_t isStart)
{
    tlkapi_printf(APP_LOG_EN, "tlkmdi_anc_btmusic_control is_start  %d", isStart);

    if (!isStart) {
        // audio_fifo_irq_dis(AUDIO_TX_FIFO0 + TLKDRV_ANC0_SPK_FIFO);//stop fifo irq
        s_anc_env.bt_music_busy = false;
#if TLK_MW_DSP_COMM_ENABLE
        d25f_set_dsp_anc_downlink_enable(0);
#endif
        // audio_tx_dma_dis(TLKDRV_ANC0_SPK_DMA);
        // audio_rx_dma_dis(TLKDRV_ANC0_MIC_DMA);
        // audio_clr_fifo_irq_status(AUDIO_TX_FIFO0 + TLKDRV_ANC0_SPK_FIFO);
        // audio_set_tx_rptr(TLKDRV_ANC0_SPK_FIFO, 0);

        // tlkmdi_anc_irq_deinit();
        // tlkdrv_codec_anc_disable();
        tlkmdi_btmusic_switch_out(handle);

        return;
    }

    s_anc_env.bt_music_busy = true;
    tlkmdi_btmusic_switch_in(handle);
#if TLK_MW_DSP_COMM_ENABLE
    d25f_set_dsp_anc_downlink_enable(1);
    tlkmdi_anc_enable_dma();
#endif
}

/**
 * @brief       Check if BT music is busy
 * @param       None
 * @return      true if busy, false otherwise
 */
bool tlkmdi_anc_btmusic_is_busy(void)
{
    return s_anc_env.bt_music_busy;
}

/**
 * @brief       Control voice for ANC
 * @param[in]   acl_handle - ACL handle
 * @param[in]   is_start - Start flag
 * @return      None
 */
static void tlkmdi_anc_voice_control(uint16_t acl_handle, uint8_t is_start)
{
    tlkapi_printf(APP_LOG_EN, "tlkmdi_anc_voice_control is_start  %d", is_start);

    if (is_start) {
        s_anc_env.bt_voice_busy = true;
        tlkmdi_anc_pausing();
        d25f_set_dsp_anc_downlink_enable(1);
        tlkmdi_btvoice_switch_in(acl_handle);
        bt_voice_open_codec();
    } else {
        s_anc_env.bt_voice_busy = false;
        g_anc_ctr.dsp_sync_flag = false;
        d25f_set_dsp_anc_downlink_enable(0);
        bt_voice_anc_close_codec();

        tlkmdi_btvoice_switch_out(acl_handle);
    }
}

/**
 * @brief       Control TPSLL for ANC
 * @param[in]   acl_handle - ACL handle
 * @param[in]   is_start - Start flag
 * @return      None
 */
static void tlkmdi_anc_tpsll_control(uint16_t acl_handle, uint8_t is_start)
{
    (void)acl_handle;
    tlkapi_printf(APP_LOG_EN, "tlkmdi_anc_tpsll_control is_start  %d", is_start);

    if (tlkmdi_anc_btmusic_is_busy() || tlkmdi_anc_btvoice_is_busy()) {
        return;
    }

    if (is_start) {
        s_anc_env.ll_audio_busy = true;
        tlkmdi_anc_pausing();
        d25f_set_dsp_anc_downlink_enable(1);
        ll_audio_switch_in();
    } else {
        s_anc_env.ll_audio_busy = false;
        g_anc_ctr.dsp_sync_flag = false;
        d25f_set_dsp_anc_downlink_enable(0);
        ll_audio_switch_out();
    }
}

/**
 * @brief       Check if BT voice is busy
 * @param       None
 * @return      true if busy, false otherwise
 */
bool tlkmdi_anc_btvoice_is_busy(void)
{
    return s_anc_env.bt_voice_busy;
}

/**
 * @brief       Check if LL audio is busy
 * @param       None
 * @return      true if busy, false otherwise
 */
bool tlkmdi_anc_ll_audio_is_busy(void)
{
    return s_anc_env.ll_audio_busy;
}

/**
 * @brief       Initialize ANC module
 * @param       None
 * @return      0 if success, otherwise error code
 */
int tlkmdi_anc_init(void)
{
    static const tlkmw_anc_sch_cfg_t s_tlkmw_anc_sch_cfg = {
        .cbArray =
            {
                [TLKMW_ANC_SCH_CB_TYPE_BT_MUSIC] = tlkmdi_anc_btmusic_control,
                [TLKMW_ANC_SCH_CB_TYPE_BT_VOICE] = tlkmdi_anc_voice_control,
                [TLKMW_ANC_SCH_CB_TYPE_TPSLL]    = tlkmdi_anc_tpsll_control,
            },
    };
    tlkmw_anc_sch_init(&s_tlkmw_anc_sch_cfg);

    return TLK_ENONE;
}

/**
 * @brief       Process next operation for ANC
 * @param       None
 * @return      true if success, false otherwise
 */
bool tlkmdi_anc_next(void)
{
    tlkmw_anc_sch_ui_next();
    return true;
}

/**
 * @brief       Process previous operation for ANC
 * @param       None
 * @return      true if success, false otherwise
 */
bool tlkmdi_anc_previous(void)
{
    tlkmw_anc_sch_ui_prev();
    return true;
}

/**
 * @brief       Operate ANC with specific command
 * @param[in]   handle - Handle value
 * @param[in]   opcode - Operation code
 * @param[in]   pdata - Pointer to data
 * @param[in]   dataLen - Length of data
 * @return      true if success, false otherwise
 */
bool tlkmdi_anc_operate(uint16_t handle, uint8_t opcode, uint8_t *pdata, uint16_t dataLen)
{
    (void)pdata;
    (void)dataLen;
    (void)handle;
    switch (opcode) {
    case TLKAUD_OPCODE_VOLUME_INC:
    {
        tlkmw_anc_sch_ui_volume_up();
    } break;
    case TLKAUD_OPCODE_VOLUME_DEC:
    {
        tlkmw_anc_sch_ui_volume_down();
    } break;
    case TLKAUD_OPCODE_CALL_ACCEPT:
    {
        tlkmw_anc_sch_ui_call_accept();
    } break;
    case TLKAUD_OPCODE_CALL_HUNGUP:
    {
        tlkmw_anc_sch_ui_call_reject();
    } break;
    case TLKAUD_OPCODE_IS_SUPPORT_TONE_MIX:
    {
        return true;
    }
    case TLKAUD_OPCODE_IS_CUSTOMIZED_PLAYPAUSE:
    {
        tlkmw_anc_sch_ui_playpause();
        return true;
    }
    case TLKAUD_OPCODE_TRIGGER_CUSTOMIZED_PLAYPAUSE:
    {
        return true;
    }
    default:
    {
        return false;
    } break;
    }
    return true;
}

/**
 * @brief       Check if ANC is busy
 * @param       None
 * @return      true if busy, false otherwise
 */
bool tlkmdi_anc_is_busy(void)
{
    return tlkmw_anc_sch_isRunning() != 0;
}

/**
 * @brief       Switch ANC state
 * @param[in]   handle - Handle value
 * @param[in]   status - Status value
 * @return      true if success, false otherwise
 */
bool tlkmdi_anc_switch(uint16_t handle, uint8_t status)
{
    (void)handle;

    tlkapi_trace(0xffffffff, "<test>", "tlkmdi_anc_switch: handle:%d, status: %d", handle, status);

    if (status == TLK_STATE_OPENED) {
        tlkmdi_anc_irq_init(ANC_FRAME_SIZE * sizeof(adc_mono_int));
        // tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_FIFO, NULL);

        tlkdrv_codec_init_env();
#if TLKALG_ANC_ENABLE
        tlkdrv_codec_anc_enable(DISABLE_RESAMPLE);
#endif

#if TLK_MW_DSP_COMM_ENABLE
        tlkmw_dsp_resume();
        d25f_set_dsp_anc_enable(1);
#endif
        // audio_tx_dma_en(TLKDRV_ANC0_SPK_DMA);
        // audio_rx_dma_en(TLKDRV_ANC0_MIC_DMA);
        d25f_set_dsp_anc_sync_enable();

        tlkmw_anc_sch_resume();
    } else {
        tlkmw_anc_sch_pause();
        tlkdrv_codec_close(TLKDRV_CODEC_SUBDEV_BOTH);
        audio_fifo_irq_dis(AUDIO_TX_FIFO0 + TLKDRV_ANC0_SPK_FIFO);
        // tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_FIFO, NULL);

#if TLK_MW_DSP_COMM_ENABLE
        d25f_set_dsp_anc_enable(0);
        tlkmdi_anc_request_close_dsp();
        tlkmw_dsp_pause();
#endif
        g_anc_ctr.dsp_sync_flag = false;
        audio_tx_dma_dis(TLKDRV_ANC0_SPK_DMA);
        audio_rx_dma_dis(TLKDRV_ANC0_MIC_DMA);
        audio_clr_fifo_irq_status(AUDIO_TX_FIFO0 + TLKDRV_ANC0_SPK_FIFO);
        audio_set_tx_rptr(TLKDRV_ANC0_SPK_FIFO, 0);

        tlkmdi_anc_irq_deinit();
        tlkdrv_codec_anc_disable();
    }
    return 1;
}

#endif
