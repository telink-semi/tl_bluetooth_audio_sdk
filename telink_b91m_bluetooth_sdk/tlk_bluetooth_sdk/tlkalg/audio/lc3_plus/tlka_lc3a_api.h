/********************************************************************************************************
 * @file    tlka_lc3a_api.h
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
#ifndef TLKA_LC3A_API_H
#define TLKA_LC3A_API_H

#ifndef _MSC_VER
#include <stdint.h>
#else
typedef __int16 int16_t;
typedef __int32 int32_t;
#endif

/*! Construct version number from major/minor/micro values. */
#define LC3A_VERSION_INT(major, minor, micro) (((major) << 16) | ((minor) << 8) | (micro))


/*! Version number to ensure header and binary are matching. */
#define LC3A_VERSION LC3A_VERSION_INT(0, 0, 11)

/*! Maximum number of supported channels.
 */
#define LC3PLUS_MAX_CHANNELS 2

/*! Maximum number of samples per channel that can be stored in one LC3 frame.
 */
#define LC3PLUS_MAX_SAMPLES 960

/*! Maximum number of bytes of one LC3 frame.
 */
#define LC3PLUS_MAX_BYTES (625 * LC3PLUS_MAX_CHANNELS)

/*! Maximum scratch size available in the system.  A buffer of this size
 *  will be allocated prior to encoding to determine the maximum scratch
 *  size needed for the particular codec configuration.
 */
#define LC3PLUS_ENC_MAX_USER_SYSTEM_SCRATCH_SIZE 40000

/*! Maximum scratch size available in the system.  A buffer of this size
 *  will be allocated prior to decoding to determine the maximum scratch
 *  size needed for the particular codec configuration.
 */
#define LC3PLUS_DEC_MAX_USER_SYSTEM_SCRATCH_SIZE 40000

/*! LC3/LC3plus Frame Durations */
typedef enum
{
    LC3_FRAME_DURATION_UNDEFINED = 0, /* Invalid */
    LC3_FRAME_DURATION_2p5MS     = 2, /* 2.5 ms  */
    LC3_FRAME_DURATION_5MS       = 4, /* 5 ms    */
    LC3_FRAME_DURATION_7p5MS     = 6, /* 7.5 ms  */
    LC3_FRAME_DURATION_10MS      = 8, /* 10 ms   */
} LC3_FrameDuration;

typedef enum
{
    LC3_WAV_FORMAT_UNUSED  = 0,
    LC3_WAV_FORMAT_INT16   = 1,
    LC3_WAV_FORMAT_INT24   = 2,
    LC3_WAV_FORMAT_INT32   = 3,
    LC3_WAV_FORMAT_FLOAT32 = 4,
} LC3_WavFormat;

/*! Error protection mode. LC3PLUS_EP_ZERO differs to LC3PLUS_EP_OFF in that
 *  errors can be detected but not corrected. */
typedef enum
{
    LC3PLUS_EP_OFF    = 0, /*!< Error protection is disabled */
    LC3PLUS_EP_ZERO   = 1, /*!< Error protection with 0 bit correction */
    LC3PLUS_EP_LOW    = 2, /*!< Error protection correcting one symbol per codeword */
    LC3PLUS_EP_MEDIUM = 3, /*!< Error protection correcting two symbols per codeword */
    LC3PLUS_EP_HIGH   = 4  /*!< Error protection correcting three symbols per codeword */
} LC3PLUS_EpMode;

/*! Error codes returned by functions. */
typedef enum
{
    LC3_UNUSED = -1,                        /*!< Used for debugging purposes, not used by the library */
    LC3PLUS_OK,                             // 0
    LC3PLUS_ALIGN_ERROR,                    // 1
    LC3PLUS_ALLOC_ERROR,                    // 2
    LC3PLUS_BITRATE_ERROR,                  // 3
    LC3PLUS_BITRATE_SET_ERROR,              // 4
    LC3PLUS_BITRATE_UNSET_ERROR,            // 5
    LC3PLUS_CHANNELS_ERROR,                 // 6
    LC3PLUS_DECODE_ERROR,                   // 7
    LC3PLUS_EPMODE_ERROR,                   // 8
    LC3PLUS_EPMR_ERROR,                     // 9
    LC3PLUS_ERROR_PROTECTION_NOT_SUPPORTED, // 10
    LC3PLUS_FRAMEMS_ERROR,                  // 11
    LC3PLUS_FRAMESIZE_ERROR,                // 12
    LC3PLUS_HRMODE_ERROR,                   // 13
    LC3PLUS_LFE_MODE_NOT_SUPPORTED,         // 14
    LC3PLUS_NULL_ERROR,                     // 15
    LC3PLUS_NUMBYTES_ERROR,                 // 16
    LC3PLUS_PADDING_ERROR,                  // 17
    LC3PLUS_PLCMODE_CONF_ERROR,             // 18
    LC3PLUS_PLCMODE_ERROR,                  // 19
    LC3PLUS_RESTRICT_BT_BINARY_ERROR,       // 20
    LC3PLUS_SAMPLERATE_ERROR,               // 21
    LC3PLUS_SCRATCH_INVALID_ERROR,          // 22
    LC3PLUS_SET_BANDWIDTH_NOT_SUPPORTED,    // 23
    LC3_WAVFORMAT_ERROR,                    // 24
    LC3PLUS_WAV_FORMAT_NOT_SUPPORTED,       // 25

    /* START WARNING */
    LC3PLUS_WARNING,    // 26
    LC3PLUS_BW_WARNING, // 27
    LC3PLUS_Error_LAST, // 28

    LC3PLUS_PLC_MDCT, //29
    LC3PLUS_PLC_TDC,  //30
    LC3PLUS_PLC_ECU   //31
} LC3PLUS_ErrorCode;

#ifdef ENABLE_ERROR_PROTECTION
/*! Error protection mode request. On the encoder side, LC3PLUS_EPMR_ZERO to LC3PLUS_EPMR_HIGH
 *  can be set. The decoder returns mode requests with different confidences. */
typedef enum
{
    LC3PLUS_EPMR_ZERO      = 0,  /*!< Request no error correction. High confidence if returned by decoder. */
    LC3PLUS_EPMR_LOW       = 1,  /*!< Request low error correction. High confidence if returned by decoder. */
    LC3PLUS_EPMR_MEDIUM    = 2,  /*!< Request medium error correction. High confidence if returned by decoder. */
    LC3PLUS_EPMR_HIGH      = 3,  /*!< Request high error correction. High confidence if returned by decoder. */
    LC3PLUS_EPMR_ZERO_MC   = 4,  /*!< No error correction requested, medium confidence. */
    LC3PLUS_EPMR_LOW_MC    = 5,  /*!< Low error correction requested, medium confidence. */
    LC3PLUS_EPMR_MEDIUM_MC = 6,  /*!< Medium error correction requested, medium confidence. */
    LC3PLUS_EPMR_HIGH_MC   = 7,  /*!< High error correction requested, medium confidence. */
    LC3PLUS_EPMR_ZERO_NC   = 8,  /*!< No error correction requested, unvalidated. */
    LC3PLUS_EPMR_LOW_NC    = 9,  /*!< Low error correction requested, unvalidated. */
    LC3PLUS_EPMR_MEDIUM_NC = 10, /*!< Medium error correction requested, unvalidated. */
    LC3PLUS_EPMR_HIGH_NC   = 11  /*!< High error correction requested, unvalidated. */
} LC3PLUS_EpModeRequest;
#endif


typedef struct LC3PLUS_Enc LC3PLUS_Enc; /*!< Opaque encoder struct. */
typedef struct LC3PLUS_Dec LC3PLUS_Dec; /*!< Opaque decoder struct. */


/*! Encoder library version number.
 *
 *  The return value should should match LC3PLUS_VERSION.
 *
 *  \return                     encoder library version number.
*/
int tlka_lc3a_get_version(void);

/*************** Encoder API *****************/
typedef struct _LC3P_ENC_CFG_Param
{
    int32_t           samplerate;
    int32_t           channels;
    LC3_FrameDuration frame_dms;
    int32_t           hrmode;
    LC3_WavFormat     wavFormat_in;
    int32_t           lfe;
    int32_t           bitrate;
} LC3P_ENC_CFG_Param;

/*! Get size of the LC3 encoder struct buffer.
 *
 *  The provided size is required by lc3plus_enc_init() for a dedicated
 *  samplerate / channel configuration.
 *
 *  Not all configurations may be enabled. For unsupported
 *  configurations an appropriate error code is returned.
 *
 *  \param[in]  samplerate      Input sample rate. Allowed sample
 *                              rates are: 8000, 16000, 24000, 32000,
 *                              44100, 48000, 96000.
 *  \param[in]  channels        Input number of channels.
 *  \param[in]  wavFormat_in    waveform format of input signal
 *                              (see LC3_BitDepth in lc3plus_param.h).
 *  \return		 size           Encoder struct size in bytes.
 *  \return                     LC3PLUS_OK on success or appropriate error
 *                              code.
 */
int tlka_lc3a_enc_get_size(int32_t samplerate, int32_t channels, LC3_WavFormat wavFormat_in);

/*! Get size of the LC3 encoder scratch buffer.
 *
 *  The provided size is required by lc3plus_enc_init() for a dedicated
 *  samplerate / channel configuration.
 *
 *  Not all configurations may be enabled. For unsupported
 *  configurations an appropriate error code is returned.
 *
 *  \param[out] encoder         Pointer to encoder memory. It must have
 *                              a size of at least the number of bytes
 *                              determined by lc3_enc_get_sizes(), being
 *                              called with the same samplerate /
 *  \param[out] size            Encoder scratch size in bytes.
 *
 */
int tlka_lc3a_enc_get_scratch_size(LC3PLUS_Enc *encoder);

/*! Initialize LC3 encoder.
 *
 *  This function is used to fill a user-allocated encoder struct. This
 *  is typically called once for a samplerate / channel / frame_dms /
 *  hrmode / wavFormat_in / bitrate configuration.
 *
 *  Not all configurations may be enabled. For unsupported
 *  configurations an appropriate error code is returned.
 *
 *  After init and before encoding the first frame
 *  I might be called later again to change the bitrate.
 *
 *  \param[out] encoder         Pointer to encoder memory. It must have
 *                              a size of at least the number of bytes
 *                              determined by lc3_enc_get_sizes(), being
 *                              called with the same samplerate /
 *                              channels configuration.
 *  \param[in]  samplerate      Input sample rate. Allowed sample
 *                              rates in Hz are: 8000, 16000, 24000, 32000,
 *                              44100, 48000, 96000.
 *  \param[in]  channels        Input number of channels.
 *  \param[in]  frame_dms       Frame duration in dms (100, 75, 50 or 25).
 *  \param[in]  hrmode          High resolution mode:
 *                              0    : disabled
 *                              else : enabled
 *                              This mode is mandatory for 96 kHz input
 *                              and can also be used for 48 kHz input.
 *  \param[in]  wavFormat_in    waveform format of input signal
 *                              (see LC3_BitDepth in lc3plus_param.h).
 *  \param[in]  lfe_channel_array Array of lfe flags for all audio channels of the input waveform (deactivates LTPF, TNS, NF):
 *                              0    : disabled
 *                              else : enabled
 *
 *  \param[in]  bitrate
 *
 *  \return                     LC3PLUS_OK on success or appropriate error code.
 */
LC3PLUS_ErrorCode tlka_lc3a_enc_init(LC3PLUS_Enc *encoder, LC3P_ENC_CFG_Param *lc3p_enc_param);

/*! Encoder delay.
 *
 *  The encoder delay in number of samples is provided.
 *
 *  \param[in]  encoder     	Encoder handle.
 *  \return 	delay           Encoder delay in samples.
 */
int tlka_lc3a_enc_get_delay(LC3PLUS_Enc *encoder);

/*! Get number of samples.

 *  The number of samples per channel expected by lc3plus_enc_process_frame()
 *  is provided.
 *
 *  \param[in]  encoder     	Encoder handle.
 *  \return 	input_samples   Number of samples.
 */
int tlka_lc3a_enc_get_input_samples(LC3PLUS_Enc *encoder);

/*! Get real internal bitrate of the encoder.
 *
 *  It might differ from the requested bitrate to match an integer frame
 *  size (bitrate is rounded down to match next lower integer frame
 *  size), to error protection or 44.1 kHz input.
 *
 *  \param[in]  encoder         Encoder handle.
 *  \return		real_bitrate    Bitrate in bits per second.
 */
int tlka_lc3a_enc_get_real_bitrate(LC3PLUS_Enc *encoder);

/*! Encode an LC3 frame.
 *
 *  lc3plus_enc_init() and lc3plus_enc_set_bitrate() must be called before
 *  calling this function.
 *
 *  Each call consumes a fixed number of samples. The number of input
 *  samples can be obtained from lc3plus_enc_get_input_samples().
 *
 *  The data format of the input samples is expected to be either
 *      + 16-bit fixed point values
 *      + 24-bit fixed point values, sign-extended to 32-bit
 *        (the sign extension of the 24-bit fixed point values is
 *         assured by copying the sign (bit 32) to bits 31-24)
 *      + 32-bit floating point values within a range of [-1,+1]
 *        (absolute values larger than 1.0 are clipped)
 *
 *  The data format is configured with lc3plus_enc_init().
 *
 *  \param[in]  encoder         Encoder handle initialized by
 *                              lc3plus_enc_init().
 *  \param[in] input_samples    Array of pointers to input channel
 *                              buffers.  Each channel buffer should
 *                              provide the number of samples as given
 *                              by lc3plus_enc_get_input_samples(). The left
 *                              channel is stored in input_samples[0],
 *                              the right channel in input_samples[1].
 *                              Due to potential in-place clipping
 *                              operations, the buffer must be writable.
 *                              Minimum expected alignment:
 *                              (wavFormat_in==LC3_BIT_DEPTH_INT16) ? 16 : 32 bits).
 *  \param[out] output_bytes    Output buffer. It must provide at least
 *                              lc3plus_enc_get_num_bytes().
 *                              Minimum expected alignment: 8-bit.
 *  \param[out] num_bytes       Number of bytes written to output_bytes.
 *  \param      scratch         A pointer to an allocated work buffer of
 *                              at least the number of bytes determined
 *                              by lc3plus_enc_init(). The buffer does not
 *                              have to persist, so it can be used for
 *                              other purposes in between calls.
 *  \return                     LC3PLUS_OK on success or appropriate error
 *                              code.
 */
LC3PLUS_ErrorCode tlka_lc3a_enc_process_frame(LC3PLUS_Enc *encoder, void **input_samples, void *output_bytes, int32_t *num_bytes, void *scratch);

/*! Set encoder bitrate for all channels.
 *
 *  This function must be called at least once before encoding the first
 *  frame. It might be called again later to change the bitrate.
 *
 *  \param[in]  encoder         Encoder handle.
 *  \param[in]  bitrate         Bitrate in bits per second.
 *  \return                     LC3PLUS_OK on success or appropriate error
 *                              code.
 */
LC3PLUS_ErrorCode tlka_lc3plus_enc_set_bitrate(LC3PLUS_Enc *const encoder, int32_t const bitrate);

/*************** Decoder API *****************/
/*! Decoder packet loss concealment mode */
typedef enum
{
    LC3PLUS_PLCMETH_STD,              /*!< Noise Substitution as specified in Bluetooth */
    LC3PLUS_PLCMETH_ADV_PHECU_TDC_NS, /*!< Enhanced concealment as specified in ETSI */
    LC3PLUS_PLCMETH_ADV_PHECU,        /*!< Phase ECU */
    LC3PLUS_PLCMETH_ADV_TDC_NS,       /*!< Time Domain Concealment + Advanced Noise Substitution */
    LC3PLUS_PLCMETH_ADV_NS,           /*!< Advanced Noise Substitution */
    LC3PLUS_PLCMETH_MUTING,           /*!< Muting */
    LC3PLUS_PLCMETH_MAXVAL,           /*!< max value */
    LC3PLUS_PLCMETH_UNSUPPORTED       /*!< unsupported marker */
} LC3PLUS_PlcMethod;

typedef struct _LC3P_DEC_CFG_Param
{
    int32_t           samplerate;
    int32_t           channels;
    LC3PLUS_PlcMethod plc_method;
    LC3_FrameDuration frame_dms;
    int32_t           hrmode;
    LC3_WavFormat     wavFormat_out;
} LC3P_DEC_CFG_Param;

/*! Get size of the LC3 decoder struct buffer
 *
 *  The provided size is required by lc3plus_dec_init() for a dedicated
 *  samplerate / channel / plc mode configuration.
 *
 *  Not all configurations may be enabled. For unsupported
 *  configurations an appropriate error code is returned.
 *
 *  \param[in] samplerate       Bitstream sample rate. Allowed
 *                              sample rates are: 8000, 16000, 24000,
 *                              32000, 44100, 48000, 96000.
 *  \param[in]  channels        Bitstream number of channels.
 *  \param[in]  plc_method      Packet loss concealment method.
 *  \param[in]  frame_dms       Frame duration in dms (100, 75, 50 or 25).
 *  \param[in]  hrmode          High resolution mode:
 *                              0    : disabled
 *                              else : enabled
 *                              This mode is mandatory for 96 kHz input
 *                              and can also be used for 48 kHz input.
 *  \param[out] size            Decoder struct size in bytes.
 *  \return                     LC3PLUS_OK on success or appropriate error
 *                              code.
 */
int tlka_lc3a_dec_get_size(int32_t samplerate, int32_t channels, LC3PLUS_PlcMethod plc_method, LC3_FrameDuration frame_dms, int32_t hrmode);

/*! Get size of the LC3 decoder scratch buffer.
 *
 *  The provided size is required by lc3plus_dec_init() for a dedicated
 *  samplerate / channel configuration.
 *
 *  Not all configurations may be enabled. For unsupported
 *  configurations an appropriate error code is returned.
 *
 *  \param[out] encoder         Pointer to encoder memory. It must have
 *                              a size of at least the number of bytes
 *                              determined by lc3_dec_get_sizes(), being
 *                              called with the same samplerate /
 *  \param[out] size            Decoder scratch size in bytes.
 *
 */
int tlka_lc3a_dec_get_scratch_size(LC3PLUS_Dec *decoder);

/*! Initialize LC3 decoder.
 *
 *  This function is used to fill a user-allocated decoder struct. This
 *  is typically called once for a samplerate / channels / plc_method /
 *  frame_dms / hrmode / ep_enabled / wavFormat_out configuration.
 *
 *  Not all configurations may be enabled. For unsupported
 *  configurations an appropriate error code is returned.
 *
 *  The configuration arguments samplerate / channels / frame_dms /
 *  hrmode must have the same values that were used for encoding. LC3
 *  does not provide a signalling scheme, transporting these values is
 *  the responsibility of the application. The argument ep_enabled must
 *  be zero, if the encoding was performed with LC3PLUS_EP_OFF, and non-zero
 *  otherwise.
 *
 *  \param[out] decoder         Pointer to decoder memory. It must have
 *                              a size of at least the number of bytes
 *                              determined by lc3_dec_get_sizes(), being
 *                              called with the same samplerate /
 *                              channels / plc_method configuration.
 *  \param[in]  samplerate      Bitstream sample rate. Allowed sample
 *                              rates are: 8000, 16000, 24000, 32000,
 *                              44100, 48000, 96000.
 *  \param[in]  channels        Bitstream number of channels.
 *  \param[in]  plc_method      Packet loss concealment method.
 *  \param[in]  frame_dms       Frame duration in dms (100, 75, 50 or 25).
 *  \param[in]  hrmode          High resolution mode:
 *                              0    : disabled
 *                              else : enabled
 *                              This mode is mandatory for 96 kHz input
 *                              and can also be used for 48 kHz input.
 *  \param[in]  wavFormat_out   waveform format for output signal
 *                              (see LC3_WavFormat in lc3plus_param.h).
 *  \param[in]  ep_enabled      Error protetion mode:
 *                              0    : disabled
 *                              else : enabled
 *  \param[out] scratchSize     Scratch size in bytes.
 *  \return                     LC3PLUS_OK on success or appropriate error
 *                              code.
 */
//LC3PLUS_ErrorCode lc3plus_dec_init(LC3PLUS_Dec* const decoder, int32_t const samplerate,
//		int32_t const channels, LC3PLUS_PlcMethod const plc_method, LC3_FrameDuration const frame_dms, int32_t const hrmode,
//		int32_t const ep_enabled, LC3_WavFormat const wavFormat_out);
LC3PLUS_ErrorCode tlka_lc3a_dec_init(LC3PLUS_Dec *decoder, LC3P_DEC_CFG_Param *lc3p_dec_param);

/*! Get decoder delay.
 *
 *  The decoder delay in number of samples is provided.
 *
 *  \param[in]  decoder         Decoder handle.
 *  \param[out] delay           Decoder delay in samples.
 *  \return                     LC3PLUS_OK on success or appropriate error
 *                              code.
 */
int tlka_lc3a_dec_get_delay(LC3PLUS_Dec *decoder);

/*! Decode an LC3 frame.
 *
 *  lc3plus_dec_init() must be called before calling this function.
 *
 *  Each call provides a fixed number of samples. The number of output
 *  samples can be obtained by lc3plus_dec_get_output_samples().
 *
 *  The output samples are either
 *      + 16-bit fixed point values, or
 *      + 24-bit fixed point values, sign-extended to 32-bit, or
 *      + 32-bit floating point values (range might exceed [-1,+1]).
 *
 *  The data format is configured with lc3plus_dec_init().
 *
 *  If the input is corrupted and cannot be decoded, LC3PLUS_DECODE_ERROR
 *  is returned and packet loss concealment is applied, meaning the
 *  output is still valid. If error protection is enabled,
 *  lc3plus_dec_get_error_report() provides information on the decoding
 *  process. In any case, either regular decoding, partial or full frame
 *  loss concealment is performed.
 *
 *  \param[in]  decoder         Decoder handle initialized by
 *                              lc3plus_dec_init().
 *  \param[in]  input_bytes     Input bytes. If error protection is
 *                              enabled, the input bytes may be altered
 *                              when error correction is applied. Thus
 *                              this buffer must be writable.
 *                              Minimum expected alignment: 8-bit.
 *  \param[in]  num_bytes       Number of valid bytes in input_bytes. To
 *                              signal a lost frame and generate
 *                              concealment output this value must be
 *                              set to 0.
 *  \param[in]  bfi_ext         External Bad Frame Indicator:
 *                              0    : valid packet (normal decoding)
 *                              3    : redundancy frame present
 *                              else : invalid packet (apply PLC)
 *  \param[out] output_samples  Array of pointers to output channel
 *                              buffers. Each channel buffer should
 *                              provide enough space to hold at least
 *                              lc3plus_dec_get_output_samples(). The left
 *                              channel is stored in output_samples[0],
 *                              the right channel in output_samples[1].
 *                              Minimum expected alignment:
 *                              (wavFormat_out==LC3_WAV_FORMAT_INT16) ? 16 : 32 bits
 *                              (wavFormat_out used in lc3plus_dec_init()).
 *  \param      scratch         A pointer to an allocated work buffer of
 *                              at least the number of bytes determined
 *                              by lc3plus_dec_init(). The buffer does not
 *                              have to persist, so it can be used for
 *                              other purposes in between calls.
 *   \return                    LC3PLUS_OK on success or appropriate error
 *                              code. Note, that the output is still
 *                              valid, if LC3PLUS_DECODE_ERROR is returned.
 */
LC3PLUS_ErrorCode tlka_lc3a_dec_process_frame(LC3PLUS_Dec *decoder, void *input_bytes, int32_t num_bytes, int32_t bfi_ext, void **output_samples, void *scratch);

/*! Get decoder frame_length.
 *
 *  \param[in]  decoder         Decoder handle.
 *  \return                     decoder frame_length
 */
int tlka_lc3a_dec_get_output_samples(const LC3PLUS_Dec *decoder);

#endif /* TLKA_LC3A_API_H */
