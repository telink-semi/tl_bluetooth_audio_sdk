/********************************************************************************************************
 * @file    lc3.h
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


#ifndef __LC3_H
#define __LC3_H

#ifdef __cplusplus
extern "C"
{
#endif


#include <stdint.h>
#include <stdbool.h>

#include "lc3_private.h"

#define LC3_VERSION_INT(major, minor, micro) (((major) << 16) | ((minor) << 8) | (micro))
#define LC3_VERSION                          LC3_VERSION_INT(0, 1, 9)


#define LC3_MIN_BITRATE                      16000
#define LC3_MAX_BITRATE                      320000

#define LC3_MIN_FRAME_BYTES                  20
#define LC3_MAX_FRAME_BYTES                  400

#define LC3_MIN_FRAME_SAMPLES                __LC3_NS(7500, 8000)
#define LC3_MAX_FRAME_SAMPLES                __LC3_NS(10000, 48000)


#define LC3_CHECK_DT_US(us)                  (((us) == 7500) || ((us) == 10000))

#define LC3_CHECK_SR_HZ(sr)                  (((sr) == 8000) || ((sr) == 16000) || ((sr) == 24000) || ((sr) == 32000) || ((sr) == 48000))

    enum lc3_pcm_format
    {
        LC3_PCM_FORMAT_S16,
        LC3_PCM_FORMAT_S24,
        LC3_PCM_FORMAT_S24_3LE,
        LC3_PCM_FORMAT_FLOAT,
    };


    typedef struct lc3_encoder *lc3_encoder_t;
    typedef struct lc3_decoder *lc3_decoder_t;


    typedef LC3_ENCODER_MEM_T(10000, 16000) lc3_encoder_mem_16k_t;
    typedef LC3_ENCODER_MEM_T(10000, 48000) lc3_encoder_mem_48k_t;

    typedef LC3_DECODER_MEM_T(10000, 16000) lc3_decoder_mem_16k_t;
    typedef LC3_DECODER_MEM_T(10000, 48000) lc3_decoder_mem_48k_t;


    /**
 * @brief       Calculate the number of samples in an LC3 frame.
 * @param[in]   dt_us   - duration of the frame in microseconds.
 * @param[in]   sr_hz   - sample rate in Hz.
 * @return      Number of samples in the frame.
 */
    int lc3_frame_samples(int dt_us, int sr_hz);

    /**
 * @brief       Calculate the number of bytes in an LC3 frame.
 * @param[in]   dt_us   - duration of the frame in microseconds.
 * @param[in]   bitrate - bitrate in bits per second.
 * @return      Number of bytes in the frame.
 */
    int lc3_frame_bytes(int dt_us, int bitrate);

    /**
 * @brief       Resolve the bitrate based on the frame duration and the number of bytes in the frame.
 * @param[in]   dt_us   - duration of the frame in microseconds.
 * @param[in]   nbytes  - number of bytes in the frame.
 * @return      Resolved bitrate in bits per second.
 */
    int lc3_resolve_bitrate(int dt_us, int nbytes);

    /**
 * @brief       Calculate the encoder delay in samples.
 * @param[in]   dt_us   - duration of the frame in microseconds.
 * @param[in]   sr_hz   - sample rate in Hz.
 * @return      Encoder delay in samples.
 */
    int lc3_delay_samples(int dt_us, int sr_hz);

    /**
 * @brief       Calculate the size of the LC3 encoder memory.
 * @param[in]   dt_us   - duration of the frame in microseconds.
 * @param[in]   sr_hz   - sample rate in Hz.
 * @return      Size of the encoder memory in bytes.
 */
    unsigned int lc3_encoder_size(int dt_us, int sr_hz);

    /**
 * @brief       Setup and initialize an LC3 encoder.
 * @param[in]   dt_us       - duration of the frame in microseconds.
 * @param[in]   sr_hz       - sample rate in Hz.
 * @param[in]   sr_pcm_hz   - PCM sample rate in Hz.
 * @param[out]  mem       - pointer to the memory allocated for the encoder.
 * @return      Pointer to the initialized LC3 encoder.
 */
    lc3_encoder_t lc3_setup_encoder(int dt_us, int sr_hz, int sr_pcm_hz, void *mem);

    /**
 * @brief       Encode PCM audio data using the LC3 encoder.
 * @param[in]   encoder - LC3 encoder object.
 * @param[in]   fmt     - PCM format.
 * @param[in]   pcm     - pointer to the PCM audio data.
 * @param[in]   stride  - stride of the PCM audio data.
 * @param[in]   nbytes  - number of bytes to encode.
 * @param[out]  out     - pointer to the encoded data.
 * @return      Number of bytes written to the output.
 */
    int lc3_encode(lc3_encoder_t encoder, enum lc3_pcm_format fmt, const void *pcm, int stride, int nbytes, void *out);

    /**
 * @brief       Calculate the size of the LC3 decoder memory.
 * @param[in]   dt_us   - duration of the frame in microseconds.
 * @param[in]   sr_hz   - sample rate in Hz.
 * @return      Size of the decoder memory in bytes.
 */
    unsigned lc3_decoder_size(int dt_us, int sr_hz);

    /**
 * @brief       Setup and initialize an LC3 decoder.
 * @param[in]   dt_us       - duration of the frame in microseconds.
 * @param[in]   sr_hz       - sample rate in Hz.
 * @param[in]   sr_pcm_hz   - PCM sample rate in Hz.
 * @param[out]  mem       - pointer to the memory allocated for the decoder.
 * @return      Pointer to the initialized LC3 decoder.
 */
    lc3_decoder_t lc3_setup_decoder(int dt_us, int sr_hz, int sr_pcm_hz, void *mem);

    /**
 * @brief       Decode LC3 encoded data to PCM audio data.
 * @param[in]   decoder - LC3 decoder object.
 * @param[in]   in      - pointer to the encoded data.
 * @param[in]   nbytes  - number of bytes to decode.
 * @param[in]   fmt     - PCM format for the output.
 * @param[out]  pcm     - pointer to the buffer for the decoded PCM audio data.
 * @param[in]   stride  - stride of the PCM buffer.
 * @return      Number of bytes written to the PCM buffer.
 */
    int lc3_decode(lc3_decoder_t decoder, const void *in, int nbytes, enum lc3_pcm_format fmt, void *pcm, int stride);

#ifdef __cplusplus
}
#endif

#endif /* __LC3_H */
