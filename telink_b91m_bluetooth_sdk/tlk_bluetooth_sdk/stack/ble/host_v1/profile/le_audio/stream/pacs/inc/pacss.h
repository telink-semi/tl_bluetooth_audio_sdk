/********************************************************************************************************
 * @file    pacss.h
 *
 * @brief   This is the header file for TLSR/TL
 *
 * @author  Bluetooth Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd.
 *          All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRANTY of ANY KIND is provided.
 *
 *******************************************************************************************************/

// PACSS: Published Audio Capabilities Service Server.
#define LEA_PAC_PARAM(lc3_id, chn, contexts) LEA_INIT_CODEC_ID_LC3_STREAM, LEA_CODEC_CAP_##lc3_id##_STREAM(chn), LEA_PAC_PARAM_METADATA(contexts)

// #define X(lc3_id, chn, contexts) lc3_id,
// COUNT_ARGS(SINK_PAC_CHANNEL_2_LIST) - 1,
// #undef X
// #define X(lc3_id, chn, contexts) LEA_PAC_PARAM(lc3_id, chn, contexts),
// SINK_PAC_CHANNEL_2_LIST
// #undef X

struct ble_pacss_register_param
{
    uint16_t       sink_pac_len; // Sink PAC Length
    const uint8_t *sink_pac;
    uint32_t       sink_audio_locations; // Sink Audio Location, LEA_LOCATION_FRONT_LEFT

    uint16_t       source_pac_len;         // Source PAC Length
    const uint8_t *source_pac;             // Source PAC
    uint32_t       source_audio_locations; // Source Audio Location, LEA_LOCATION_FRONT_LEFT

    uint16_t available_sink_contexts;   // Available Sink Contexts, BLC_AUDIO_CONTEXT_TYPE_UNSPECIFIED
    uint16_t available_source_contexts; // Available Source Contexts, BLC_AUDIO_CONTEXT_TYPE_UNSPECIFIED
    uint16_t supported_sink_contexts;   // Supported Sink Contexts, BLC_AUDIO_CONTEXT_TYPE_UNSPECIFIED
    uint16_t supported_source_contexts; // Supported Sink Contexts, BLC_AUDIO_CONTEXT_TYPE_UNSPECIFIED
};

/**
 *   @brief Register the Published Audio Capabilities Service server control.
 *
 *   @param[in] param Pointer to the structure containing the initial values for the service.
 *
 *   @note This function should be called before any other function in the PACSS module.
 *
 *   @return None.
 */
void ble_lea_register_PACS_control_server(const struct ble_pacss_register_param *param);

/**
 *   @brief Update the Sink Audio Locations of the Published Audio Capabilities Service server.
 *
 *   @param[in] conn_handle Connection handle to identify the connection.
 *   @param[in] locations The new Sink Audio Locations to be set.
 *
 *   @return BLE_HOST_ERR_SUCC if the update was successful, or an error code otherwise.
 *
 *   @note Even if the connection handle is invalid, the Sink Audio Locations is still updated.
 */
int ble_pacss_update_sink_audio_locations(uint16_t conn_handle, uint32_t locations);

/**
 *   @brief Update the Source Audio Locations of the Published Audio Capabilities Service server.
 *
 *   @param[in] conn_handle Connection handle to identify the connection.
 *   @param[in] locations The new Source Audio Locations to be set.
 *
 *   @return BLE_HOST_ERR_SUCC if the update was successful, or an error code otherwise.
 *
 *   @note Even if the connection handle is invalid, the Source Audio Locations is still updated.
 */
int ble_pacss_update_source_audio_locations(uint16_t conn_handle, uint32_t locations);

/**
 *   @brief Update the Available and Supported Audio Contexts of the Published Audio Capabilities Service server.
 *
 *   @param[in] conn_handle Connection handle to identify the connection.
 *   @param[in] sinkContexts The new Available and Supported Sink Contexts to be set.
 *   @param[in] sourceContexts The new Available and Supported Source Contexts to be set.
 *
 *   @return BLE_HOST_ERR_SUCC if the update was successful, or an error code otherwise.
 *
 *   @note Even if the connection handle is invalid, the Available and Supported Audio Contexts is still updated.
 */
int ble_pacss_update_available_audio_contexts(uint16_t conn_handle, uint16_t sinkContexts, uint16_t sourceContexts);

/**
 *   @brief Get the Available Audio Contexts of the Published Audio Capabilities Service server.
 *
 *   @param[in] sink_pac True if the Sink PAC is requested, False if the Source PAC is requested.
 *
 *   @return The Available Audio Contexts of the requested PAC.
 */
uint16_t ble_pacss_get_available_context(bool sink_pac);

/**
 *   @brief Check Codec ID and Specific Configuration for the requested PAC.
 *
 *   @param[in] sink_pac True if the Sink PAC is requested, False if the Source PAC is requested.
 *   @param[in] codec_id Pointer to the Codec ID to be checked.
 *   @param[in] codec_cfg_param Pointer to the Codec Specific Configuration to be checked.
 *
 *   @return True if the Codec ID and Specific Configuration are valid, False otherwise.
 */
bool ble_pacss_check_codec_configuration(bool sink_pac, struct lea_codec_id *codec_id, struct lea_codec_specific_config_parsed *codec_cfg_param);

#define LEA_CODEC_ID_CODEC_FORMAT(format, company_id, vendor_spec) (format), U16_TO_BYTES(company_id), U16_TO_BYTES(vendor_spec)
#define LEA_INIT_CODEC_ID_LC3_STREAM                               LEA_CODEC_ID_CODEC_FORMAT(BT_CODING_FORMAT_LC3, 0x0000, 0x0000)

#define LEA_CODEC_CAP_SUPP_SAMPLING_FREQ(freq)                     0x03, LEA_CAP_TYPE_SUPP_SAMPLE_FREQUENCY, U16_TO_BYTES(freq)
#define LEA_CODEC_CAP_SUPP_FRAME_DURATION(duration)                0x02, LEA_CAP_TYPE_SUPP_FRAME_DURATION, (duration)
#define LEA_CODEC_CAP_AUDIO_CHANNEL_COUNTS(chn)                    0x02, LEA_CAP_TYPE_SUPP_AUDIO_CHN_COUNTS, chn
#define LEA_CODEC_CAP_PER_CODEC_FRAME(min_octets, max_octets)      0x05, LEA_CAP_TYPE_SUPP_OCTETS_PER_CODEC_FRAME, U16_TO_BYTES(min_octets), U16_TO_BYTES(max_octets)
#define LEA_CODEC_CAP_MAX_CODEC_FRAMES_PER_SDU(frames)             0x02, LEA_CAP_TYPE_SUPP_MAX_CODEC_FRAMES_PER_SDU, frames

#define LEA_INIT_CODEC_SPEC_CAP_STREAM(freq, duration, octets, chn, frames)                                                           \
    19, LEA_CODEC_CAP_SUPP_SAMPLING_FREQ(freq), LEA_CODEC_CAP_SUPP_FRAME_DURATION(duration), LEA_CODEC_CAP_AUDIO_CHANNEL_COUNTS(chn), \
        LEA_CODEC_CAP_PER_CODEC_FRAME(octets, octets), LEA_CODEC_CAP_MAX_CODEC_FRAMES_PER_SDU(frames)

#define LEA_CODEC_CAP_LC3_8_1_STREAM(chn)         LEA_INIT_CODEC_SPEC_CAP_STREAM(LEA_SUPP_FREQ_8000_HZ, LEA_SUPP_FRAME_DURATION_7_5, 26, chn, 1)
#define LEA_CODEC_CAP_LC3_8_2_STREAM(chn)         LEA_INIT_CODEC_SPEC_CAP_STREAM(LEA_SUPP_FREQ_8000_HZ, LEA_SUPP_FRAME_DURATION_10, 30, chn, 1)
#define LEA_CODEC_CAP_LC3_16_1_STREAM(chn)        LEA_INIT_CODEC_SPEC_CAP_STREAM(LEA_SUPP_FREQ_16000_HZ, LEA_SUPP_FRAME_DURATION_7_5, 30, chn, 1)
#define LEA_CODEC_CAP_LC3_16_2_STREAM(chn)        LEA_INIT_CODEC_SPEC_CAP_STREAM(LEA_SUPP_FREQ_16000_HZ, LEA_SUPP_FRAME_DURATION_10, 40, chn, 1)
#define LEA_CODEC_CAP_LC3_24_1_STREAM(chn)        LEA_INIT_CODEC_SPEC_CAP_STREAM(LEA_SUPP_FREQ_24000_HZ, LEA_SUPP_FRAME_DURATION_7_5, 45, chn, 1)
#define LEA_CODEC_CAP_LC3_24_2_STREAM(chn)        LEA_INIT_CODEC_SPEC_CAP_STREAM(LEA_SUPP_FREQ_24000_HZ, LEA_SUPP_FRAME_DURATION_10, 60, chn, 1)
#define LEA_CODEC_CAP_LC3_32_1_STREAM(chn)        LEA_INIT_CODEC_SPEC_CAP_STREAM(LEA_SUPP_FREQ_32000_HZ, LEA_SUPP_FRAME_DURATION_7_5, 60, chn, 1)
#define LEA_CODEC_CAP_LC3_32_2_STREAM(chn)        LEA_INIT_CODEC_SPEC_CAP_STREAM(LEA_SUPP_FREQ_32000_HZ, LEA_SUPP_FRAME_DURATION_10, 80, chn, 1)
#define LEA_CODEC_CAP_LC3_441_1_STREAM(chn)       LEA_INIT_CODEC_SPEC_CAP_STREAM(LEA_SUPP_FREQ_44100_HZ, LEA_SUPP_FRAME_DURATION_7_5, 97, chn, 1)
#define LEA_CODEC_CAP_LC3_441_2_STREAM(chn)       LEA_INIT_CODEC_SPEC_CAP_STREAM(LEA_SUPP_FREQ_44100_HZ, LEA_SUPP_FRAME_DURATION_10, 130, chn, 1)
#define LEA_CODEC_CAP_LC3_48_1_STREAM(chn)        LEA_INIT_CODEC_SPEC_CAP_STREAM(LEA_SUPP_FREQ_48000_HZ, LEA_SUPP_FRAME_DURATION_7_5, 75, chn, 1)
#define LEA_CODEC_CAP_LC3_48_2_STREAM(chn)        LEA_INIT_CODEC_SPEC_CAP_STREAM(LEA_SUPP_FREQ_48000_HZ, LEA_SUPP_FRAME_DURATION_10, 100, chn, 1)
#define LEA_CODEC_CAP_LC3_48_3_STREAM(chn)        LEA_INIT_CODEC_SPEC_CAP_STREAM(LEA_SUPP_FREQ_48000_HZ, LEA_SUPP_FRAME_DURATION_7_5, 90, chn, 1)
#define LEA_CODEC_CAP_LC3_48_4_STREAM(chn)        LEA_INIT_CODEC_SPEC_CAP_STREAM(LEA_SUPP_FREQ_48000_HZ, LEA_SUPP_FRAME_DURATION_10, 120, chn, 1)
#define LEA_CODEC_CAP_LC3_48_5_STREAM(chn)        LEA_INIT_CODEC_SPEC_CAP_STREAM(LEA_SUPP_FREQ_48000_HZ, LEA_SUPP_FRAME_DURATION_7_5, 117, chn, 1)
#define LEA_CODEC_CAP_LC3_48_6_STREAM(chn)        LEA_INIT_CODEC_SPEC_CAP_STREAM(LEA_SUPP_FREQ_48000_HZ, LEA_SUPP_FRAME_DURATION_10, 155, chn, 1)

#define LEA_METADATA_PREFERRED_CONTEXTS(contexts) 0x03, LEA_METADATA_TYPE_PREFERRED_CONTEXTS, U16_TO_BYTES(contexts)
#define LEA_METADATA_STREAMING_CONTEXTS(contexts) 0x03, LEA_METADATA_TYPE_STREAMING_CONTEXTS, U16_TO_BYTES(contexts)

#define LEA_PAC_PARAM_METADATA(contexts)          0x08, LEA_METADATA_PREFERRED_CONTEXTS(contexts), LEA_METADATA_STREAMING_CONTEXTS(contexts)
