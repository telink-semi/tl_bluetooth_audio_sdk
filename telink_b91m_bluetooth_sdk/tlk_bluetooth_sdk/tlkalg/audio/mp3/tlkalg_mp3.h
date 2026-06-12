/********************************************************************************************************
 * @file    tlkalg_mp3.h
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
#ifndef TLKALG_MP3_H
#define TLKALG_MP3_H


/*! Version number to ensure header and binary are matching. */
#define MP3D_VERSION_INT(major, minor, micro) (((major) << 16) | ((minor) << 8) | (micro))
#define MP3D_VERSION                          MP3D_VERSION_INT(1, 0, 0)

/*#define MINIMP3_ONLY_MP3*/
#define MINIMP3_ALLOW_MONO_STEREO_TRANSITION

#ifdef _WIN32
#ifndef int16_t
typedef short int16_t;
#endif

#ifndef uint16_t
typedef unsigned short uint16_t;
#endif

#ifndef int32_t
typedef int int32_t;
#endif

#ifndef uint32_t
typedef unsigned int uint32_t;
#endif

#ifndef uint64_t
typedef unsigned long long uint64_t;
#endif

#ifndef uint8_t
typedef unsigned char uint8_t;
#endif

#endif

/* return error codes */
#define MP3D_E_PARAM                  -1
#define MP3D_E_MEMORY                 -2
#define MP3D_E_IOERROR                -3
#define MP3D_E_USER                   -4 /* can be used to stop processing from callbacks without indicating specific error */
#define MP3D_E_DECODE                 -5 /* decode error which can't be safely skipped, such as sample rate, layer and channels change */

#define MINIMP3_MAX_SAMPLES_PER_FRAME (1152 * 2)

#define MINIMP3_ID3_DETECT_SIZE       (10)

typedef struct
{
    int frame_bytes;
    int frame_offset;
    int channels;
    int hz;
    int layer;
    int bitrate_kbps;
} mp3dec_frame_info_t;

typedef struct
{
#ifndef FIXED_POINT
    float mdct_overlap[2][9 * 32];
    float qmf_state[15 * 2 * 32];
#else
    int mdct_overlap_i[2][9 * 32];
    int qmf_state_i[15 * 2 * 32];
#endif // FIXED_POINT
    int           reserve;
    int           free_format_bytes;
    unsigned char header[4];
    unsigned char reserve_buf[511];
} mp3dec_t;

typedef int16_t mp3d_sample_t;

/**
 * @brief       Return library version number. It should match MP3D_VERSION.
 * @return      Library version number.
 */
int mp3d_get_version(void);

/**
 * @brief       Find the beginning position and frame length of a mp3 frame.
 * @param[in]   mp3                       Input buffer.
 * @param[in]   mp3_bytes                 Number of available bytes in input buffer.
 * @param[out]  free_format_bytes         Not used currently, set to 0 by default.
 * @param[out]  ptr_frame_bytes           Mp3 frame length.
 * @return      Offset to the beginning of a mp3 frame.
 */
int mp3d_find_frame(const uint8_t *mp3, int mp3_bytes, int *free_format_bytes, int *ptr_frame_bytes);

/**
 * @brief       Not Used.
 */
void mp3dec_skip_id3v1(const uint8_t *buf, int32_t *pbuf_size);

/**
 * @brief       Return the size of id3v2 header at the beginning of a mp3 file.
 * @param[in]   buf                       Pointer to input buffer.
 * @param[in]   buf_size                  Number of available bytes in input buffer.
 * @return      Id3v2 header size.
 */
int32_t mp3dec_skip_id3v2(const uint8_t *buf, int32_t buf_size);

/**
 * @brief       Not Used.
 */
void mp3dec_skip_id3(const uint8_t **pbuf, int32_t *pbuf_size);

/**
 * @brief       Get the size of the static buffer required by mp3 decoder.
 * @return      Size in bytes.
 */
int32_t mp3dec_Query_BufferSize();

/**
 * @brief       Get the size of the scratch buffer required by mp3 decoder.
 * @return      Size in bytes.
 */
int32_t mp3dec_Query_ScratchSize();

/**
 * @brief       Init a mp3 decoder.
 * @param[in]   pBuf        Pointer to a pre-alloced buffer. The buffer size is obtained from mp3dec_Query_BufferSize().
 * @param[in]   pScratch    Pointer to a scratch buffer. The buffer size is obtained from mp3dec_Query_ScratchSize().
 * @return      Mp3 decoder handle.
 */
mp3dec_t *mp3dec_init(uint8_t *pBuf, uint8_t *pScratch);

/**
 * @brief       Decode a mp3 frame.
 * @param[in]   dec         Mp3 decoder handle initialized by mp3dec_init().
 * @param[in]   mp3         Input buffer.
 * @param[in]   mp3_bytes   Number of available bytes in input buffer.
 * @param[out]  pcm         Output buffer.
 * @param[out]  info        Return mp3 frame information which defined in mp3dec_frame_info_t structure.
 * @return      Samples of current decoded mp3 frame.
 */
int mp3dec_decode_frame(mp3dec_t *dec, const uint8_t *mp3, int mp3_bytes, mp3d_sample_t *pcm, mp3dec_frame_info_t *info);

/**
 * @brief       Close mp3 decoder.
 * @param[in]   dec         Mp3 decoder handle initialized by mp3dec_init().
 */
void mp3dec_close(mp3dec_t *dec);


#endif //TLKALG_MP3_H
