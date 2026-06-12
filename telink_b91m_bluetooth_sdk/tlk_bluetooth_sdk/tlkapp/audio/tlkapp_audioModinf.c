/********************************************************************************************************
 * @file    tlkapp_audioModinf.c
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
#if (TLK_MW_AUDIO_ENABLE)
#include "tlkmw/tlkmw.h"
#include "tlkapp/tlkapp.h"

static const tlkapp_audio_modinf_t *spTlkAppAudioModinfs[TLKAUD_TYPE_MAX];

/**
 * @brief       This function gets the audio module interface by type.
 * @param[in]   optype  - audio type to be operated. Refer TLKAUD_TYPE_ENUM.
 * @return      Pointer to the audio module interface.
 */
static const tlkapp_audio_modinf_t *tlkapp_audio_getModinf(TLKAUD_TYPE_ENUM optype)
{
    if (optype >= TLKAUD_TYPE_MAX) {
        return NULL;
    }
    return spTlkAppAudioModinfs[optype];
}

/**
 * @brief       This function initializes the audio module nodes.
 * @param[in]   none.
 * @return      0 if success, otherwise fail.
 */
int tlkapp_audio_modinfNodeInit(void)
{
    for (uint32_t i = 0; i < TLKAUD_TYPE_MAX; i++) {
        if (spTlkAppAudioModinfs[i] == NULL || spTlkAppAudioModinfs[i]->Init == NULL) {
            continue;
        }
        spTlkAppAudioModinfs[i]->Init();
    }
    return TLK_ENONE;
}

/**
 * @brief       This function starts the audio module.
 * @param[in]   optype  - audio type to be operated. Refer TLKAUD_TYPE_ENUM.
 * @param[in]   handle  - the connection handle of the current audio initiator.
 * @param[in]   param   - control parameters of the current audio initiator.
 * @return      Operating results. TLK_ENONE means success, others means failure.
 */
int tlkapp_audio_modinfStart(TLKAUD_TYPE_ENUM optype, uint16_t handle, uint32_t param)
{
    const tlkapp_audio_modinf_t *pModinf;
    pModinf = tlkapp_audio_getModinf(optype);
    if (pModinf == NULL || pModinf->Start == NULL) {
        return -TLK_ENOSUPPORT;
    } else {
        return pModinf->Start(handle, param);
    }
}

/**
 * @brief       This function closes the audio module.
 * @param[in]   optype  - audio type to be operated. Refer TLKAUD_TYPE_ENUM.
 * @param[in]   handle  - the connection handle of the current audio initiator.
 * @return      Operating results. TLK_ENONE means success, others means failure.
 */
int tlkapp_audio_modinfClose(TLKAUD_TYPE_ENUM optype, uint16_t handle)
{
    const tlkapp_audio_modinf_t *pModinf;
    pModinf = tlkapp_audio_getModinf(optype);
    if (pModinf == NULL || pModinf->Close == NULL) {
        return -TLK_ENOSUPPORT;
    } else {
        return pModinf->Close(handle);
    }
}

/**
 * @brief       This function handles the timing callback for the audio module.
 * @param[in]   optype  - audio type to be operated. Refer TLKAUD_TYPE_ENUM.
 * @return      none.
 */
void tlkapp_audio_modinfTimer(TLKAUD_TYPE_ENUM optype)
{
    const tlkapp_audio_modinf_t *pModinf;
    pModinf = tlkapp_audio_getModinf(optype);
    if (pModinf != NULL && pModinf->Timer != NULL) {
        pModinf->Timer();
    }
}

/**
 * @brief       This function switches the audio module state.
 * @param[in]   optype  - audio type to be operated. Refer TLKAUD_TYPE_ENUM.
 * @param[in]   handle  - the connection handle of the current audio initiator.
 * @param[in]   status  - the state of the audio control.
 *                      TLK_STATE_OPENED -- The audio is on.
 *                      TLK_STATE_CLOSED -- The audio is off.
 *                      TLK_STATE_PAUSED -- The audio is paused.
 * @return      Operating results. True means success, others means failure.
 */
bool tlkapp_audio_modinfSwitch(TLKAUD_TYPE_ENUM optype, uint16_t handle, uint8_t status)
{
    const tlkapp_audio_modinf_t *pModinf;
    pModinf = tlkapp_audio_getModinf(optype);
    if (pModinf == NULL || pModinf->Switch == NULL) {
        return false;
    } else {
        return pModinf->Switch(handle, status);
    }
}

/**
 * @brief       This function plays the next track.
 * @param[in]   optype  - audio type to be operated. Refer TLKAUD_TYPE_ENUM.
 * @return      Operating results. true means success, others means failure.
 */
bool tlkapp_audio_modinfToNext(TLKAUD_TYPE_ENUM optype)
{
    const tlkapp_audio_modinf_t *pModinf;
    pModinf = tlkapp_audio_getModinf(optype);
    if (pModinf == NULL || pModinf->ToNext == NULL) {
        return false;
    } else {
        return pModinf->ToNext();
    }
}

/**
 * @brief       This function plays the previous track.
 * @param[in]   optype  - audio type to be operated. Refer TLKAUD_TYPE_ENUM.
 * @return      Operating results. True means success, others means failure.
 */
bool tlkapp_audio_modinfToPrev(TLKAUD_TYPE_ENUM optype)
{
    const tlkapp_audio_modinf_t *pModinf;
    pModinf = tlkapp_audio_getModinf(optype);
    if (pModinf == NULL || pModinf->ToPrev == NULL) {
        return false;
    } else {
        return pModinf->ToPrev();
    }
}

/**
 * @brief       This function checks if the audio module is busy.
 * @param[in]   optype  - audio type to be operated. Refer TLKAUD_TYPE_ENUM.
 * @return      Operating results. True means audio is busy, others means audio is idle.
 */
bool tlkapp_audio_modinfIsBusy(TLKAUD_TYPE_ENUM optype)
{
    const tlkapp_audio_modinf_t *pModinf;
    pModinf = tlkapp_audio_getModinf(optype);
    if (pModinf == NULL || pModinf->IsBusy == NULL) {
        return false;
    } else {
        return pModinf->IsBusy();
    }
}

/**
 * @brief       This function operates on the audio module with specific data.
 * @param[in]   handle  - the connection handle of the current audio initiator.
 * @param[in]   optype  - audio type to be operated. Refer TLKAUD_TYPE_ENUM.
 * @param[in]   pData   - pointer to the data.
 * @param[in]   dataLen - length of the data.
 * @return      true if success, false if failure.
 */
bool tlkapp_audio_modinfOperate(uint16_t handle, TLKAUD_TYPE_ENUM optype, uint8_t *pData, uint16_t dataLen)
{
    if (pData == NULL || dataLen < 1) {
        return false;
    }
    uint8_t                      opcode = pData[0];
    const tlkapp_audio_modinf_t *pModinf;

    pModinf = tlkapp_audio_getModinf(optype);
    if (pModinf == NULL || pModinf->operate == NULL) {
        return false;
    } else {
        return pModinf->operate(handle, opcode, pData + 1, dataLen - 1);
    }
}

#if (TLK_CFG_TONE_ENABLE)
static const tlkapp_audio_modinf_t sTlkAppdAudioToneModinf = {
    .Init    = tlkmdi_tone_init,
    .Switch  = tlkmdi_tone_switch,
    .IsBusy  = tlkmdi_tone_is_busy,
    .Start   = tlkmdi_tone_start,
    .operate = tlkmdi_tone_operate,
};
#endif

#if (TLK_CFG_A2DP_TO_BIS_ENABLE)
static const tlkapp_audio_modinf_t sTlkAppdAudioA2dptoBisModinf = {
    .Init    = tlkmdi_a2dp_to_bis_init,
    .Switch  = tlkmdi_a2dp_to_bis_switch,
    .IsBusy  = tlkmdi_a2dp_to_bis_is_busy,
    .Start   = tlkmdi_a2dp_to_bis_start,
    .Close   = tlkmdi_a2dp_to_bis_close,
    .ToNext  = tlkmdi_a2dp_to_bis_next,
    .ToPrev  = tlkmdi_a2dp_to_bis_previous,
    .operate = tlkmdi_a2dp_to_bis_operate,
};
#endif


#if (TLKBTP_CFG_A2DPSNK_ENABLE)
static const tlkapp_audio_modinf_t sTlkAppAudioCCBTMusicModinf = {
    .Init    = tlkmdi_bt_music_init,
    .Switch  = tlkmdi_bt_music_switch,
    .IsBusy  = tlkmdi_bt_music_is_busy,
    .Start   = tlkmdi_bt_music_start,
    .Close   = tlkmdi_bt_music_close,
    .ToNext  = tlkmdi_bt_music_next,
    .ToPrev  = tlkmdi_bt_music_previous,
    .operate = tlkmdi_bt_music_operate,
};
#endif

#if (TLK_CFG_HRA_ENABLE)
static const tlkapp_audio_modinf_t sTlkAppdAudioHraModinf = {
    .Init   = tlkmdi_hra_init,
    .Switch = tlkmdi_hra_switch,
    .IsBusy = tlkmdi_hra_is_busy,
};
#endif

#if (TLKMW_SIDETONE_EN)
static const tlkapp_audio_modinf_t sTlkAppdAudioSidetoneModinf = {
    .Init   = tlkmdi_sidetone_init,
    .Switch = tlkmdi_sidetone_switch,
    .IsBusy = tlkmdi_sidetone_is_busy,
};
#endif

#if (TLKMW_INTERPHONE_EN)
static const tlkapp_audio_modinf_t sTlkAppAudioInterphoneModinf = {
    .Switch  = tlkmdi_interphone_switch,
    .IsBusy  = tlkmdi_interphone_is_busy,
    .Init    = tlkmdi_interphone_init,
    .Start   = tlkmdi_interphone_start,
    .Close   = tlkmdi_interphone_close,
    .ToNext  = tlkmdi_interphone_next,
    .ToPrev  = tlkmdi_interphone_previous,
    .operate = tlkmdi_interphone_operate,
};
#endif

#if (TLKMW_RECORDING_CARD_EN)
static const tlkapp_audio_modinf_t sTlkAppAudioRecordingcardModinf = {
    .Switch = tlkmdi_recording_card_switch,
    .IsBusy = tlkmdi_recording_card_is_busy,
    .Init   = tlkmdi_recording_card_init,
};
#endif
#if (TLKALG_ANC_ENABLE)
static const tlkapp_audio_modinf_t sTlkAppAudioAncModinf = {
    .Switch  = tlkmdi_anc_switch,
    .IsBusy  = tlkmdi_anc_is_busy,
    .Init    = tlkmdi_anc_init,
    .ToNext  = tlkmdi_anc_next,
    .ToPrev  = tlkmdi_anc_previous,
    .operate = tlkmdi_anc_operate,
};
#endif

#if (TLKBTP_CFG_HFP_ENABLE)
static const tlkapp_audio_modinf_t sTlkAppAudioCCBTVoiceModinf = {
    .Init    = tlkmdi_bt_voice_init,
    .Switch  = tlkmdi_bt_voice_switch,
    .IsBusy  = tlkmdi_bt_voice_is_busy,
    .operate = tlkmdi_bt_voice_operate,
};
#endif


#if (TLKSTK_BT_TPS_ENABLE)

static const tlkapp_audio_modinf_t sTlkAppAudioTPSLLModinf = {
    .Init    = tlkmdi_tpsll_audio_init,
    .Switch  = tlkmdi_tpsll_audio_switch,
    .IsBusy  = tlkmdi_tpsll_audio_is_busy,
    .Start   = tlkmdi_tpsll_audio_start,
    .Close   = tlkmdi_tpsll_audio_close,
    .ToNext  = tlkmdi_tpsll_audio_next,
    .ToPrev  = tlkmdi_tpsll_audio_previous,
    .operate = tlkmdi_tpsll_audio_process_operate,
};

#endif
#ifdef BT_A2DP_OUT_INPUT_TYPE
#if (TLKBTP_CFG_A2DPSRC_ENABLE)
static const tlkapp_audio_modinf_t sTlkAppAudioA2dpOutModinf = {
    .Init    = tlkmdi_a2dp_out_init,
    .Switch  = tlkmdi_a2dp_out_switch,
    .Timer   = tlkmdi_a2dp_out_timer,
    .IsBusy  = tlkmdi_a2dp_out_isBusy,
    .Start   = tlkmdi_a2dp_out_start,
    .Close   = tlkmdi_a2dp_out_close,
    .ToNext  = tlkmdi_a2dp_out_next,
    .ToPrev  = tlkmdi_a2dp_out_previous,
    .operate = tlkmdi_a2dp_out_operate,
};
#endif
#endif

#if (TLK_MW_LEA_UC_MUSIC_ENABLE)
static const tlkapp_audio_modinf_t sTlkAppAudioLeaUcMusicModInf = {
    .Init    = tlkmdi_lea_uc_init,
    .Switch  = tlkmdi_lea_uc_music_switch,
    .IsBusy  = tlkmdi_lea_uc_music_is_busy,
    .Start   = tlkmdi_lea_uc_music_start,
    .Close   = tlkmdi_lea_uc_music_close,
    .ToNext  = tlkmdi_lea_uc_music_next,
    .ToPrev  = tlkmdi_lea_uc_music_previous,
    .operate = tlkmdi_lea_uc_music_operate,
};
#endif

#if (TLK_MW_LEA_UC_VOICE_ENABLE)
static const tlkapp_audio_modinf_t sTlkAppAudioLeaUcVoiceModInf = {
    .Init    = tlkmdi_lea_uc_init,
    .Switch  = tlkmdi_lea_uc_voice_switch,
    .IsBusy  = tlkmdi_lea_uc_voice_is_busy,
    .operate = tlkmdi_lea_uc_voice_operate,
};
#endif

#if (TLK_MW_LEA_US_MUSIC_ENABLE)
static const tlkapp_audio_modinf_t sTlkAppAudioLeaUsMusicModInf = {
    .Init    = tlkmdi_lea_us_init,
    .Switch  = tlkmdi_lea_us_music_switch,
    .IsBusy  = tlkmdi_lea_us_music_is_busy,
    .Start   = tlkmdi_lea_us_music_start,
    .Close   = tlkmdi_lea_us_music_close,
    .ToNext  = tlkmdi_lea_us_music_next,
    .ToPrev  = tlkmdi_lea_us_music_previous,
    .operate = tlkmdi_lea_us_music_operate,
};
#endif

#if (TLK_MW_LEA_US_VOICE_ENABLE)
static const tlkapp_audio_modinf_t sTlkAppAudioLeaUsVoiceModInf = {
    .Init    = tlkmdi_lea_us_init,
    .Switch  = tlkmdi_lea_us_voice_switch,
    .IsBusy  = tlkmdi_lea_us_voice_is_busy,
    .operate = tlkmdi_lea_us_voice_operate,
};
#endif

#if (TLK_MW_LEA_BMS_ENABLE)
static const tlkapp_audio_modinf_t sTlkAppAudioLeaBmsModInf = {
    .Init   = tlkmdi_lea_bms_init,
    .Switch = tlkmdi_lea_bms_switch,
    .IsBusy = tlkmdi_lea_bms_is_busy,
};
#endif

#if (TLK_MW_LEA_BMR_ENABLE)
static const tlkapp_audio_modinf_t sTlkAppAudioLeaBmrModInf = {
    .Init   = tlkmdi_lea_bmr_init,
    .Switch = tlkmdi_lea_bmr_switch,
    .IsBusy = tlkmdi_lea_bmr_is_busy,
};
#endif

#if (TLK_USB_UAC_ENABLE && TLKBTP_CFG_HFPAG_ENABLE)
static const tlkapp_audio_modinf_t sTlkAppAUdioU2hVoiceModInf = {
    .Init   = tlkmdi_audu2h_voice_init,
    .Switch = tlkmdi_audu2h_voice_switch,
    .Timer  = tlkmdi_audu2h_voice_timer,
    .IsBusy = tlkmdi_audu2h_voice_isBusy,
    .Start  = tlkmdi_audu2h_voice_start,
};
#endif

#if (TLK_USB_UAC_PLAYBACK_ENABLE)
static const tlkapp_audio_modinf_t sTlkAppAudioUACAudModinf = {
    .Init    = tlkmdi_auduac_music_init,
    .Switch  = tlkmdi_auduac_music_switch,
    .IsBusy  = tlkmdi_auduac_music_isBusy,
    .operate = tlkmdi_auduac_music_operate,
};

#endif

#if (TLK_MW_LL_DONGLE_MUSIC_ENABLE)

static const tlkapp_audio_modinf_t sTlkAppAudioLLDongMusicModinf = {
    .Init    = tlkmdi_ll_dongle_music_init,
    .Switch  = tlkmdi_ll_dongle_music_switch,
    .IsBusy  = tlkmdi_ll_dongle_music_is_busy,
    .Start   = tlkmdi_ll_dongle_music_start,
    .Close   = tlkmdi_ll_dongle_music_close,
    .ToNext  = tlkmdi_ll_dongle_music_next,
    .ToPrev  = tlkmdi_ll_dongle_music_previous,
    .operate = tlkmdi_ll_dongle_music_operate,
};
#endif

#if (TLK_USB_UAC_AUDIO_LOCAL_ENABLE)
static const tlkapp_audio_modinf_t sTlkAppAudioUacLocalAudModinf = {
    .Init    = tlkmdi_local_uac_audio_init,
    .Switch  = tlkmdi_local_uac_audio_switch,
    .IsBusy  = tlkmdi_local_uac_audio_isBusy,
    .Start   = tlkmdi_local_uac_audio_start,
    .Close   = tlkmdi_local_uac_audio_close,
    .ToNext  = tlkmdi_local_uac_audio_next,
    .ToPrev  = tlkmdi_local_uac_audio_prev,
    .operate = tlkmdi_local_uac_audio_operate,
};
#endif


#if (TLKMW_BT_1_TO_2_FORWARD_EN)
static const tlkapp_audio_modinf_t sTlkAppAudioBtVoiceForwardModinf = {
    .Init    = tlkmdi_bt_voice_forward_init,
    .Switch  = tlkmdi_bt_voice_forward_switch,
    .IsBusy  = tlkmdi_bt_voice_forward_isBusy,
    .operate = tlkmdi_bt_voice_forward_operate,
};
static const tlkapp_audio_modinf_t sTlkAppAudioBtMusicForwardModinf = {
    .Init    = tlkmdi_bt_music_forward_init,
    .Switch  = tlkmdi_bt_music_forward_switch,
    .IsBusy  = tlkmdi_bt_music_forward_isBusy,
    .Start   = tlkmdi_bt_music_forward_start,
    .Close   = tlkmdi_bt_music_forward_close,
    .ToNext  = tlkmdi_bt_music_forward_next,
    .ToPrev  = tlkmdi_bt_music_forward_previous,
    .operate = tlkmdi_bt_music_forward_operate,
};
#endif

static const tlkapp_audio_modinf_t *spTlkAppAudioModinfs[TLKAUD_TYPE_MAX] = {
#if (TLK_CFG_TONE_ENABLE)
    [TLKAUD_TYPE_TONE] = &sTlkAppdAudioToneModinf,
#endif


#if (TLKBTP_CFG_HFP_ENABLE)
    [TLKAUD_TYPE_CC_BT_VOICE] = &sTlkAppAudioCCBTVoiceModinf,
#endif

#if (TLKBTP_CFG_A2DPSNK_ENABLE)
    [TLKAUD_TYPE_CC_BT_MUSIC] = &sTlkAppAudioCCBTMusicModinf,
#endif


#if (TLKSTK_BT_TPS_ENABLE)
    [TLKAUD_TYPE_TPH_AUDIO] = &sTlkAppAudioTPSLLModinf,
#endif
#ifdef BT_A2DP_OUT_INPUT_TYPE
#if (TLKBTP_CFG_A2DPSRC_ENABLE)
    [TLKAUD_TYPE_A2DP_OUT] = &sTlkAppAudioA2dpOutModinf,
#endif
#endif

#if (TLK_MW_LEA_UC_MUSIC_ENABLE)
    [TLKAUD_TYPE_LEA_UC_MUSIC] = &sTlkAppAudioLeaUcMusicModInf,
#endif

#if (TLK_MW_LEA_UC_VOICE_ENABLE)
    [TLKAUD_TYPE_LEA_UC_VOICE] = &sTlkAppAudioLeaUcVoiceModInf,
#endif
#if (TLK_MW_LEA_US_MUSIC_ENABLE)
    [TLKAUD_TYPE_LEA_US_MUSIC] = &sTlkAppAudioLeaUsMusicModInf,
#endif

#if (TLK_MW_LEA_US_VOICE_ENABLE)
    [TLKAUD_TYPE_LEA_US_VOICE] = &sTlkAppAudioLeaUsVoiceModInf,
#endif

#if (TLK_MW_LEA_BMS_ENABLE)
    [TLKAUD_TYPE_LEA_BMS] = &sTlkAppAudioLeaBmsModInf,
#endif

#if (TLK_MW_LEA_BMR_ENABLE)
    [TLKAUD_TYPE_LEA_BMR] = &sTlkAppAudioLeaBmrModInf,
#endif

#if (TLK_USB_UAC_ENABLE && TLKBTP_CFG_HFPAG_ENABLE)
    [TLKAUD_TYPE_U2H_VOICE] = &sTlkAppAUdioU2hVoiceModInf,
#endif

#if (TLK_CFG_A2DP_TO_BIS_ENABLE)
    [TLKAUD_TYPE_A2DP_TO_BIS] = &sTlkAppdAudioA2dptoBisModinf,
#endif

#if (TLK_CFG_HRA_ENABLE)
    [TLKAUD_TYPE_HRA] = &sTlkAppdAudioHraModinf,
#endif

#if (TLKMW_SIDETONE_EN)
    [TLKAUD_TYPE_SIDETONE] = &sTlkAppdAudioSidetoneModinf,
#endif

#if (TLKMW_INTERPHONE_EN)
    [TLKAUD_TYPE_INTRTPHONE] = &sTlkAppAudioInterphoneModinf,
#endif

#if (TLKMW_RECORDING_CARD_EN)
    [TLKAUD_TYPE_RECORDING_CARD] = &sTlkAppAudioRecordingcardModinf,
#endif

#if (TLKALG_ANC_ENABLE)
    [TLKAUD_TYPE_ANC] = &sTlkAppAudioAncModinf,
#endif

#if (TLK_USB_UAC_PLAYBACK_ENABLE)
    [TLKAUD_TYPE_UAC_AUD] = &sTlkAppAudioUACAudModinf,
#endif

#if (TLK_MW_LL_DONGLE_MUSIC_ENABLE)
    [TLKAUD_TYPE_TPD_AUDIO] = &sTlkAppAudioLLDongMusicModinf,
#endif

#if (TLK_USB_UAC_AUDIO_LOCAL_ENABLE)
    [TLKAUD_TYPE_UAC_LOCAL_AUDIO] = &sTlkAppAudioUacLocalAudModinf,
#endif

#if (TLKMW_BT_1_TO_2_FORWARD_EN)
    [TLKAUD_TYPE_BT_VOICE_FORWARD] = &sTlkAppAudioBtVoiceForwardModinf,
    [TLKAUD_TYPE_BT_MUSIC_FORWARD] = &sTlkAppAudioBtMusicForwardModinf,
#endif

};

#endif // #if (TLK_MW_AUDIO_ENABLE)
