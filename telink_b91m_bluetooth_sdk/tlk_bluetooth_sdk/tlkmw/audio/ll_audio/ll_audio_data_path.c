/********************************************************************************************************
 * @file    ll_audio_data_path.c
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
#include "tlkmw/audio/tlkmw_audio.h"
#include "tlkmw/tpsll/tlkmdi_bt_tpsll_tws/tlkmdi_bt_tpsll_tws.h"

#if (TLKSTK_BT_TPS_ENABLE)
extern uint8_t g_tpsll_data_buff[TPSLL_ENC_BUFF_FRAME_SIZE * TPSLL_ENC_BUFF_NUM];

//extern uint8_t latency_check_flag;

uint32_t rcv_tick_diff;
uint32_t g_data_rcv_tick_last;

/**
 * @brief      Handle received audio data.
 * @param[in]  tick - Timestamp of the received packet.
 * @param[in]  rx_packet_id - ID of the received packet.
 * @param[in]  wptr - Write pointer for data storage.
 * @param[in]  p_data - Pointer to received data.
 * @param[in]  len - Length of received data.
 * @return     None.
 */
void ll_audio_audio_rx(uint32_t tick, uint8_t rx_packet_id, uint8_t wptr, uint8_t *p_data, uint8_t len)
{
    (void)len;
    async_audio_ctx.data_rcv_tick = tick;
    //async_audio_ctx.mix_tick = tick; //async_audio_ctx.stimer_irq_tick;
    async_audio_ctx.rx_packet_id = rx_packet_id;
    async_audio_ctx.wptr         = wptr;
    async_audio_ctx.mix_wptr     = wptr;

    rcv_tick_diff        = async_audio_ctx.data_rcv_tick - g_data_rcv_tick_last;
    rcv_tick_diff        = rcv_tick_diff / 24;
    g_data_rcv_tick_last = async_audio_ctx.data_rcv_tick;

#if BT_TPSLL_MIX_AUDIO_GPIO_DEBUG
    gpio_write(GPIO_PC0, 0);
    gpio_write(GPIO_PC0, 1);
    gpio_write(GPIO_PC0, 0);
#endif

    if (async_audio_ctx.stimer_ref_wptr != wptr) {
        //        gpio_write(GPIO_PB6, 0);
        //        gpio_write(GPIO_PB6, 1);
        //        gpio_write(GPIO_PB6, 0);
    }

    //async_audio_ctx.sco_num_in_bt = rx_packet_id;

#if TWS_AUDIO_PATH_GPIO_DEBUG
    gpio_write(GPIO_PC1, 0);
    gpio_write(GPIO_PC1, 1);
    gpio_write(GPIO_PC1, 0);
#endif

#ifdef SLET_ll_audio_rx
    log_tick_general(SL_LL_AUDIO_LOG_EN, SLET_ll_audio_rx);
#endif

#ifdef SL16_mix_ll_wrptr
    log_b16(SL_AUDIO_PATH_LOG_EN, SL16_mix_ll_wrptr, async_audio_ctx.mix_wptr << 8 | async_audio_ctx.mix_rptr);
#endif

    uint8_t *p_enc_buff = tpsll_get_enc_buff_by_wptr(wptr);
    //uint8_t *p_enc_buff = tpsll_get_enc_buff_ptr();

    p_enc_buff[0] = ASYNC_AUDIO_DATA_READY0;
    p_enc_buff[1] = 0;

    //    if (latency_check_flag == 1) {
    //        latency_check_flag = 0;
    //    }
    uint16_t data_len = ll_audio_get_enc_frame_length();
    tmemcpy(p_enc_buff + 2, p_data, data_len * 2);

    //tpsll_update_enc_buff_wptr();

    if (rcv_tick_diff > 15000 &&
        !(tlkmdi_bt_music_is_busy()
#if TLKALG_ANC_ENABLE
          || tlkmdi_anc_btmusic_is_busy()
#endif
              ) &&
        !(tlkmdi_bt_voice_is_busy()
#if TLKALG_ANC_ENABLE
          || tlkmdi_anc_btvoice_is_busy()
#endif
              )) {
#if TWS_AUDIO_PATH_GPIO_DEBUG
//        gpio_write(GPIO_PB0, 0);
//        gpio_write(GPIO_PB0, 1);
//        gpio_write(GPIO_PB0, 0);
#endif
    }

    ll_aud_gpio_toggle_test(wptr);
    tlksys_task_setEvtFromIsr(TLKSYS_TASKID_AUDIO, TLKSYS_TASK_EVT_AUD_MAIN);
}

/*disable by mingqian 20250704, confirm by xiaogang.*/
#if (0)
_attribute_ram_code_sec_noinline_ void ll_audio_update_sco_msg_rx_params(uint32_t tick, tph_sco_msg_pdu_header_for_host_t *p_pdu_format)
{
    uint8_t *p_src;

    if (p_pdu_format->data_format & ASYNC_AUDIO_FORMAT_16K_MASK) {
        async_audio_ctx.mix_tick = tick;
        async_audio_ctx.mix_wptr = p_pdu_format->data_wptr;
    }

    async_audio_ctx.mix_tick = clock_time();
    async_audio_ctx.wptr     = p_pdu_format->data_wptr;

    p_src = tlk_tpsll_tph_get_sco_msg_from_rxfifo(async_audio_ctx.wptr);

    if (async_audio_ctx.mix_tick && clock_time_exceed(async_audio_ctx.mix_tick, 400000)) {
        async_audio_ctx.mix_tick = 0;
        async_audio_ctx.rx_pkt   = 0x80;
        tpsll_enc_buff_init(g_tpsll_data_buff);
        tlkapi_trace(BT_AUDIO_DBG_FLAG, BT_AUDIO_DBG_SIGN, "mix reinit");
        return;
    }

    if (p_src[ASYNC_AUDIO_DATA_FLAG] & ASYNC_AUDIO_DATA_READY0) {
        uint8_t *p_enc_buff = tpsll_get_enc_buff_ptr();
        tmemcpy(p_enc_buff, p_src, TPSLL_ENC_BUFF_FRAME_SIZE);
        tpsll_update_enc_buff_wptr();

#if DSP_GPIO_DBG
        gpio_write(GPIO_PG3, 0);
        gpio_write(GPIO_PG3, 1);
        gpio_write(GPIO_PG3, 0);
#endif

#if 0
        tlkapi_printf(APP_AUDIO_LOG_EN, "wptr: %d, enc_wptr: %d, rptr:%d, avil: %d, flag:%d",
                     async_audio_ctx.wptr,
                     g_tpsll_enc_buf_ctx.tpsll_enc_buff_wptr,
                     g_tpsll_enc_buf_ctx.tpsll_enc_buff_rptr,
                     tpsll_get_enc_buff_available(),
                     p_enc_buff[ASYNC_AUDIO_DATA_FLAG] & ASYNC_AUDIO_DATA_READY0);
#endif
    }
}
#endif


#if 0

#include "tlkmw/audio/ll_audio/ll_audio_data_path.h"
#include "tlkmw/audio/ll_audio/ll_audio_main.h"
#include "stack/tpsll/tpsll.h"

/**
 * @brief  async stack post received audio data to queue
 *
 * @param[in]  p_master: async data packet
 *
 * @returns none
 */
uint8_t ll_audio_post_audio_data_to_async_queue(uint8_t *p_data, uint8_t wptr)
{
#if 0
    /* 0000 - 0110, 0 2 4 6 */
    uint8_t wptr   = p_master->cmd & 6;
    uint8_t *p_des = async.data + 128 * wptr;

    tmemcpy(p_des, p_master->data, ASYNC_AUDIO_DATA_LEN);
    tmemcpy(p_des + 128, p_master->data, ASYNC_AUDIO_DATA_LEN);

    p_des[ASYNC_AUDIO_DATA_END]       = ASYNC_AUDIO_DATA_READY0;
    p_des[ASYNC_AUDIO_DATA_END + 128] = ASYNC_AUDIO_DATA_READY0;

    async.wptr = (async.wptr & 0xf0) | wptr;

    return wptr;
#endif
    (void)p_data;
    (void)wptr;
    return 0;
}

/**
 * @brief  low latency decoder task get async audio data from queue
 *
 * @param[in]  wptr: async audio data wptr
 *
 * @returns audio data ptr
 */
uint8_t *ll_audio_get_async_queue_wptr(uint8_t wptr)
{
    (void)wptr;
    //    uint8_t *p_audio = NULL;
    //    return p_audio;
    return 0;
}
#endif
/**
 * @brief  low latency mic task post async audio data to fifo
 *
 * @param[in]  idx: mic id
 * @param[in]  p_enc: encoded audio data
 *
 * @returns none
 */
void ll_audio_post_audio_data_to_async_fifo(uint8_t idx, uint8_t *p_enc)
{
#if BT_TPSLL_MIX_AUDIO_GPIO_DEBUG
    gpio_write(GPIO_PC0, 0);
    gpio_write(GPIO_PC0, 1);
    gpio_write(GPIO_PC0, 0);
#endif

    (void)idx;
    // u32 ret = tlk_tpsll_tph_sco_msg_push_txfifo(ASYNC_AUDIO_FORMAT_LC3A, idx, p_enc + 6, LL_10MS_MIC_LC3_DATA_LEN, 0);
    /* p_enc has not header from dsp, only encode data, need to confirm. */
    if (ll_audio_get_ultra_low_latency_flag()) {
        tpsll_hci_sendMicDataCmd(1, 0, ULTRA_LL_5MS_MIC_LC3_DATA_LEN, &p_enc[0]);
    } else {
        tpsll_hci_sendMicDataCmd(1, 0, LL_10MS_MIC_LC3_DATA_LEN, &p_enc[0]);
    }

#if 0
    tlk_tpsll_tph_sco_msg_push_txfifo(ASYNC_AUDIO_FORMAT_LC3A, idx, p_enc, LL_10MS_MIC_LC3_DATA_LEN, 0);
#endif
    //  tlkapi_send_string_data(1, "push mic pkt", p_enc, 30);
    // tlkapi_send_string_data(1, "push mic idx", &idx, 1);
}

#endif
