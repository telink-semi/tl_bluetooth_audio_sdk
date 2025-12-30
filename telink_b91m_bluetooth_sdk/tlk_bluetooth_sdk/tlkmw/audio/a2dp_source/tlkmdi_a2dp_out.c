/********************************************************************************************************
 * @file    tlkmdi_a2dp_out.c
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
#if (TLKBTP_CFG_A2DPSRC_ENABLE)
#include "tlkmw/tlkmw.h"
#include "tlkalg/audio/audio_alg_interface.h"
#include "stack/bt/host/btp/btp_stdio.h"
#include "stack/bt/host/bth/bth_stdio.h"
#include "tlkmdi_a2dp_out.h"
#include "codec/tlkmdi_a2dp_codec_hal.h"

static tlkmdi_a2dp_out_ctrl_t sTlkA2dpOutCtrl = {0};

/**
 * @brief      This function is called to initialize the A2DP output module.
 * @param[in]  sample_rate - the sample rate of the audio data to be sent.
*/
static void tlkmdi_a2dp_out_resetParam(int sample_rate)
{
    if (sample_rate == 0) {
        sample_rate = 48000;
    }
    sample_rate /= 100;
    sTlkA2dpOutCtrl.unitTime  = (10000 * 128 * SYSTEM_TIMER_TICK_1US + sample_rate / 2) / sample_rate;
    sTlkA2dpOutCtrl.seqNumber = 1;
    sTlkA2dpOutCtrl.timeStamp = 0;
    sTlkA2dpOutCtrl.refTime   = clock_time() - 4 * sTlkA2dpOutCtrl.unitTime;
    sTlkA2dpOutCtrl.lagTime   = 0;
    sTlkA2dpOutCtrl.cacheFrm  = 0;
    sTlkA2dpOutCtrl.sndFrame  = 0;
    sTlkA2dpOutCtrl.sendFail  = false;
}

/**
 * @brief      This function is used to handle a2dp status changed event.
 * @param[in]  pData - the data of the event.
 * @param[in]  dataLen - the length of the data.
 * @return     TLK_ENONE is success, others is fail.
*/
static int tlkmdi_a2dp_out_a2dpStatusEvt(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;
    btp_a2dpStatusChangeEvt_t *pEvt;
    uint32_t                   spkSampleRate = 0;

    pEvt = (btp_a2dpStatusChangeEvt_t *)pData;

    tlk_printf("tlkmdi_a2dp_out_a2dpStatusEvt, enable[%d], status[%d], wait[%d]", sTlkA2dpOutCtrl.enable, pEvt->status, sTlkA2dpOutCtrl.waitStart);

    spkSampleRate = tlkmdi_a2dp_codec_hal_get_sampleRate();

    if (sTlkA2dpOutCtrl.enable && sTlkA2dpOutCtrl.handle != pEvt->handle) {
        tlk_printf("tlkmdi_a2dp_out_a2dpStatusEvt: other device request");
        if (pEvt->status == BTP_A2DP_STATUS_STREAM) {
            btp_a2dpsrc_suspend(pEvt->handle);
        }
        btp_avrcp_notifyPlayState(pEvt->handle, BTP_AVRCP_PLAY_STATE_PAUSED);
        btp_avrcp_setPlayState(pEvt->handle, BTP_AVRCP_PLAY_STATE_PAUSED);
        return -TLK_EFAIL;
    }

    if (pEvt->status == BTP_A2DP_STATUS_STREAM) {
        if (sTlkA2dpOutCtrl.waitFlag & TLKMDI_A2DP_OUT_WAIT_FLAG_START_RSP) {
            sTlkA2dpOutCtrl.waitFlag &= ~TLKMDI_A2DP_OUT_WAIT_FLAG_START_RSP;
        }

        uint32_t sampleRate = btp_a2dpsrc_getSampleRate(pEvt->handle);
        if (sampleRate != spkSampleRate) {
            tlkmdi_a2dp_out_close(pEvt->handle);
            return -TLK_EFAIL;
        }

        sTlkA2dpOutCtrl.frmSize = TLKMDI_A2DP_OUT_FRAME_SIZE;
        sTlkA2dpOutCtrl.pktNumb = (pEvt->mtuSize - 13) / sTlkA2dpOutCtrl.frmSize;
        if (sTlkA2dpOutCtrl.pktNumb > TLKMDI_A2DP_OUT_FRAME_MAX_NUMB) {
            sTlkA2dpOutCtrl.pktNumb = TLKMDI_A2DP_OUT_FRAME_MAX_NUMB;
        }
        sTlkA2dpOutCtrl.pktTime = 128 * sTlkA2dpOutCtrl.pktNumb;
        if (pEvt->mtuSize < 13 || sTlkA2dpOutCtrl.pktNumb == 0) {
            tlk_printf("tlkmdi_a2dp_out_a2dpStatusEvt: mtuSize[%d] is error!", pEvt->mtuSize);
            tlkmdi_a2dp_out_close(pEvt->handle);
            return -TLK_EFAIL;
        }

        tlk_printf("tlkmdi_a2dp_out_a2dpStatusEvt[START]: frmSize[%d], pktNumb[%d], pktTime[%d], sampRate[%d], localSampRate[%d]", sTlkA2dpOutCtrl.frmSize, sTlkA2dpOutCtrl.pktNumb,
                   sTlkA2dpOutCtrl.pktTime, sampleRate, tlkmdi_a2dp_codec_hal_get_sampleRate());

        uint16_t sampleRate_temp;
        int      time_temp;
        uint8_t  sbcBlock = 16;
        uint8_t  subBand  = 8;

        sTlkA2dpOutCtrl.samples_per_frame = sbcBlock * subBand;

        sampleRate_temp                  = spkSampleRate / 100;
        sTlkA2dpOutCtrl.unitTime         = (10000 * sTlkA2dpOutCtrl.samples_per_frame * SYSTEM_TIMER_TICK_1US + sampleRate_temp / 2) / sampleRate_temp;
        time_temp                        = sTlkA2dpOutCtrl.samples_per_frame * 1000 * 1000;
        sTlkA2dpOutCtrl.playtimePerFrame = time_temp / spkSampleRate; // one sbc frame play time;

        tlk_printf("tlkmdi_a2dp_out_a2dpStatusEvt[START]: time_temp[%d], unitTime[%d], perFrame[%d]", time_temp, sTlkA2dpOutCtrl.unitTime, sTlkA2dpOutCtrl.playtimePerFrame);

        if (tlkmdi_a2dp_out_isBusy()) {
            tlkmdi_a2dp_out_user_notify(true);
        }

        btp_avrcp_notifyPlayState(pEvt->handle, BTP_AVRCP_PLAY_STATE_PLAYING);
        btp_avrcp_setPlayState(pEvt->handle, BTP_AVRCP_PLAY_STATE_PLAYING);
        sTlkA2dpOutCtrl.isSuppSet = btp_avrcp_isSupportSetVolume(pEvt->handle);

        tlkmdi_a2dp_out_resetParam(spkSampleRate);

        sTlkA2dpOutCtrl.refTime = clock_time() | 1;
        sTlkA2dpOutCtrl.lagTime = 0;
        if (sTlkA2dpOutCtrl.waitStart == 0 && sTlkA2dpOutCtrl.enable) {
            tlkmdi_audio_sendStartEvt(TLKAUD_TYPE_A2DP_OUT, pEvt->handle);
        }
        sTlkA2dpOutCtrl.waitStart = 0;
        sTlkA2dpOutCtrl.waitTimer = 0;
    } else {
        tlkmdi_a2dp_out_user_notify(false);

        tlk_printf("tlkmdi_a2dp_out_a2dpStatusEvt[CLOSE]: 002");
        sTlkA2dpOutCtrl.pktNumb = 0;
        btp_avrcp_notifyPlayState(pEvt->handle, BTP_AVRCP_PLAY_STATE_PAUSED);
        btp_avrcp_setPlayState(pEvt->handle, BTP_AVRCP_PLAY_STATE_PAUSED);
        if (sTlkA2dpOutCtrl.enable) {
            tlkmdi_audio_sendCloseEvt(TLKAUD_TYPE_A2DP_OUT, sTlkA2dpOutCtrl.handle);
        }
    }
    return TLK_ENONE;
}

/**
 * @brief      This function is used to handle the reconfiguration completion event of A2DP.
 * @param[in]  pData - the data of the event.
 * @param[in]  dataLen - the length of the data.
 * @return     TLK_ENONE is success, others is fail.
*/
static int tlkmdi_a2dp_out_reConfigCompleteEvt(uint8_t *pData, uint16_t dataLen)
{
    (void)dataLen;

    btp_a2dpReconfigCompleteEvt_t *pEvt = (btp_a2dpReconfigCompleteEvt_t *)pData;
    tlk_printf("tlkmdi_a2dp_out_reConfigCompleteEvt");

    if (pEvt->status != TLK_ENONE) {
        //TODO: CX
        sTlkA2dpOutCtrl.isNeedAsrc = 1;
    }
    return TLK_ENONE;
}

/**
 * @brief      This function is used to handle the AVRCP key changed event.
 * @param[in]  aclHandle - the ACL handle.
 * @param[in]  keyID - the key ID.
 * @param[in]  isPress - the key press or release.
*/
static void tlkmdi_a2dp_out_keyChangedEvt(uint16_t aclHandle, uint8_t keyID, uint8_t isPress)
{
    tlk_printf("tlkmdi_a2dp_out_keyChangedEvt:{handle-%d,keyID-%d,isPress-%d enable-%d}", aclHandle, keyID, isPress, sTlkA2dpOutCtrl.enable);

    if (sTlkA2dpOutCtrl.enable == false) {
        return;
    }
    if (!isPress) {
        if (keyID == BTP_AVRCP_KEYID_PLAY) {
            if (!sTlkA2dpOutCtrl.enable) {
                tlkmdi_a2dp_out_start(aclHandle, 0);
            } else {
                btp_avrcp_notifyPlayState(aclHandle, BTP_AVRCP_PLAY_STATE_PAUSED);
            }
        } else if (keyID == BTP_AVRCP_KEYID_STOP || keyID == BTP_AVRCP_KEYID_PAUSE) {
            if (sTlkA2dpOutCtrl.enable) {
                tlkmdi_a2dp_out_close(aclHandle);
            } else {
                btp_avrcp_notifyPlayState(aclHandle, BTP_AVRCP_PLAY_STATE_PAUSED);
            }
        } else if (keyID == BTP_AVRCP_KEYID_FORWARD) {
            if (sTlkA2dpOutCtrl.enable) {
                tlkmdi_a2dp_out_next();
            }
        } else if (keyID == BTP_AVRCP_KEYID_BACKWARD) {
            if (sTlkA2dpOutCtrl.enable) {
                tlkmdi_a2dp_out_previous();
            }
        }
    }
}

/**
 * @brief      This function is used to actively trigger the start of music playback.
 * @param[in]  handle - the ACL handle.
 * @param[in]  param - the parameter of the start.
 * @return     TLK_ENONE is success, others is fail.
*/
int tlkmdi_a2dp_out_start(uint16_t handle, uint32_t param)
{
    (void)handle;
    (void)param;
    if (sTlkA2dpOutCtrl.enable) {
        return -TLK_EREPEAT;
    }
    tlkmdi_a2dp_out_play_pause();
    return TLK_ENONE;
}

/**
 * @brief      This function is used to actively trigger the pause of music playback.
 * @param[in]  handle - the ACL handle.
 * @param[in]  param - the parameter of the pause.
 * @return     TLK_ENONE is success, others is fail.
*/
int tlkmdi_a2dp_out_close(uint16_t handle)
{
    if (!sTlkA2dpOutCtrl.enable) {
        return -TLK_EREPEAT;
    }
    if (handle != sTlkA2dpOutCtrl.handle) {
        return -TLK_EHANDLE;
    }
    tlkmdi_a2dp_out_play_pause();
    return TLK_ENONE;
}

/**
 * @brief      This function is used to actively trigger play next.
 * @return     true is success, false is fail.
*/
bool tlkmdi_a2dp_out_next(void)
{
    tlkmdi_a2dp_out_play_next();
    return true;
}

/**
 * @brief      This function is used to actively trigger play previous.
 * @return     true is success, false is fail.
*/
bool tlkmdi_a2dp_out_previous(void)
{
    tlkmdi_a2dp_out_play_prev();
    return true;
}

/**
 * @brief      This function is used to handle specified instructions in this scenario.
 * @param[in]  handle - the ACL handle.
 * @param[in]  opcode - the operation code.
 * @param[in]  pdata  - the parameter data.
 * @param[in]  dataLen - the length of the parameter data.
 * @return     true is success, false is fail.
*/
bool tlkmdi_a2dp_out_operate(uint16_t handle, uint8_t opcode, uint8_t *pdata, uint16_t dataLen)
{
    (void)pdata;
    (void)dataLen;
    (void)handle;
    switch (opcode) {
    case TLKAUD_OPCODE_VOLUME_INC:
    {
        tlkmdi_a2dp_out_volume_up();
    } break;
    case TLKAUD_OPCODE_VOLUME_DEC:
    {
        tlkmdi_a2dp_out_volume_down();
    } break;
    default:
    {
        return false;
    } break;
    }
    return true;
}

/**
 * @brief      This function is used to malloc/free algorithm buffer.
 * @param[in]  enable - true is malloc, false is free.
 * @return     true is success, false is fail.
*/
bool tlkmdi_a2dp_out_initBuffer(bool enable)
{
    audio_alg_interface_t *p_audio_alg_if;
    if (enable) {
        if (sTlkA2dpOutCtrl.pA2dpFramesBuff == NULL) {
            sTlkA2dpOutCtrl.pA2dpFramesBuff = tlkmdi_audmem_calloc(TLKMDI_A2DP_OUT_PKT_BUFF_SIZE);
            if (sTlkA2dpOutCtrl.pA2dpFramesBuff == NULL) {
                return false;
            }
        }

        /*TODO: init by enc type: AAC or SBC*/
        if (sTlkA2dpOutCtrl.pA2dpEncBuff == NULL) {
            p_audio_alg_if = audio_alg_get_interface_by_type(ALG_SBC_ENC);
            if (p_audio_alg_if == NULL) {
                return false;
            }
            uint16_t sbc_enc_mem_size    = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
            sTlkA2dpOutCtrl.pA2dpEncBuff = (uint8_t *)tlkmdi_audmem_calloc(sbc_enc_mem_size);
            if (sTlkA2dpOutCtrl.pA2dpEncBuff == NULL) {
                return false;
            }
            p_audio_alg_if->audio_alg_init(sTlkA2dpOutCtrl.pA2dpEncBuff, ALG_CHANNEL_STEREO);
        }

#if (TLK_ALG_PPM_ENABLE && TLKALG_PPM_SPK_ENABLE)
        p_audio_alg_if = audio_alg_get_interface_by_type(ALG_PPM_SPK);
        if (p_audio_alg_if == NULL) {
            return false;
        }
        uint16_t ppm_spk_size        = p_audio_alg_if->audio_alg_get_size(ALG_CHANNEL_STEREO);
        sTlkA2dpOutCtrl.pA2dpPPMBuff = (uint8_t *)tlkalg_malloc_func(ppm_spk_size);
        if (sTlkA2dpOutCtrl.pA2dpPPMBuff == NULL) {
            return false;
        }
        p_audio_alg_if->audio_alg_init(sTlkA2dpOutCtrl.pA2dpPPMBuff, ALG_CHANNEL_STEREO);

        tlkmdi_a2dp_out_user_alg_init(enable);
#endif

    } else {
        if (sTlkA2dpOutCtrl.pA2dpFramesBuff != NULL) {
            tlkalg_free_func(sTlkA2dpOutCtrl.pA2dpFramesBuff);
            sTlkA2dpOutCtrl.pA2dpFramesBuff = NULL;
        }
        if (sTlkA2dpOutCtrl.pA2dpEncBuff != NULL) {
            p_audio_alg_if = audio_alg_get_interface_by_type(ALG_SBC_ENC);
            if (p_audio_alg_if == NULL) {
                return false;
            }
            tlkalg_free_func(sTlkA2dpOutCtrl.pA2dpEncBuff);
            p_audio_alg_if->audio_alg_deinit();
            sTlkA2dpOutCtrl.pA2dpEncBuff = NULL;
        }
#if (TLK_ALG_PPM_ENABLE && TLKALG_PPM_SPK_ENABLE)
        tlkmdi_a2dp_out_user_alg_init(enable);

        if (sTlkA2dpOutCtrl.pA2dpPPMBuff != NULL) {
            p_audio_alg_if = audio_alg_get_interface_by_type(ALG_PPM_SPK);
            if (p_audio_alg_if == NULL) {
                return false;
            }
            tlkalg_free_func(sTlkA2dpOutCtrl.pA2dpPPMBuff);
            p_audio_alg_if->audio_alg_deinit();
            sTlkA2dpOutCtrl.pA2dpPPMBuff = NULL;
        }
#endif
    }

    return true;
}

/**
 * @brief      This function is used to get the audio task is busy or not.
 * @return     true is busy, false is not busy.
*/
bool tlkmdi_a2dp_out_isBusy(void)
{
    return (sTlkA2dpOutCtrl.enable && sTlkA2dpOutCtrl.running);
}

/**
 * @brief      This function is used to get the audio task interval.
 * @return     the interval of the audio task.
*/
uint32_t tlkmdi_a2dp_out_intval(void)
{
    uint32_t intval;

    if (sTlkA2dpOutCtrl.lagTime > sTlkA2dpOutCtrl.unitTime * sTlkA2dpOutCtrl.pktNumb) {
        intval = sTlkA2dpOutCtrl.playtimePerFrame / 2;
    } else {
        intval = sTlkA2dpOutCtrl.playtimePerFrame;
    }

    return intval;
}

/**
 * @brief      This function is used to clear the asrc flag.
 * @param[in]  aclHandle - the ACL handle.
*/
void tlkmdi_a2dp_out_clearAsrcFlag(uint16_t aclHandle)
{
    if (aclHandle == sTlkA2dpOutCtrl.handle) {
        sTlkA2dpOutCtrl.isNeedAsrc = 0;
    }
}

/**
 * @brief      This function is used to encode the pcm data and send to headset.
*/
static void tlkmdi_a2dp_out_handler(void)
{
    uint32_t intv         = 0;
    uint16_t read_samples = 0;
    uint16_t need_samples = 0;
    uint32_t refTime;
    int16_t  a2dp_out_pcm_buff[256];
    uint32_t sampleRate = tlkmdi_a2dp_codec_hal_get_sampleRate();

    intv = tlkmdi_a2dp_out_intval();

    if (intv == 0) {
        intv = 3000;
    } else if (intv < 500) {
        intv = 500;
    } else if (intv > 1000000) {
        intv = 1000000;
    }

    // DBG_YATING_CHN0_HIGH;
    // DBG_YATING_CHN0_LOW;

    tlkmdi_audio_task_set_next_irq(intv);

    refTime = clock_time() | 1;

    sTlkA2dpOutCtrl.lagTime += (refTime - sTlkA2dpOutCtrl.refTime);
    sTlkA2dpOutCtrl.refTime = refTime;

    if (!sTlkA2dpOutCtrl.running || sTlkA2dpOutCtrl.pktNumb == 0) {
        sTlkA2dpOutCtrl.lagTime = 0;
        sTlkA2dpOutCtrl.refTime = clock_time() | 1;
        return;
    }

    sTlkA2dpOutCtrl.runTimer = clock_time() | 1;

    tlkmdi_a2dp_out_user_irq_handler(&refTime);

    uint16_t used_samples = tlkmdi_a2dp_out_get_used_samples();

    /*Cache 8ms data.TODO: */
    if (used_samples >= (sampleRate / 1000 * 2 * 8) && !sTlkA2dpOutCtrl.startflag) {
        sTlkA2dpOutCtrl.startflag = 1;
        sTlkA2dpOutCtrl.refTime   = clock_time() | 1;
        sTlkA2dpOutCtrl.lagTime   = 0;
    }

    if (sTlkA2dpOutCtrl.startflag) {
        /*Subbands * Blocks * Channels * Bits per Sample/8 bytes*/
        need_samples = sTlkA2dpOutCtrl.samples_per_frame * tlkmdi_a2dp_codec_hal_get_bitDepth() / 8;

        read_samples = tlkmdi_a2dp_out_read_samples(a2dp_out_pcm_buff, need_samples);

        if (read_samples == 0 || read_samples < need_samples) {
            return;
        }

        if ((sTlkA2dpOutCtrl.sndFrame < sTlkA2dpOutCtrl.pktNumb) && (read_samples >= need_samples)) {
            uint8_t *pBuffer = sTlkA2dpOutCtrl.pA2dpFramesBuff + (1 + sTlkA2dpOutCtrl.sndFrame * sTlkA2dpOutCtrl.frmSize);

            tlkmdi_a2dp_out_volume_adjust(a2dp_out_pcm_buff, read_samples);

            audio_alg_interface_t *p_audio_alg_if = audio_alg_get_interface_by_type(ALG_SBC_ENC);
            if (p_audio_alg_if == NULL) {
                return;
            }
            p_audio_alg_if->audio_alg_process((uint8_t *)a2dp_out_pcm_buff, pBuffer, read_samples * 2, 0, 0);
            sTlkA2dpOutCtrl.sndFrame++;
        }

        if (sTlkA2dpOutCtrl.sndFrame >= sTlkA2dpOutCtrl.pktNumb) {
            if (sTlkA2dpOutCtrl.lagTime > sTlkA2dpOutCtrl.unitTime * sTlkA2dpOutCtrl.pktNumb) {
                sTlkA2dpOutCtrl.cacheFrm += sTlkA2dpOutCtrl.pktNumb;
                sTlkA2dpOutCtrl.timeStamp += sTlkA2dpOutCtrl.pktNumb * sTlkA2dpOutCtrl.samples_per_frame;

                extern uint8_t tlkbt_hci_aclGetAvalSize(void);
                if (tlkbt_hci_aclGetAvalSize() < 2) {
                    sTlkA2dpOutCtrl.sendFail = true;
                } else {
                    uint16_t pktLen;
                    uint32_t timeStamp;
                    sTlkA2dpOutCtrl.lagTime -= sTlkA2dpOutCtrl.unitTime * sTlkA2dpOutCtrl.pktNumb;
                    timeStamp = sTlkA2dpOutCtrl.timeStamp - sTlkA2dpOutCtrl.pktTime - sTlkA2dpOutCtrl.cacheFrm * sTlkA2dpOutCtrl.samples_per_frame;
                    pktLen    = 1 + sTlkA2dpOutCtrl.frmSize * sTlkA2dpOutCtrl.pktNumb;
                    sTlkA2dpOutCtrl.cacheFrm -= sTlkA2dpOutCtrl.pktNumb;
                    sTlkA2dpOutCtrl.pA2dpFramesBuff[0] = sTlkA2dpOutCtrl.pktNumb;
                    if (TLK_ENONE == btp_a2dpsrc_sendMediaData(sTlkA2dpOutCtrl.handle, sTlkA2dpOutCtrl.seqNumber, timeStamp, sTlkA2dpOutCtrl.pA2dpFramesBuff, pktLen)) {
                        sTlkA2dpOutCtrl.sendFail = false;
                        sTlkA2dpOutCtrl.sndFrame = 0;
                        sTlkA2dpOutCtrl.seqNumber++;
                    } else {
                        sTlkA2dpOutCtrl.sendFail = true;
                    }
                }
            } else {
            }
        }
    }
}

/**
 * @brief      This function is used to start or stop the audio task.
 * @param[in]  handle - the ACL handle.
 * @param[in]  status - the status of the audio task.
 * @return     true is success, false is fail.
*/
bool tlkmdi_a2dp_out_switch(uint16_t handle, uint8_t status)
{
    bool enable;
    int  ret;
    if (status != TLK_STATE_OPENED && !sTlkA2dpOutCtrl.enable) {
        return true;
    }

    if (status == TLK_STATE_OPENED) {
        enable = true;
    } else {
        enable = false;
    }
    if (handle == 0) {
        enable = false;
    }

    if (!tlkmdi_a2dp_out_initBuffer(enable)) {
        return false;
    }

    sTlkA2dpOutCtrl.handle    = handle;
    sTlkA2dpOutCtrl.enable    = enable;
    sTlkA2dpOutCtrl.running   = enable;
    sTlkA2dpOutCtrl.startflag = 0;
    sTlkA2dpOutCtrl.refTime   = 0;
    sTlkA2dpOutCtrl.runTimer  = clock_time() | 1;
    sTlkA2dpOutCtrl.lagTime   = 0;
    sTlkA2dpOutCtrl.timeStamp = 0;
    sTlkA2dpOutCtrl.unitTime  = 0;
    sTlkA2dpOutCtrl.waitTimer = 0;
    sTlkA2dpOutCtrl.waitFlag  = 0;
    sTlkA2dpOutCtrl.waitStart = TLKMDI_A2DP_OUT_WAIT_START_READY;
    sTlkA2dpOutCtrl.sndFrame  = 0;
    sTlkA2dpOutCtrl.pktNumb   = 0;
    uint32_t sampleRate       = tlkmdi_a2dp_codec_hal_get_sampleRate();

    tlk_printf("tlkmdi_a2dp_out_switch enable[%d] handle[%d]", sTlkA2dpOutCtrl.enable, sTlkA2dpOutCtrl.handle);

    if (enable) {
        tlkmw_audio_btif_inform_host_audio_en(handle, true); /*Enter more data mode;*/
        tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TIMER, tlkmdi_a2dp_out_handler);
        tlkmdi_audio_setup_and_start_timer();

        btp_avrcp_notifyPlayState(sTlkA2dpOutCtrl.handle, BTP_AVRCP_PLAY_STATE_PLAYING);
        btp_avrcp_setPlayState(sTlkA2dpOutCtrl.handle, BTP_AVRCP_PLAY_STATE_PLAYING);

        if (btp_a2dpsrc_getSampleRate(handle) != sampleRate) {
            ret = btp_a2dpSrc_setSampleRate01(handle, sampleRate);
            if (ret != TLK_ENONE) {
                return false;
            }
        } else {
            ret = btp_a2dpsrc_start(sTlkA2dpOutCtrl.handle);

            if (ret != TLK_ENONE) {
                btp_avrcp_notifyPlayState(sTlkA2dpOutCtrl.handle, BTP_AVRCP_PLAY_STATE_PAUSED);
                btp_avrcp_setPlayState(sTlkA2dpOutCtrl.handle, BTP_AVRCP_PLAY_STATE_PAUSED);
                return false;
            } else {
                sTlkA2dpOutCtrl.waitFlag |= TLKMDI_A2DP_OUT_WAIT_FLAG_START_RSP;
            }
        }

        tlkmdi_a2dp_out_resetParam(sampleRate);
    } else {
        if (sTlkA2dpOutCtrl.handle != handle) {
            return -TLK_EHANDLE;
        }

        tlkmw_audio_btif_inform_host_audio_dis(handle);
        btp_avrcp_notifyPlayState(sTlkA2dpOutCtrl.handle, BTP_AVRCP_PLAY_STATE_PAUSED);
        btp_a2dpsrc_suspend(sTlkA2dpOutCtrl.handle);

        tlkmdi_audio_stop_timer();
    }

    tlkmdi_a2dp_out_user_switch(enable);

    tlk_printf("tlkmdi_a2dp_out_switch: %d %d, sndFrame[%d]", handle, status, sTlkA2dpOutCtrl.sndFrame);

    return true;
}

/**
 * @brief       This function handles A2DP output timer events, including timeout handling and sample rate configuration.
 * @return      none.
 */
void tlkmdi_a2dp_out_timer(void)
{
    //If the trigger times out, need to manually close audio, otherwise the CLOSE will trigger repeatedly
    if (sTlkA2dpOutCtrl.enable && sTlkA2dpOutCtrl.runTimer != 0 && clock_time_exceed(sTlkA2dpOutCtrl.runTimer, 3000000)) {
        sTlkA2dpOutCtrl.runTimer = clock_time() | 1;
        tlk_printf("tlkmdi_a2dp_out_timer: timeout");
        tlkmdi_a2dp_out_close(sTlkA2dpOutCtrl.handle);
        tlkmdi_a2dp_out_switch(sTlkA2dpOutCtrl.handle, TLK_STATE_CLOSED);
    }

    if (sTlkA2dpOutCtrl.waitStart == TLKMDI_A2DP_OUT_WAIT_START_SMPMIS) {
        tlk_printf("tlkmdi_a2dp_out_timer: enable set sampleRate");
        int ret = btp_a2dpsrc_setSampleRate(sTlkA2dpOutCtrl.handle, tlkmdi_a2dp_codec_hal_get_sampleRate());
        if (ret == TLK_ENONE) {
            sTlkA2dpOutCtrl.waitTimer = 0;
            sTlkA2dpOutCtrl.waitStart = TLKMDI_A2DP_OUT_WAIT_START_READY;
        } else if (ret == -TLK_EBUSY) {
            sTlkA2dpOutCtrl.waitTimer = clock_time() | 1;
            sTlkA2dpOutCtrl.waitStart = TLKMDI_A2DP_OUT_WAIT_START_BUSY;
        } else {
            tlk_printf("tlkmdi_a2dp_out_timer: disable - set sampleRate");
            sTlkA2dpOutCtrl.running   = false;
            sTlkA2dpOutCtrl.waitStart = TLKMDI_A2DP_OUT_WAIT_START_CLOSE;
            tlkmdi_a2dp_out_close(sTlkA2dpOutCtrl.handle);
            tlkmdi_a2dp_out_switch(sTlkA2dpOutCtrl.handle, TLK_STATE_CLOSED);
        }
    }
    if (sTlkA2dpOutCtrl.waitStart != TLKMDI_A2DP_OUT_WAIT_START_READY && sTlkA2dpOutCtrl.waitStart != TLKMDI_A2DP_OUT_WAIT_START_CLOSE && sTlkA2dpOutCtrl.waitTimer != 0 &&
        clock_time_exceed(sTlkA2dpOutCtrl.waitTimer, TLKMDI_A2DP_OUT_WAIT_RECFG_TIMEOUT)) {
        tlk_printf("tlkmdi_audsrc_timer: wait timeout - waitStart[%d] waitTimer[%d]", sTlkA2dpOutCtrl.waitStart, sTlkA2dpOutCtrl.waitTimer);
        sTlkA2dpOutCtrl.running   = false;
        sTlkA2dpOutCtrl.waitStart = TLKMDI_A2DP_OUT_WAIT_START_CLOSE;
        tlkmdi_a2dp_out_close(sTlkA2dpOutCtrl.handle);
        tlkmdi_a2dp_out_switch(sTlkA2dpOutCtrl.handle, TLK_STATE_CLOSED);
    }
}

BTP_EVT_REGISTER(BTP_EVTID_A2DPSRC_STATUS_CHANGED, tlkmdi_a2dp_out_a2dpStatusEvt);
BTP_EVT_REGISTER(BTP_EVTID_A2DPSRC_RECONFIG_COMPLETE, tlkmdi_a2dp_out_reConfigCompleteEvt);

/**
 * @brief   Initialize the A2DP output module
 * @note    This function initializes the A2DP output control structure and registers
 *          the AVRCP key change callback function
 * @param   None
 * @return  TLK_ENONE - Success
 */
int tlkmdi_a2dp_out_init(void)
{
    memset(&sTlkA2dpOutCtrl, 0, sizeof(tlkmdi_a2dp_out_ctrl_t));
    btp_avrcp_regKeyChangeCB(tlkmdi_a2dp_out_keyChangedEvt);

    return TLK_ENONE;
}


#endif //#if (TLKBTP_CFG_A2DPSRC_ENABLE)
