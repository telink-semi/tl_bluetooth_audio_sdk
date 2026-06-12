/********************************************************************************************************
 * @file    tlkdrv_codec.h
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
#pragma once

#include "common/types.h"
#include "vendor/common/user_config.h"

#ifndef CODEC_CFG_TEST_MODE_EN
#define CODEC_CFG_TEST_MODE_EN 0
#endif


#if (AUDIO_PATH_24BITS_EN)
#if CODEC_DAC_MONO_MODE
typedef int32_t codec_int;
#else
typedef int64_t codec_int;
#endif

typedef int64_t adc_int;

typedef int32_t codec_mono_int;
typedef int32_t adc_mono_int;

#else

typedef int64_t adc_4ch_int;
typedef int32_t codec_int;
typedef int32_t adc_int;
typedef int16_t codec_mono_int;
typedef int16_t adc_mono_int;

#endif

#if (TLK_DEV_CODEC_ENABLE)

extern uint8_t gTlkdrvCodecSpkDmaChn;
extern uint8_t gTlkdrvCodecMicDmaChn;
#if (PROJ_RECORDING_CARD && TLKALG_BBF_ENABLE == TLKALG_BBF_6CH_EN)
extern uint8_t gTlkdrvCodecMic1DmaChn;
#endif

#define TLKDRV_CODEC_DBG_FLAG ((TLK_MAJOR_DBGID_MW << 24) | (TLK_MINOR_DBGID_MW_CODEC << 16) | TLK_DEBUG_DBG_FLAG_ALL)
#define TLKDRV_CODEC_DBG_SIGN "[CODEC]"

#ifndef CODEC_SPK_FIFO_SAMPLES
#if (TLKMW_INTERPHONE_EN)
#define CODEC_SPK_FIFO_SAMPLES 4096
#else
#if (TLKALG_AAC_DEC_ENABLE)
#define CODEC_SPK_FIFO_SAMPLES 2048
#else
#define CODEC_SPK_FIFO_SAMPLES 2048
#endif
#endif
#endif

#ifndef CODEC_MIC_FIFO_SAMPLES
#define CODEC_MIC_FIFO_SAMPLES 512
#endif

#if TLKALG_ANC_ENABLE
extern codec_int g_codec_spk_buff[CODEC_SPK_FIFO_SAMPLES + 256];
#else
extern codec_int g_codec_spk_buff[CODEC_SPK_FIFO_SAMPLES];
#endif
#if (!PROJ_RECORDING_CARD)
extern adc_int g_codec_mic_buff[CODEC_MIC_FIFO_SAMPLES];
#else
#if (TLKALG_BBF_ENABLE == TLKALG_BBF_6CH_EN)
extern _attribute_iram_data_ adc_4ch_int g_codec_mic_buff[CODEC_MIC_FIFO_SAMPLES];
extern _attribute_iram_data_ adc_int     g_codec_mic2_buff[CODEC_MIC_FIFO_SAMPLES];
#elif (TLKALG_BBF_ENABLE == TLKALG_BBF_4CH_EN)
extern _attribute_iram_data_ adc_4ch_int g_codec_mic_buff[CODEC_MIC_FIFO_SAMPLES];
#elif (TLKALG_BBF_ENABLE == TLKALG_BBF_2CH_EN)
extern adc_int g_codec_mic_buff[CODEC_MIC_FIFO_SAMPLES];
#else
extern _attribute_iram_data_ adc_mono_int g_codec_mic_buff[CODEC_MIC_FIFO_SAMPLES];
#endif
#endif

///This macro defines the codec input mode
#define CODEC_INPUT_LINEIN 1
#define CODEC_INPUT_AMIC   2
#define CODEC_INPUT_DMIC   3
#if (TLKHW_TYPE == TLKHW_TL751X_EVK_C1T368A20_V1_0 || TLKHW_TYPE == TLKHW_TL751X_CUSTOMER_OWS01 || TLKHW_TYPE == TLKHW_TL751X_EVK_C1TXA67_V1_5 || \
     TLKHW_TYPE == TLKHW_TL751X_EVK_C1T368A110_V1_0)
#if (TLK_AUDIO_TEST_MODE)
#define CODEC_INPUT_MODE CODEC_INPUT_LINEIN //CODEC_INPUT_DMIC //0:line_in; 1:AMIC; 2:DMIC
#elif (TLKMW_RECORDING_CARD_EN)
#define CODEC_INPUT_MODE CODEC_INPUT_DMIC //CODEC_INPUT_DMIC //0:line_in; 1:AMIC; 2:DMIC
#else
#define CODEC_INPUT_MODE CODEC_INPUT_AMIC //CODEC_INPUT_DMIC //0:line_in; 1:AMIC; 2:DMIC
#endif
#elif (MCU_CORE_TYPE == MCU_CORE_TL322X)
#define CODEC_INPUT_MODE CODEC_INPUT_DMIC
#elif (MCU_CORE_TYPE == MCU_CORE_TL721X)
#if PROJ_BLE_AUDIO_LL
#define CODEC_INPUT_MODE CODEC_INPUT_LINEIN
#else
#define CODEC_INPUT_MODE CODEC_INPUT_DMIC
#endif
#elif (TLKHW_TYPE == TLKHW_TL751X_EVK_C1T368A87_V1_0 || TLKHW_TYPE == TLKHW_TL751X_EVK_C1T368A87_V1_2)
#define CODEC_INPUT_MODE CODEC_INPUT_DMIC
#elif (TLKHW_TYPE == TLKHW_TL752X_96PIN || TLKHW_TYPE == TLKHW_TL752X_52PIN)
#define CODEC_INPUT_MODE CODEC_INPUT_AMIC
#else
#define CODEC_INPUT_MODE CODEC_INPUT_DMIC
#endif

#define AUDIO_SAMPLE_RATE_16K  16000
#define AUDIO_SAMPLE_RATE_24K  24000
#define AUDIO_SAMPLE_RATE_32K  32000
#define AUDIO_SAMPLE_RATE_441K 44100
#define AUDIO_SAMPLE_RATE_48K  48000

//mic group config
#define TLKDRV_CODEC_MICA 1
#define TLKDRV_CODEC_MICB 2
#ifndef TLKDRV_VOICE_MIC
#define TLKDRV_VOICE_MIC TLKDRV_CODEC_MICA
#endif
#ifndef TLKDRV_ANC_MIC
#define TLKDRV_ANC_MIC TLKDRV_CODEC_MICB
#endif
enum codec_select
{
    CODEC_INT = 0,
    CODEC_EXT = 1,
};

// ANC HARDWARE RESAMPLE
#define DISABLE_RESAMPLE FALSE
#define ENABLE_RESAMPLE  TRUE

typedef enum
{
    TLKDRV_CODEC_DEV_NONE = 0,
#if (TLKDRV_CODEC_ICODEC_ENABLE)
    TLKDRV_CODEC_DEV_INNER,
#endif
#if (TLKDRV_CODEC_I2S_SLAVE_ENABLE)
    TLKDRV_CODEC_DEV_IISSLV,
#endif
#if (TLKDRV_CODEC_I2S_MASTER_ENABLE)
    TLKDRV_CODEC_DEV_IISMST,
#endif
    TLKDRV_CODEC_DEV_MAX,
} TLKDRV_CODEC_DEV_ENUM;

typedef enum
{
    TLKDRV_CODEC_SUBDEV_DEF  = 0x00,
    TLKDRV_CODEC_SUBDEV_MIC  = 0x01,
    TLKDRV_CODEC_SUBDEV_SPK  = 0x02,
    TLKDRV_CODEC_SUBDEV_BOTH = 0x03,
} TLKDRV_CODEC_SUBDEV_ENUM;

typedef enum
{
    TLKDRV_CODEC_OPCODE_NONE = 0,
    TLKDRV_CODEC_OPCODE_SET_CHANNEL, // param0:[uint8_t]chnCnt-1/2/3, 0x01-left channel, 0x02-right channel
    TLKDRV_CODEC_OPCODE_GET_CHANNEL,
    TLKDRV_CODEC_OPCODE_SET_BIT_DEPTH, // param0:[uint8_t]bitDepth-16/20/24
    TLKDRV_CODEC_OPCODE_GET_BIT_DEPTH,
    TLKDRV_CODEC_OPCODE_SET_SAMPLE_RATE, // param0:[uint32_t]sampleRate-8k/16k/32k/44.1k/48k
    TLKDRV_CODEC_OPCODE_GET_SAMPLE_RATE,
} TLKDRV_CODEC_OPCODE_ENUM;

typedef enum
{
    TLKDRV_CODEC_CHANNEL_LEFT   = 0x01,
    TLKDRV_CODEC_CHANNEL_RIGHT  = 0x02,
    TLKDRV_CODEC_CHANNEL_STEREO = 0x03,
} TLKDRV_CODEC_CHANNEL_ENUM;

typedef enum
{
    TLKDRV_CODEC_BITDEPTH_16 = 16,
    TLKDRV_CODEC_BITDEPTH_20 = 20,
    TLKDRV_CODEC_BITDEPTH_24 = 24,
} TLKDRV_CODEC_BITDEPTH_ENUM;

typedef enum
{
    TLKDRV_CODEC_SAMPLERATE_8000  = 8000,
    TLKDRV_CODEC_SAMPLERATE_16000 = 16000,
    TLKDRV_CODEC_SAMPLERATE_24000 = 24000,
    TLKDRV_CODEC_SAMPLERATE_32000 = 32000,
    TLKDRV_CODEC_SAMPLERATE_44100 = 44100,
    TLKDRV_CODEC_SAMPLERATE_48000 = 48000,
    TLKDRV_CODEC_SAMPLERATE_96000 = 96000,
} TLKDRV_CODEC_SAMPLERATE_ENUM;

typedef struct
{
    int (*powerOn)(void);
    bool (*IsOpen)(uint8_t subDev);
    int (*Init)(uint8_t subDev);
    int (*Open)(uint8_t subDev);
    int (*Close)(uint8_t subDev);
    int (*Config)(uint8_t subDev, uint8_t opcode, uint32_t param0, uint32_t param1);
} tlkdrv_codec_modinf_t;

typedef struct
{
    uint16_t setTimeMs;
    uint16_t remainTimeMs;
    uint16_t nowstep;
    uint16_t samplesPerMs;
    uint32_t remainSamples;
} tlkdrv_codec_fade_t;

typedef struct
{
    uint8_t  spkIsMute;
    uint8_t  spkIsForceMute;
    uint8_t  majorDev;
    uint8_t  minorDev;
    uint16_t spkOffset;
    uint16_t micOffset;
#if ((TLKALG_BBF_ENABLE == TLKALG_BBF_6CH_EN) && (PROJ_RECORDING_CARD))
    uint16_t spkOffset_1;
    uint16_t micOffset_1;
#endif
    tlkdrv_codec_fade_t fadeCtrl;
} tlkdrv_codec_ctrl_t;

extern volatile int audio_hd_anc_enable;

void tlkdrv_codec_test_mode_en(uint8_t en);

uint8_t tlkdrv_codec_is_in_test_mode(void);

///ANC option
/**
 * @brief Enable ANC (Active Noise Cancellation) feature
 * @param[in] resample_enable - Enable/disable resampling
 * @return None
 */
void tlkdrv_codec_anc_enable(bool resample_enable);

/**
 * @brief Disable ANC (Active Noise Cancellation) feature
 * @param None
 * @return None
 */
void tlkdrv_codec_anc_disable(void);

///Codec option
/**
 * @brief Power on codec hardware
 * @param None
 * @return None
 */
void tlkdrv_codec_power_on(void);

/**
 * @brief Deinitialize codec hardware
 * @param None
 * @return None
 */
void tlkdrv_codec_hd_deinit(void);

/**
 * @brief Check if codec sub-device is open
 * @param[in] subDev - Codec sub-device type
 * @return True if sub-device is open, false otherwise
 */
bool tlkdrv_codec_isOpen(TLKDRV_CODEC_SUBDEV_ENUM subDev);

/**
 * @brief Initialize codec with specified device and sub-device
 * @param[in] dev - Codec device type
 * @param[in] subDev - Codec sub-device type
 * @return Operation result status
 */
int tlkdrv_codec_init(TLKDRV_CODEC_DEV_ENUM dev, TLKDRV_CODEC_SUBDEV_ENUM subDev);

/**
 * @brief Open codec sub-device
 * @param[in] subDev - Codec sub-device type
 * @return Operation result status
 */
int tlkdrv_codec_open(TLKDRV_CODEC_SUBDEV_ENUM subDev);

/**
 * @brief Close codec sub-device
 * @param[in] subDev - Codec sub-device type
 * @return Operation result status
 */
int tlkdrv_codec_close(TLKDRV_CODEC_SUBDEV_ENUM subDev);

/**
 * @brief Configure codec sub-device with specified operation
 * @param[in] subDev - Codec sub-device type
 * @param[in] opcode - Operation code
 * @param[in] param0 - First parameter for operation
 * @param[in] param1 - Second parameter for operation
 * @return Operation result status
 */
int tlkdrv_codec_config(TLKDRV_CODEC_SUBDEV_ENUM subDev, uint8_t opcode, uint32_t param0, uint32_t param1);


/**
 * @brief Enable/disable force mute for speaker
 * @param[in] en - Enable flag (1=enable, 0=disable)
 * @return Operation result status
 */
int tlkdrv_codec_forceMuteEnable(uint8_t en);

///Default option
/**
 * @brief Set codec channel
 * @param[in] channel - Channel type
 * @return Operation result status
 */
int tlkdrv_codec_setChannel(uint8_t channel);

/**
 * @brief Set codec bit depth
 * @param[in] bitDepth - Bit depth value
 * @return Operation result status
 */
int tlkdrv_codec_setBitDepth(uint8_t bitDepth);

/**
 * @brief Get codec bit depth
 * @param None
 * @return Current bit depth value
 */
int tlkdrv_codec_getBitDepth(void);

/**
 * @brief Set codec sample rate
 * @param[in] sampleRate - Sample rate value
 * @return Operation result status
 */
int tlkdrv_codec_setSampleRate(uint32_t sampleRate);

/**
 * @brief Get codec sample rate
 * @param None
 * @return Current sample rate value
 */
uint32_t tlkdrv_codec_getSampleRate(void);

/**
 * @brief Get codec channel configuration
 * @param None
 * @return Current channel configuration
 */
uint8_t tlkdrv_codec_getChannel(void);

/**
 * @brief Get speaker buffer offset
 * @param None
 * @return Speaker buffer offset in bytes
 */
uint tlkdrv_codec_getSpkOffset(void);

/**
 * @brief Get microphone buffer offset
 * @param None
 * @return Microphone buffer offset in bytes
 */
uint tlkdrv_codec_getMicOffset(void);

/**
 * @brief Set speaker buffer offset
 * @param[in] offset - Offset value in bytes
 * @return None
 */
void tlkdrv_codec_setSpkOffset(uint16_t offset);

/**
 * @brief Set microphone buffer offset
 * @param[in] offset - Offset value in bytes
 * @return None
 */
void tlkdrv_codec_setMicOffset(uint16_t offset);

/**
 * @brief Set speaker buffer
 * @param[in] pBuffer - Pointer to buffer
 * @param[in] buffLen - Buffer length in bytes
 * @return None
 */
void tlkdrv_codec_setSpkBuffer(uint8_t *pBuffer, uint16_t buffLen);

/**
 * @brief Set microphone buffer
 * @param[in] pBuffer - Pointer to buffer
 * @param[in] buffLen - Buffer length in bytes
 * @return None
 */
void tlkdrv_codec_setMicBuffer(uint8_t *pBuffer, uint16_t buffLen);

/**
 * @brief Get speaker buffer length
 * @param None
 * @return Speaker buffer length in bytes
 */
uint tlkdrv_codec_getSpkBuffLen(void);

/**
 * @brief Get microphone buffer length
 * @param None
 * @return Microphone buffer length in bytes
 */
uint tlkdrv_codec_getMicBuffLen(void);

/**
 * @brief Get speaker idle buffer length
 * @param None
 * @return Idle buffer length in bytes
 */
uint tlkdrv_codec_getSpkIdleLen(void);

/**
 * @brief Get speaker data length in buffer
 * @param None
 * @return Data length in bytes
 */
uint tlkdrv_codec_getSpkDataLen(void);

/**
 * @brief Get microphone data length in buffer
 * @param None
 * @return Data length in bytes
 */
uint tlkdrv_codec_getMicDataLen(void);
#if ((PROJ_RECORDING_CARD) && (TLKALG_BBF_ENABLE == TLKALG_BBF_6CH_EN))
uint tlkdrv_codec_getMicDataLen_1(void);
#endif

/**
 * @brief Read microphone data from buffer
 * @param[out] pBuffer - Pointer to destination buffer
 * @param[in] buffLen - Buffer length in bytes
 * @param[out] pOffset - Pointer to store offset value
 * @return True if successful, false otherwise
 */
bool tlkdrv_codec_readMicData(uint8_t *pBuffer, uint16_t buffLen, uint16_t *pOffset);
#if ((PROJ_RECORDING_CARD) && (TLKALG_BBF_ENABLE == TLKALG_BBF_6CH_EN))
void tlkdrv_codec_between_sync(int frame_len, int codec0_chn, int codec1_chn);
bool tlkdrv_codec_readMicData_1(uint8_t *pBuffer, uint16_t buffLen, uint16_t *pOffset);
#endif

/**
 * @brief Mute speaker buffer
 * @param None
 * @return None
 */
void tlkdrv_codec_muteSpkBuff(void);

/**
 * @brief Mute microphone buffer
 * @param None
 * @return None
 */
void tlkdrv_codec_muteMicBuff(void);

/**
 * @brief Fill speaker buffer with audio data
 * @param[in] pData - Pointer to audio data
 * @param[in] dataLen - Data length in bytes
 * @return True if successful, false otherwise
 */
bool tlkdrv_codec_fillSpkBuff(uint8_t *pData, uint16_t dataLen);

/**
 * @brief Read speaker data from buffer with offset
 * @param[out] pBuffer - Pointer to destination buffer
 * @param[in] buffLen - Buffer length in bytes
 * @param[in] offset - Offset value in bytes
 * @param[in] isBack - Backward flag
 * @return True if successful, false otherwise
 */
bool tlkdrv_codec_backReadSpkData(uint8_t *pBuffer, uint16_t buffLen, uint16_t offset, bool isBack);

/**
 * @brief Initialize codec environment
 * @param None
 * @return Operation result status
 */
int tlkdrv_codec_init_env(void);

/**
 * @brief Open codec with specified configuration
 * @param[in] subDev - Codec sub-device type
 * @param[in] channel - Channel configuration
 * @param[in] bitDepth - Bit depth value
 * @param[in] sampleRate - Sample rate value
 * @param[in] fadeInTimeMs - Fade in duration in milliseconds
 * @return Operation result status
 */
int tlkdrv_open_codec(TLKDRV_CODEC_SUBDEV_ENUM subDev, uint8_t channel, uint8_t bitDepth, uint32_t sampleRate, uint16_t fadeInTimeMs);

//uint8_t tlkdrv_codec_get_sin_data(int16_t *p_des, uint16_t sample_num);

/**
 * @brief Synchronize microphone samples
 * @param[in] samples - Number of samples to synchronize
 * @return Microphone offset value
 */
uint32_t tlkdrv_codec_sync_mic_samples(uint16_t samples);

/**
 * @brief Synchronize playback samples
 * @param[in] samples - Number of samples to synchronize
 * @return None
 */
void tlkdrv_codec_sync_play_samples(uint16_t samples);

/**
 * @brief Get microphone available samples count
 * @param None
 * @return Available samples count
 */
uint32_t tlkdrv_codec_get_mic_avail_samples(void);

/**
 * @brief Get speaker available samples count
 * @param None
 * @return Available samples count
 */
uint32_t tlkdrv_codec_get_spk_avail_samples(void);

/**
 * @brief Get speaker buffer idle size in samples
 * @param None
 * @return Idle buffer size in samples
 */
uint32_t tlkdrv_codec_get_spk_buf_idle_size(void);

/**
 * @brief Get speaker read pointer
 * @param None
 * @return Speaker read pointer value
 */
uint32_t tlkdrv_codec_get_speaker_rptr(void);

/**
 * @brief Get mic write pointer
 * @param None
 * @return Write pointer value
 */
uint32_t tlkdrv_codec_get_mic_wptr(void);

/**
 * @brief Synchronize speaker samples
 * @param[in] samples - Number of samples to synchronize
 * @return None
 */
void tlkdrv_codec_sync_speaker_samples(uint16_t samples);

#endif //TLK_DEV_CODEC_ENABLE
