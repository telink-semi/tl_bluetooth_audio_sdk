/********************************************************************************************************
 * @file    tlkalg_aec_ns.h
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
#ifndef TLKALG_AEC_NS_H
#define TLKALG_AEC_NS_H

#define GSCON 1
#define AECON 1

/*! Version number to ensure header and binary are matching. */
#define AEC_NS_VERSION_INT(major, minor, micro) (((major) << 16) | ((minor) << 8) | (micro))
#define AEC_NS_VERSION                          AEC_NS_VERSION_INT(1, 5, 8)

#ifndef _SPEEX_TYPES_H
#define _SPEEX_TYPES_H

typedef short          spx_int16_t;
typedef unsigned short spx_uint16_t;
typedef int            spx_int32_t;
typedef unsigned int   spx_uint32_t;

#endif

typedef struct
{
    spx_int16_t use_pre_emp;   /* 1: enable pre-emphasis filter, 0: disable pre-emphasis filter */
    spx_int16_t use_dc_notch;  /* 1: enable DC removal filter, 0: disable DC removal filter */
    spx_int32_t sampling_rate; /* sample rate */
    spx_int32_t frame_size;    /* frame length, should be less than or equal to MAX_FRAME_SIZE */
    spx_int32_t filter_length; /* Number of samples of echo to cancel (should generally equal to frame_size) */
} AEC_CFG_PARAS;

typedef struct
{
    spx_int32_t sampling_rate;                /* sample rate */
    spx_int32_t frame_size;                   /* frame length, should be less than or equal to MAX_FRAME_SIZE */
    int         low_shelf_enable;             /* 1: enable lowshelf filter, 0: disable lowshelf filter */
    int         post_gain_enable;             /* 1: enable post gain processing, 0: disable post gain processing */
    int         hf_cutting_enable;            /* 1: cut frequency above 4kHz, 0: observe frequency above 4kHz */
    int         noise_suppress_default;       /* noise suppression ratio, set to -15db by default */
    int         echo_suppress_default;        /* echo suppression ratio, only work when AEC is enabled */
    int         echo_suppress_active_default; /* work with echo_suppress_default to control echo suppression floor */
    short       ns_smoothness;                /* gain smoothing factor in Q15 format */
    float       ns_threshold_low;
    int         reserved;
} NS_CFG_PARAS;

typedef enum
{
    TLKA_AEC_OK = 0,
    TLKA_AEC_INVALID_FRAMESIZE,
    TLKA_AEC_INVALID_SAMPLERATE,
    TLKA_AEC_INVALID_INPUT,
    TLKA_AEC_PARA_END
} TLKA_AEC_Error;

typedef enum
{
    TLKA_NS_OK = 0,
    TLKA_NS_INVALID_FRAMESIZE,
    TLKA_NS_INVALID_SAMPLERATE,
    TLKA_NS_INVALID_INPUT,
    TLKA_NS_PARA_END
} TLKA_NS_Error;

#define SPEEX_SAMPLERATE  (16000)
#define MAX_FRAME_SIZE    (120)
#define SPEEX_TAIL_LENGTH (MAX_FRAME_SIZE)
#define SPEEX_WINDOW_SIZE (MAX_FRAME_SIZE << 1)
#define SPEEX_M           ((SPEEX_TAIL_LENGTH + MAX_FRAME_SIZE - 1) / MAX_FRAME_SIZE)

/** Set preprocessor denoiser state */
#define SPEEX_PREPROCESS_SET_DENOISE 0
/** Get preprocessor denoiser state */
#define SPEEX_PREPROCESS_GET_DENOISE 1

/** Set preprocessor Voice Activity Detection state */
#define SPEEX_PREPROCESS_SET_VAD 4
/** Get preprocessor Voice Activity Detection state */
#define SPEEX_PREPROCESS_GET_VAD 5

/** Set preprocessor dereverb state */
#define SPEEX_PREPROCESS_SET_DEREVERB 8
/** Get preprocessor dereverb state */
#define SPEEX_PREPROCESS_GET_DEREVERB 9

/** Set preprocessor dereverb level */
#define SPEEX_PREPROCESS_SET_DEREVERB_LEVEL 10
/** Get preprocessor dereverb level */
#define SPEEX_PREPROCESS_GET_DEREVERB_LEVEL 11

/** Set preprocessor dereverb decay */
#define SPEEX_PREPROCESS_SET_DEREVERB_DECAY 12
/** Get preprocessor dereverb decay */
#define SPEEX_PREPROCESS_GET_DEREVERB_DECAY 13

/** Set probability required for the VAD to go from silence to voice */
#define SPEEX_PREPROCESS_SET_PROB_START 14
/** Get probability required for the VAD to go from silence to voice */
#define SPEEX_PREPROCESS_GET_PROB_START 15

/** Set probability required for the VAD to stay in the voice state (integer percent) */
#define SPEEX_PREPROCESS_SET_PROB_CONTINUE 16
/** Get probability required for the VAD to stay in the voice state (integer percent) */
#define SPEEX_PREPROCESS_GET_PROB_CONTINUE 17

/** Set maximum attenuation of the noise in dB (negative number) */
#define SPEEX_PREPROCESS_SET_NOISE_SUPPRESS 18
/** Get maximum attenuation of the noise in dB (negative number) */
#define SPEEX_PREPROCESS_GET_NOISE_SUPPRESS 19

/** Set maximum attenuation of the residual echo in dB (negative number) */
#define SPEEX_PREPROCESS_SET_ECHO_SUPPRESS 20
/** Get maximum attenuation of the residual echo in dB (negative number) */
#define SPEEX_PREPROCESS_GET_ECHO_SUPPRESS 21

/** Set maximum attenuation of the residual echo in dB when near end is active (negative number) */
#define SPEEX_PREPROCESS_SET_ECHO_SUPPRESS_ACTIVE 22
/** Get maximum attenuation of the residual echo in dB when near end is active (negative number) */
#define SPEEX_PREPROCESS_GET_ECHO_SUPPRESS_ACTIVE 23

/** Set the corresponding echo canceller state so that residual echo suppression can be performed (NULL for no residual echo suppression) */
#define SPEEX_PREPROCESS_SET_ECHO_STATE 24
/** Get the corresponding echo canceller state */
#define SPEEX_PREPROCESS_GET_ECHO_STATE 25

/*  Can't set spectrum size */
/** Get spectrum size for power spectrum (int32) */
#define SPEEX_PREPROCESS_GET_PSD_SIZE 37

/*  Can't set noise size */
/** Get spectrum size for noise estimate (int32)  */
#define SPEEX_PREPROCESS_GET_NOISE_PSD_SIZE 41

/*  Can't set noise estimate */
/** Get noise estimate (int32[] of squared values) */
#define SPEEX_PREPROCESS_GET_NOISE_PSD 43

/* Can't set speech probability */
/** Get speech probability in last frame (int32).  */
#define SPEEX_PREPROCESS_GET_PROB 45

/** Set threshold to implement NS */
#define SPEEX_PREPROCESS_SET_THRESHOLD_LOW  48
#define SPEEX_PREPROCESS_SET_THRESHOLD_HIGH 49
#define SPEEX_PREPROCESS_SET_FREEZE_WIN     50
#define SPEEX_PREPROCESS_SET_GAINMU         51

/** Internal echo canceller state. Should never be accessed directly. */
typedef struct SpeexEchoState_ SpeexEchoState;

/** State of the preprocessor (one per channel). Should never be accessed directly. */
typedef struct SpeexPreprocessState_ SpeexPreprocessState;

#if GSCON
/*! Version number to ensure header and binary are matching. */
// #define GSC_VERSION_INT(major, minor, micro) (((major) << 16) | ((minor) << 8) | (micro))
// #define GSC_VERSION GSC_VERSION_INT(1, 0, 0)

#define MICNUM (2)

typedef struct gscState_ gscState;

//	int gsc_get_version(void);
int gsc_get_size();
int gsc_state_init(gscState *st, int frame_size, int exchange_mic);
int gsc_BeamFormer(short *x_in, short *ref_in, short *x_out, gscState *st, int was_speech);
#endif

int tlka_aec_ns_get_version(void);

#if AECON
/** Return persistent memory size required by AEC
 */
int tlka_aec_get_size();

/** Return scratch memory size required by AEC
 */
int tlka_aec_get_scratch_size();
#endif

/** Return persistent memory size required by NS
 */
int tlka_ns_get_size();

/** Return scratch memory size required by NS
 */
int tlka_ns_get_scratch_size();

#if AECON
/** Init a new echo canceller state
 * @param state Echo canceller state
 * @param param User defined parameters, see AEC_CFG_PARAS for detail
 * @param pScratch Point to the scratch buffer which was alloced by caller
 * @return error code defined by TLKA_AEC_Error
 */
//TLKA_AEC_Error tlka_aec_init(void *state, AEC_CFG_PARAS *param, void *pScratch);
TLKA_AEC_Error tlka_aec_init(void *state, void *parameter, void *pScratch);

/** Performs echo cancellation a frame, based on the audio sent to the speaker (no delay is added
 * to playback in this form)
 *
 * @param st Echo canceller state
 * @param in Signal from the microphone (near end + far end echo)
 * @param far_end Signal played to the speaker (received from far end)
 * @param out Returns near-end signal with echo removed
 */
TLKA_AEC_Error tlka_aec_process_frame(void *st, const spx_int16_t *in, const spx_int16_t *far_end, spx_int16_t *out);
#endif

/** Init noise suppressor state. You MUST create one state per channel processed.
 * @param param User defined parameters, see NS_CFG_PARAS for detail
 * @param pScratch Point to the scratch buffer which was alloced by caller
 * @return error code defined by TLKA_NS_Error
 */
TLKA_NS_Error tlka_ns_init(void *state, void *param, void *pScratch);

/** Preprocess a frame
 * @param st Preprocessor state
 * @param x Audio sample vector (in and out). Must be same size as specified in tlka_ns_init().
 * @return error code defined by TLKA_NS_Error
 */
TLKA_NS_Error tlka_ns_process_frame(void *st, spx_int16_t *x);

/** Used like the ioctl function to control the preprocessor parameters
 * @param state Preprocessor state
 * @param request ioctl-type request (one of the SPEEX_PREPROCESS_* macros)
 * @param ptr Data exchanged to-from function
 * @return 0 if no error, -1 if request in unknown
 */
int tlka_ns_set_parameter(void *state, int request, void *ptr);

float tlka_ns_get_gain_average(SpeexPreprocessState *st);

void tlka_ns_free(void *state);


#endif //TLKALG_AEC_NS_H
