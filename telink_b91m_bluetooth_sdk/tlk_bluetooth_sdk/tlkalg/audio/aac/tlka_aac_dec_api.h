/********************************************************************************************************
 * @file    tlka_aac_dec_api.h
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
#ifndef TLKA_AAC_DEC_API_H
#define TLKA_AAC_DEC_API_H

/*! Construct version number from major/minor/micro values. */
#define AACDEC_VERSION_INT(major, minor, micro) (((major) << 16) | ((minor) << 8) | (micro))

/*! Version number to ensure header and binary are matching. */
#define AACDEC_VERSION AACDEC_VERSION_INT(0, 3, 3)


typedef struct AAC_DECODER_INSTANCE *HANDLE_AACDECODER; /*!< Pointer to a AAC decoder instance. */

typedef enum
{
    AAC_CHANNEL_DEFAULT = 0, /* the output channels is matched with input bitstream */
    AAC_CHANNEL_LEFT    = 1, /* only output left channel */
    AAC_CHANNEL_RIGHT   = 2, /* only output right channel */
} AAC_CHANNEL_IDX;

/* error code */
typedef enum
{
    TLKA_AAC_DEC_OK = 0,
    TLKA_AAC_DEC_FILL_ERR,
    TLKA_AAC_DEC_DECODE_ERR,
    TLKA_AAC_DEC_PARA_END
} TLKA_AAC_DEC_ERROR;

typedef struct
{
    AAC_CHANNEL_IDX chIdx;
} TLKA_AAC_DEC_CFG_PARAS;

/**
 * \brief This structure gives information about the currently decoded audio
 * data. All fields are read-only.
 */
typedef struct
{
    /* These five members are the only really relevant ones for the user. */
    int sampleRate; /*!< The sample rate in Hz of the decoded PCM audio signal. */
    int frameSize;  /*!< The frame size of the decoded PCM audio signal. */
    int channels;   /*!< The number of output audio channels */
} TLKA_AAC_DEC_INFO;

/*! Return library version number. It should match AACDEC_VERSION. */
int tlka_aac_dec_get_version(void);

/*
 * return buffer size (bytes) needed by AAC decoder
 */
int tlka_aac_dec_get_size(void);

/*
 * return scratch buffer size (bytes) needed by AAC decoder
 */
int tlka_aac_dec_get_scratch_size(void);

/*
 *  \param[in]  pBuf  pointer to a buffer which will be needed by AAC decoder
 *  \param[in]  pScratchBuf  pointer to a scratch buffer which will be needed by AAC decoder
 *  \return handle of AAC decoder
 */
HANDLE_AACDECODER tlka_aac_dec_init(TLKA_AAC_DEC_CFG_PARAS *cfgs, void *pBuf, void *pScratchBuf);

/*
 *  \param[in]  hAacDec  handle of AAC decoder
 *  \param[in]  inBuf    pointer to input buffer
 *  \param[in]  inSize   input buffer size (bytes)
 *  \param[m]   bValid   valid data in input buffer, it will be modified inside tlka_aac_dec_process_frame
 *  \param[out] outBuf   pointer to output buffer
 *  \param[in]  outSize  output buffer size
 *  \return handle of AAC decoder
 */
TLKA_AAC_DEC_ERROR tlka_aac_dec_process_frame(HANDLE_AACDECODER hAacDec, unsigned char *inBuf, int inSize, int *bValid, short *outBuf, int outSize, TLKA_AAC_DEC_INFO *pInfo);

/*
 *  \param[in]  hAacDec  handle of AAC decoder
 */
void tlka_aac_dec_free(HANDLE_AACDECODER hAacDec);

#endif
