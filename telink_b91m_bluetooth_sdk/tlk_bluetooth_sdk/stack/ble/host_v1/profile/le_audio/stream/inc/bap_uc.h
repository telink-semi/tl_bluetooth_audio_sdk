/********************************************************************************************************
 * @file    bap_uc.h
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

enum bapuc_operate_status
{
    BAPUC_OPERATE_STATUS_SUCCESS          = 0,
    BAPUC_OPERATE_STATUS_FAIL             = 1,
    BAPUC_OPERATE_STATUS_ACL_DISCONNECTED = 2,
    BAPUC_OPERATE_STATUS_RELEASING        = 3,
};

typedef void (*ble_bapuc_operate_callback)(uint16_t conn_handle, uint8_t status, void *user_data);

struct ble_bapuc_codec_config
{
    uint8_t                            ase_id;
    const struct ble_ase_codec_config *p_codec;
};

struct ble_bapuc_qos_config
{
    uint8_t                          ase_id;
    const struct ble_ase_qos_config *p_qos;
};

struct ble_bapuc_qos_config_default
{
    uint8_t                           ase_id;
    struct ble_ase_qos_config_default qos;
};

struct ble_bapuc_enable
{
    uint8_t                          ase_id;
    const struct lea_metadata_param *p_metadata;
};

struct ble_bapuc_enter_enabling
{
    uint8_t                            ase_id;
    uint8_t                            cig_id;
    uint8_t                            cis_id;
    const struct ble_ase_codec_config *p_codec;
    const struct lea_metadata_param   *p_metadata;
};

/**
 * @brief       Initializes the BAP unicast client module.
 * @return      none.
 * @note        Must be called before issuing any BAPUC procedures.
 */
void ble_bapuc_init(void);

/**
 * @brief       Sends a Configure Codec opcode for one or more ASEs.
 * @param[in]   conn_handle         - ACL connection handle.
 * @param[in]   ase_num             - number of ASE configurations provided.
 * @param[in]   p_bapuc_codec_config- array of codec configuration parameters.
 * @param[in]   callback            - operation completion callback.
 * @param[in]   user_data           - user context passed to the callback.
 * @return      BLE_PRF_SUCCESS if the request is queued, otherwise an error code.
 * @note        Each entry in `p_bapuc_codec_config` maps to a single ASE.
 */
int ble_bapuc_send_config_codec(uint16_t conn_handle, uint8_t ase_num, const struct ble_bapuc_codec_config *p_bapuc_codec_config, ble_bapuc_operate_callback callback,
                                void *user_data);

/**
 * @brief       Sends a Configure QoS opcode for one or more ASEs.
 * @param[in]   conn_handle         - ACL connection handle.
 * @param[in]   ase_num             - number of ASE configurations provided.
 * @param[in]   p_bapuc_qos_config  - array of QoS configuration parameters.
 * @param[in]   callback            - operation completion callback.
 * @param[in]   user_data           - user context passed to the callback.
 * @return      BLE_PRF_SUCCESS if the request is queued, otherwise an error code.
 */
int ble_bapuc_send_config_qos(uint16_t conn_handle, uint8_t ase_num, const struct ble_bapuc_qos_config *p_bapuc_qos_config, ble_bapuc_operate_callback callback, void *user_data);

/**
 * @brief       Sends a Configure QoS (default) opcode for one or more ASEs.
 * @param[in]   conn_handle             - ACL connection handle.
 * @param[in]   ase_num                 - number of ASE configurations provided.
 * @param[in]   p_bapuc_qos_config      - array of default QoS configuration parameters.
 * @param[in]   callback                - operation completion callback.
 * @param[in]   user_data               - user context passed to the callback.
 * @return      BLE_PRF_SUCCESS if the request is queued, otherwise an error code.
 */
int ble_bapuc_send_config_qos_default(uint16_t conn_handle, uint8_t ase_num, const struct ble_bapuc_qos_config_default *p_bapuc_qos_config, ble_bapuc_operate_callback callback,
                                      void *user_data);

/**
 * @brief       Sends an Enable opcode with metadata for one or more ASEs.
 * @param[in]   conn_handle     - ACL connection handle.
 * @param[in]   ase_num         - number of ASE configurations provided.
 * @param[in]   p_bapuc_enable  - array of enable parameters per ASE.
 * @param[in]   callback        - operation completion callback.
 * @param[in]   user_data       - user context passed to the callback.
 * @return      BLE_PRF_SUCCESS if the request is queued, otherwise an error code.
 */
int ble_bapuc_send_enable(uint16_t conn_handle, uint8_t ase_num, const struct ble_bapuc_enable *p_bapuc_enable, ble_bapuc_operate_callback callback, void *user_data);

/**
 * @brief       Sends a Receiver Start Ready opcode for the specified ASE IDs.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   ase_num     - number of ASE IDs provided.
 * @param[in]   p_ase_id    - pointer to ASE ID array.
 * @param[in]   callback    - operation completion callback.
 * @param[in]   user_data   - user context passed to the callback.
 * @return      BLE_PRF_SUCCESS if the request is queued, otherwise an error code.
 */
int ble_bapuc_send_receiver_start_ready(uint16_t conn_handle, uint8_t ase_num, const uint8_t *p_ase_id, ble_bapuc_operate_callback callback, void *user_data);

/**
 * @brief       Sends a Release opcode for the specified ASE IDs.
 * @param[in]   conn_handle - ACL connection handle.
 * @param[in]   ase_num     - number of ASE IDs provided.
 * @param[in]   p_ase_id    - pointer to ASE ID array.
 * @param[in]   callback    - operation completion callback.
 * @param[in]   user_data   - user context passed to the callback.
 * @return      BLE_PRF_SUCCESS if the request is queued, otherwise an error code.
 */
int ble_bapuc_send_release(uint16_t conn_handle, uint8_t ase_num, const uint8_t *p_ase_id, ble_bapuc_operate_callback callback, void *user_data);
