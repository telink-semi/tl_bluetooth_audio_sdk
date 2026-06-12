/********************************************************************************************************
 * @file    tlkmw_audio.h
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
#ifndef TLKMW_AUDIO_H_
#define TLKMW_AUDIO_H_

#include "vendor/common/user_config.h"

#include "tlkmw/audio/bt_audio/bt_audio_main.h"
#include "tlkmw/audio/bt_audio/bt_audio_sync.h"
#include "tlkmw/audio/bt_audio/tlkmdi_bt_music.h"
#include "tlkmw/audio/bt_audio/tlkmdi_bt_voice.h"
#include "tlkmw/audio/bt_audio/music/bt_music.h"
#include "tlkmw/audio/bt_audio/music/bt_music_lhdc.h"
#include "tlkmw/audio/bt_audio/voice/bt_voice.h"
#include "tlkmw/audio/bt_audio/voice/bt_voice_forward.h"
#include "tlkmw/audio/bt_audio/music/bt_music_forward.h"

#include "tlkmw/audio/bt_audio/bt_audio_interface.h"
#include "tlkmw/audio/common/codec/tlkmw_codec.h"
#include "tlkmw/audio/common/host_interface/tlkmdi_audio_host_if.h"
#include "tlkmw/audio/common/tlkmdi_audio_debug.h"
#include "tlkmw/audio/common/tlkmdi_audio_common.h"
#include "tlkmw/audio/common/tlkmdi_audio_timer.h"
#include "tlkmw/audio/common/audio_common.h"
#include "tlkmw/audio/common/audio_control.h"
#include "tlkmw/audio/common/tlkmdi_audio.h"
#include "tlkmw/audio/common/tlkmdi_audmem.h"
#include "tlkmw/audio/common/tlkmdi_audio_codec_test.h"
#include "tlkmw/audio/common/tlkalg_audio_common.h"
#include "tlkmw/audio/bt_audio_forward/tlkmdi_bt_voice_forward.h"
#include "tlkmw/audio/bt_audio_forward/tlkmdi_bt_music_forward.h"
#if (TLK_MW_DSP_COMM_ENABLE)
#include "tlkmw/audio/common/dsp/tlkmw_audio_dsp.h"
#include "tlkmw/audio/common/dsp/ipc_service/ipc_audio.h"
#include "tlkmw/audio/common/dsp/ipc_service/ipc_msg.h"
#include "tlkmw/audio/common/dsp/msg_process/ipc_msg_process.h"
#include "tlkmw/audio/common/dsp/utils/queue.h"
#endif
#include "tlkmw/audio/dongle/tlkmdi_audu2h_voice.h"
#include "tlkmw/audio/dongle/tlkmdi_auduac_music.h"
#include "tlkmw/audio/local_uac/tlkmdi_local_uac_audio.h"

#include "tlkmw/audio/a2dp_source/tlkmdi_a2dp_out.h"

#include "tlkmw/audio/le_audio/tlkmdi_lea_uc.h"
#include "tlkmw/audio/le_audio/tlkmdi_lea_us.h"
#include "tlkmw/audio/le_audio/tlkmdi_lea_bms.h"
#include "tlkmw/audio/le_audio/tlkmdi_lea_bmr.h"
#if (TLKSTK_BT_TPS_ENABLE)
#include "tlkmw/audio/ll_audio/ll_audio_data_path.h"
#include "tlkmw/audio/ll_audio/ll_audio_main.h"
#include "tlkmw/audio/ll_audio/ll_audio.h"
#include "tlkmw/audio/ll_audio/tlkmdi_tpsll.h"
#include "stack/tpsll/tpsll.h"
#if (TLK_CFG_HRA_ENABLE)
#include "tlkmw/audio/hra_audio/tlkmdi_hra.h"
#include "tlkmw/audio/hra_audio/tlkmdi_hra_bt_audio.h"
#endif
#endif
#if TLKMW_SIDETONE_EN
#include "tlkmw/audio/sidetone/tlkmdi_sidetone.h"
#endif
#if TLKMW_INTERPHONE_EN
#include "tlkmw/audio/interphone/tlkmdi_interphone.h"
#include "tlkmw/audio/interphone/tlkmdi_interphone_handler.h"
#include "tlkmw/audio/interphone/tlkmdi_interphone_linkmgr.h"
#include "tlkmw/audio/interphone/tlkmdi_interphone_meshmgr.h"
#include "tlkmw/audio/interphone/tlkmdi_interphone_hfmgr.h"
#include "tlkmw/audio/interphone/tlkmdi_interphone_api.h"
#endif
#if TLKMW_RECORDING_CARD_EN
#include "tlkmw/audio/recording_card/tlkmdi_recording_card.h"
#include "tlkmw/audio/recording_card/tlkmdi_recording_card_handler.h"
#endif
#if 1 //TLKALG_ANC_ENABLE
#include "tlkmw/audio/anc/tlkmdi_anc.h"
#include "tlkmw/audio/anc/tlkmdi_anc_handler.h"
#endif
#if ((MCU_CORE_TYPE == MCU_CORE_TL721X) || (MCU_CORE_TYPE == MCU_CORE_TL322X))
#include "tlkmw/audio/ll_dongle/tlkmdi_ll_dongle_music.h"
#endif
#include "tlkmw/audio/tone/tlkmdi_tone.h"
#include "tlkmw/audio/tone/tone.h"
#if (TLK_CFG_A2DP_TO_BIS_ENABLE)
#include "tlkmw/audio/a2dp_to_bis/tlkmdi_a2dp_to_bis.h"
#endif
#include "tlkmw/audio/audio_mw_manager.h"

#endif /* TLKMW_AUDIO_H_ */
