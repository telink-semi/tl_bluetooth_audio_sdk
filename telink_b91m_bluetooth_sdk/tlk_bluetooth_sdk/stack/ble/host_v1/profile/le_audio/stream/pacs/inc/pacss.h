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

#define LEA_PAC_PARAM(lc3_id, chn, contexts)               \
    {                                                      \
        .codec_id  = LEA_INIT_CODEC_ID_LC3,                \
        .codec_cap = LEA_CODEC_CAP_##lc3_id(chn),          \
        .metadata  = LEA_INIT_METADATA_CONTEXTS(contexts), \
    }

struct lea_pac_param
{
    struct lea_codec_id             codec_id; //Codec ID, 06 0000 0000 mean LC3 codec
    struct lea_codec_spec_cap_param codec_cap;
    struct lea_metadata_param       metadata;
};

struct ble_pacss_register_param
{
    uint8_t                     sink_pac_num;         // number of Sink PAC records
    const struct lea_pac_param *sink_pac;             // Sink PAC
    uint32_t                    sink_audio_locations; // Sink Audio Location, LEA_LOCATION_FRONT_LEFT

    uint8_t                     source_pac_num;         // number of Source PAC records
    const struct lea_pac_param *source_pac;             // Source PAC
    uint32_t                    source_audio_locations; // Source Audio Location, LEA_LOCATION_FRONT_LEFT

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
