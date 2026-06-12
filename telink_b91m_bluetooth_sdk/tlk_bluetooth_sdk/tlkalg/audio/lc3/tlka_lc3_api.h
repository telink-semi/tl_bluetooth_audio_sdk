/********************************************************************************************************
 * @file    tlka_lc3_api.h
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
#ifndef _LC3_API_H
#define _LC3_API_H

#ifndef __riscv
typedef char      Word8;
typedef short     Word16;
typedef int       Word32;
typedef long long Word64;

typedef unsigned char  UWord8;
typedef unsigned short UWord16;
typedef unsigned int   UWord32;
#else
#include <nds_math_types.h>
typedef int8_t  Word8;
typedef int16_t Word16;
typedef int32_t Word32;
typedef int64_t Word64;

typedef uint8_t  UWord8;
typedef uint16_t UWord16;
typedef uint32_t UWord32;
#endif

/*! Construct version number from major/minor/micro values. */
#define LC3_VERSION_INT(major, minor, micro) (((major) << 16) | ((minor) << 8) | (micro))
#define LC3_VERSION                          LC3_VERSION_INT(1, 3, 5)

#define LC3_MODE_10MS                        (0)
#define LC3_MODE_75MS                        (1)


#define LC3_MAX_CHANNELS                     (2)
#define LC3_MAX_SAMPLES                      (480)
#define LC3_MAX_FRAME_SIZE                   (870)

int tlka_lc3_get_version(void);

/* decoder part */
typedef enum
{
    LC3_PARA_FRAMESIZE = 0,
    LC3_PARA_OUTSIZE,
    LC3_PARA_CHANNELS,
    LC3_PARA_SAMPLERATE,
    LC3_PARA_BITRATE,
    LC3_PARA_SAMPLES,
    LC3_PARA_DELAY,
    LC3_PARA_BEC_DETECT,
    LC3_PARA_REAL_BITRATE,
    LC3_PARA_FRAME_CNT,
    LC3_PARA_NMS,
    LC3_PARA_TNS,
    LC3_PARA_END
} LC3_PARAMETER;

typedef struct _LC3_CFG_Param
{
    Word32 samplerate;
    Word16 channels;
    Word16 frame_ms;

    /* only encoder uses bitrate */
    Word32 bitrate;

    UWord8 *pScratch;

    /* defines the MDCT-PLC fadeout to zero in ms, only decoder uses plc_fadeout_in_ms */
    Word16 plc_fadeout_in_ms;
} LC3_CFG_Param;

/* decoder part */
typedef enum
{
    LC3DEC_OK            = 0,
    LC3DEC_ERROR_GETPARA = 1,
    LC3DEC_ERROR_INIT,
    LC3DEC_ERROR_INVALID_CH,
    LC3DEC_ERROR_INVALID_NMS_MODE,
    LC3DEC_ERROR_INVALID_SR,
    LC3DEC_ERROR_SHAPE,
    LC3DEC_ERROR_SETPARA,
    LC3DEC_ERROR_BITSTREAM,
    LC3DEC_ERROR_ARITHMETIC,
} LC3DEC_Error;

typedef struct LC3_DEC LC3_DEC_STRU;

LC3DEC_Error tlka_lc3dec_init(LC3_DEC_STRU *dec, LC3_CFG_Param *lc3_dec_param);
LC3DEC_Error tlka_lc3dec_decode_frame(LC3_DEC_STRU *dec, UWord8 *inbuf, UWord32 inlen, Word16 *outbuf);
LC3DEC_Error tlka_lc3dec_get_parameter(LC3_DEC_STRU *dec, LC3_PARAMETER para, Word32 *val);
LC3DEC_Error tlka_lc3dec_set_parameter(LC3_DEC_STRU *dec, LC3_PARAMETER para, Word32 *val);
LC3DEC_Error tlka_lc3dec_free(LC3_DEC_STRU *dec);
Word32       tlka_lc3dec_get_size();
Word32       tlka_lc3dec_get_scratch_size();

/* encoder part */

typedef enum
{
    LC3ENC_OK         = 0,
    LC3ENC_ERROR_INIT = 1,
    LC3ENC_ERROR_INVALID_SR,
    LC3ENC_ERROR_INVALID_BITRATE,
    LC3ENC_ERROR_GETPARA,
    LC3ENC_ERROR_SETPARA
} LC3ENC_Error;

typedef struct LC3_ENC LC3_ENC_STRU;

LC3ENC_Error tlka_lc3enc_init(LC3_ENC_STRU *enc, LC3_CFG_Param *lc3_enc_param);
LC3ENC_Error tlka_lc3enc_encode_frame(LC3_ENC_STRU *enc, Word16 *input, UWord8 *output);
LC3ENC_Error tlka_lc3enc_get_parameter(LC3_ENC_STRU *enc, LC3_PARAMETER para, Word32 *val);
LC3ENC_Error tlka_lc3enc_set_parameter(LC3_ENC_STRU *enc, LC3_PARAMETER para, Word32 *val);
LC3ENC_Error tlka_lc3enc_change_bitrate(LC3_ENC_STRU *enc, Word32 bitrate);
LC3ENC_Error tlka_lc3enc_free(LC3_ENC_STRU *enc);
Word32       tlka_lc3enc_get_size();
Word32       tlka_lc3enc_get_scratch_size();

Word32 *tlka_lc3enc_get_subband_power(LC3_ENC_STRU *enc, Word16 *pExp);

#endif


#ifdef __XTENSA__
void tlka_lc3enc_get_init(LC3_CFG_Param *lc3_enc_param);
void tlka_lc3dec_get_init(LC3_CFG_Param *lc3_dec_param);
#endif
