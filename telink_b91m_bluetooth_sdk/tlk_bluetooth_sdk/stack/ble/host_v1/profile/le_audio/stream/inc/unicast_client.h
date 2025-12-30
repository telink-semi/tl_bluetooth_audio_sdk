/********************************************************************************************************
 * @file    unicast_client.h
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

#ifndef UNICAST_CLIENT_SUPPORT_MAX_ASE_NUM
#define UNICAST_CLIENT_SUPPORT_MAX_ASE_NUM 2
#endif

enum lea_uc_result
{
    LEA_UC_RESULT_SUCCESS,
    LEA_UC_RESULT_ENTER_RELEASING, /** < remote enter releasing state, high level need to disconnect cis. */
    LEA_UC_RESULT_CODEC_FAILURE,
    LEA_UC_RESULT_QOS_FAILURE,
    LEA_UC_RESULT_ENABLE_FAILURE,
    LEA_UC_RESULT_STREAM_FAILURE,
    LEA_UC_RESULT_STOP_FAILURE,
};

struct ble_uc_stream_config;

typedef void (*ble_uc_start_stream_callback)(struct ble_uc_stream_config *p_stream_config, enum lea_uc_result result);

typedef void (*ble_uc_enter_stream_callback)(uint16_t conn_handle, enum lea_uc_result result);

typedef void (*ble_uc_stop_stream_callback)(uint16_t conn_handle, enum lea_uc_result result);

struct ble_uc_stream_config
{
    uint16_t conn_handle;
    uint8_t  cig_id;
    uint8_t  stream_mode;
    uint8_t  ase_num;

    struct
    {
        uint8_t                            ase_id;
        uint8_t                            cis_id;
        uint8_t                            latency; /** < refer to enum lea_ascs_target_latency */
        uint8_t                            phy;     /** < refer to enum lea_ascs_target_phy  */
        struct lea_codec_id                codec_id;
        struct lea_codec_spec_config_param codec_cfg;
        struct lea_metadata_param          metadata;
    } ase_info[UNICAST_CLIENT_SUPPORT_MAX_ASE_NUM];

    ble_uc_start_stream_callback callback;
};

struct ble_uc_enter_stream
{
    uint16_t conn_handle;
    uint8_t  ase_num;
    uint8_t  ase_id[UNICAST_CLIENT_SUPPORT_MAX_ASE_NUM];

    ble_uc_enter_stream_callback callback;
};

struct ble_uc_stop_stream
{
    uint16_t conn_handle;
    uint8_t  ase_num;
    uint8_t  ase_id[UNICAST_CLIENT_SUPPORT_MAX_ASE_NUM];

    ble_uc_stop_stream_callback callback;
};

/**
 * @brief       Starts a unicast stream based on the provided configuration.
 * @param[in]   p_stream_config - pointer to the stream configuration.
 * @return      BLE_PRF_SUCCESS if the stream procedure starts, otherwise an error code.
 * @note        Completion result is reported through the callback inside `ble_uc_stream_config`.
 */
int ble_uc_start_stream(struct ble_uc_stream_config *p_stream_config);

/**
 * @brief       Enters an already configured unicast stream (CIS establishment).
 * @param[in]   p_enter_stream - pointer to the enter-stream parameters.
 * @return      BLE_PRF_SUCCESS if the procedure starts, otherwise an error code.
 * @note        Completion result is delivered via `ble_uc_enter_stream_callback`.
 */
int ble_uc_enter_stream(struct ble_uc_enter_stream *p_enter_stream);

/**
 * @brief       Stops an active unicast stream.
 * @param[in]   p_stream_config - pointer to the stop-stream parameters.
 * @return      BLE_PRF_SUCCESS if the stop procedure starts, otherwise an error code.
 * @note        Completion result is delivered via `ble_uc_stop_stream_callback`.
 */
int ble_uc_stop_stream(struct ble_uc_stop_stream *p_stream_config);
