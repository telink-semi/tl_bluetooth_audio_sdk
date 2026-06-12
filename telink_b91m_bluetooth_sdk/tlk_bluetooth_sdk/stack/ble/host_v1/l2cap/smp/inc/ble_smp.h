/********************************************************************************************************
 * @file    ble_smp.h
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

enum ble_host_smp_pair_method
{
    BLE_HOST_SMP_PAIR_JUST_WORKS         = 1 << 0,
    BLE_HOST_SMP_PAIR_METHOD_OOB         = 1 << 1,
    BLE_HOST_SMP_PAIR_METHOD_INIT_INPUT  = 1 << 2,
    BLE_HOST_SMP_PAIR_METHOD_INIT_OUTPUT = 1 << 3,
    BLE_HOST_SMP_PAIR_METHOD_INIT_ALL    = BLE_HOST_SMP_PAIR_METHOD_INIT_INPUT | BLE_HOST_SMP_PAIR_METHOD_INIT_OUTPUT,
    BLE_HOST_SMP_PAIR_METHOD_RSP_INPUT   = 1 << 4,
    BLE_HOST_SMP_PAIR_METHOD_RSP_OUTPUT  = 1 << 5,
    BLE_HOST_SMP_PAIR_METHOD_RSP_ALL     = BLE_HOST_SMP_PAIR_METHOD_RSP_INPUT | BLE_HOST_SMP_PAIR_METHOD_RSP_OUTPUT,
    BLE_HOST_SMP_PAIR_METHOD_SC          = 1 << 6,

    BLE_HOST_SMP_PAIR_PASSKEY_ENTRY_RSP_INPUT     = BLE_HOST_SMP_PAIR_METHOD_INIT_OUTPUT | BLE_HOST_SMP_PAIR_METHOD_RSP_INPUT,
    BLE_HOST_SMP_PAIR_PASSKEY_ENTRY_INIT_INPUT    = BLE_HOST_SMP_PAIR_METHOD_INIT_INPUT | BLE_HOST_SMP_PAIR_METHOD_RSP_OUTPUT,
    BLE_HOST_SMP_PAIR_PASSKEY_ENTRY_BOTH_INPUT    = BLE_HOST_SMP_PAIR_METHOD_INIT_INPUT | BLE_HOST_SMP_PAIR_METHOD_RSP_INPUT,
    BLE_HOST_SMP_PAIR_OOB_AUTHENTICATION          = BLE_HOST_SMP_PAIR_METHOD_OOB,
    BLE_HOST_SMP_PAIR_SC_JUST_WORKS               = BLE_HOST_SMP_PAIR_JUST_WORKS | BLE_HOST_SMP_PAIR_METHOD_SC,
    BLE_HOST_SMP_PAIR_SC_PASSKEY_ENTRY_RSP_INPUT  = BLE_HOST_SMP_PAIR_PASSKEY_ENTRY_RSP_INPUT | BLE_HOST_SMP_PAIR_METHOD_SC,
    BLE_HOST_SMP_PAIR_SC_PASSKEY_ENTRY_INIT_INPUT = BLE_HOST_SMP_PAIR_PASSKEY_ENTRY_INIT_INPUT | BLE_HOST_SMP_PAIR_METHOD_SC,
    BLE_HOST_SMP_PAIR_SC_PASSKEY_ENTRY_BOTH_INPUT = BLE_HOST_SMP_PAIR_PASSKEY_ENTRY_BOTH_INPUT | BLE_HOST_SMP_PAIR_METHOD_SC,
    BLE_HOST_SMP_PAIR_SC_OOB_AUTHENTICATION       = BLE_HOST_SMP_PAIR_OOB_AUTHENTICATION | BLE_HOST_SMP_PAIR_METHOD_SC,
    BLE_HOST_SMP_PAIR_NUMERIC_COMPARISON          = BLE_HOST_SMP_PAIR_METHOD_SC | BLE_HOST_SMP_PAIR_METHOD_INIT_ALL | BLE_HOST_SMP_PAIR_METHOD_RSP_ALL,
};

enum ble_host_smp_security_level
{
    BLE_HOST_SECURITY_MODE_1_LEVEL_1, /** < No authentication, no encryption */
    BLE_HOST_SECURITY_MODE_1_LEVEL_2, /** < Unauthenticated pairing with encryption */
    BLE_HOST_SECURITY_MODE_1_LEVEL_3, /** < Authenticated pairing with encryption */
    BLE_HOST_SECURITY_MODE_1_LEVEL_4, /** < Secure Connections pairing with encryption */
};

struct ble_host_smp_pairing_params
{
    bool    secure_connection;
    uint8_t method;
    uint8_t level;
};

typedef void (*ble_host_smp_pairing_start_callback)(uint16_t conn_handle, const struct ble_host_smp_pairing_params *params);

typedef void (*ble_host_smp_pairing_finished_callback)(uint16_t conn_handle, uint8_t reason);

typedef void (*ble_host_smp_io_output_callback)(uint16_t conn_handle, uint32_t tk, bool numeric);

typedef void (*ble_host_smp_io_input_callback)(uint16_t conn_handle);

typedef void (*ble_host_smp_oob_request_callback)(uint16_t conn_handle, bool secure_connections);

struct ble_host_smp_callbacks
{
    ble_host_smp_io_output_callback        io_output_cb;
    ble_host_smp_io_input_callback         io_input_cb;
    ble_host_smp_oob_request_callback      oob_request_cb;
    ble_host_smp_pairing_start_callback    pairing_start_cb;
    ble_host_smp_pairing_finished_callback pairing_finish_cb;
};

struct ble_host_smp_init_params
{
    /** < If true and io_input_cb is not NULL, input capacity is set keyboard, If false and io_input_cb is not NULL, input capacity is set Yes/No. */
    bool                                 input_keyboard;
    bool                                 bonding;      /** < bonding flags, default use true. */
    bool                                 mitm;         /** < man-in-the-middle. */
    bool                                 sc;           /** < secure connection. */
    bool                                 keypress;     /** < keypress notifications. */
    bool                                 ct2;          /** < confirm value 2. */
    uint8_t                              max_key_size; /** < range: 7 to 16 , default use 16. */
    uint8_t                              initiator_key_distribution;
    uint8_t                              responder_key_distribution;
    const struct ble_host_smp_callbacks *callbacks; /** < callbacks for SMP events. must use global variable. */
};

#define BLE_HOST_SMP_INIT_ALL_CALLBACKS(input_callback, output_callback, oob_callback, start_callback, finish_callback) \
    (&(const struct ble_host_smp_callbacks){                                                                            \
        .io_input_cb       = input_callback,                                                                            \
        .io_output_cb      = output_callback,                                                                           \
        .oob_request_cb    = oob_callback,                                                                              \
        .pairing_start_cb  = start_callback,                                                                            \
        .pairing_finish_cb = finish_callback,                                                                           \
    })

#define BLE_HOST_SMP_LEGACY_JUST_WORKS_INIT_PARAMS \
    (&(const struct ble_host_smp_init_params){     \
        .bonding                    = true,        \
        .max_key_size               = 16,          \
        .initiator_key_distribution = 0x03,        \
        .responder_key_distribution = 0x03,        \
    })

#define BLE_HOST_SMP_LEGACY_JUST_WORKS(start_callback, finish_callback)                                                   \
    (&(const struct ble_host_smp_init_params){                                                                            \
        .bonding                    = true,                                                                               \
        .max_key_size               = 16,                                                                                 \
        .initiator_key_distribution = 0x03,                                                                               \
        .responder_key_distribution = 0x03,                                                                               \
        .callbacks                  = BLE_HOST_SMP_INIT_ALL_CALLBACKS(NULL, NULL, NULL, start_callback, finish_callback), \
    })

#define BLE_HOST_SMP_SC_JUST_WORKS_INIT_PARAMS \
    (&(const struct ble_host_smp_init_params){ \
        .bonding                    = true,    \
        .sc                         = true,    \
        .max_key_size               = 16,      \
        .initiator_key_distribution = 0x03,    \
        .responder_key_distribution = 0x03,    \
    })

#define BLE_HOST_SMP_SC_JUST_WORKS(start_callback, finish_callback)                                                       \
    (&(const struct ble_host_smp_init_params){                                                                            \
        .bonding                    = true,                                                                               \
        .sc                         = true,                                                                               \
        .max_key_size               = 16,                                                                                 \
        .initiator_key_distribution = 0x03,                                                                               \
        .responder_key_distribution = 0x03,                                                                               \
        .callbacks                  = BLE_HOST_SMP_INIT_ALL_CALLBACKS(NULL, NULL, NULL, start_callback, finish_callback), \
    })

#define BLE_HOST_SMP_LEGACY_PASSKEY_INIT_INPUT(keyboard, input_callback, output_callback, start_callback, finish_callback)                     \
    (&(const struct ble_host_smp_init_params){                                                                                                 \
        .input_keyboard             = keyboard,                                                                                                \
        .bonding                    = true,                                                                                                    \
        .mitm                       = true,                                                                                                    \
        .max_key_size               = 16,                                                                                                      \
        .initiator_key_distribution = 0x03,                                                                                                    \
        .responder_key_distribution = 0x03,                                                                                                    \
        .callbacks                  = BLE_HOST_SMP_INIT_ALL_CALLBACKS(input_callback, output_callback, NULL, start_callback, finish_callback), \
    })

#define BLE_HOST_SMP_SC_PASSKEY_INIT_INPUT(keyboard, input_callback, output_callback, start_callback, finish_callback)                         \
    (&(const struct ble_host_smp_init_params){                                                                                                 \
        .input_keyboard             = keyboard,                                                                                                \
        .bonding                    = true,                                                                                                    \
        .mitm                       = true,                                                                                                    \
        .sc                         = true,                                                                                                    \
        .max_key_size               = 16,                                                                                                      \
        .initiator_key_distribution = 0x03,                                                                                                    \
        .responder_key_distribution = 0x03,                                                                                                    \
        .callbacks                  = BLE_HOST_SMP_INIT_ALL_CALLBACKS(input_callback, output_callback, NULL, start_callback, finish_callback), \
    })

#define BLE_HOST_SMP_LEGACY_OOB_INIT(oob_callback, start_callback, finish_callback)                                               \
    (&(const struct ble_host_smp_init_params){                                                                                    \
        .bonding                    = true,                                                                                       \
        .max_key_size               = 16,                                                                                         \
        .initiator_key_distribution = 0x03,                                                                                       \
        .responder_key_distribution = 0x03,                                                                                       \
        .callbacks                  = BLE_HOST_SMP_INIT_ALL_CALLBACKS(NULL, NULL, oob_callback, start_callback, finish_callback), \
    })

#define BLE_HOST_SMP_INIT_PARAMS_DEFAULT BLE_HOST_SMP_SC_JUST_WORKS_INIT_PARAMS

/**
 *    @brief BLE Host SMP initialization.
 *
 *    @param[in] params SMP initialization parameters.
 *
 *    @return BLE_HOST_ERR_SUCC initialization success.
 *             - BLE_L2CAP_ERR(BLE_L2CAP_ERR_SMP_INVALID_MAX_KEY_SIZE) max key size is not valid.
 *             - BLE_L2CAP_ERR(BLE_L2CAP_ERR_SMP_INVALID_KEY_DISTRIBUTION) key distribution is not valid.
 *             - BLE_L2CAP_ERR(BLE_L2CAP_ERR_INVALID_PARAMS) invalid parameters.
 */
int ble_host_smp_initial(const struct ble_host_smp_init_params *params);

/**
 *    @brief BLE Host SMP derivation key enable.
 *
 *    @param[in] enable Derivation key enable,bt link to le ltk and ltk to link key.
 */
void ble_host_smp_derivation_key_enable(uint8_t enable);

/**
 *    @brief BLE Host SMP deinitialization.
 *
 *    @return BLE_HOST_ERR_SUCC deinitialization success.
 */
int ble_host_smp_deinit(void);

/**
 *    @brief BLE Host SMP set local IRK.
 *
 *    @param[in] irk Local IRK.
 *
 *    @return None.
 */
void ble_host_smp_set_local_irk(const uint8_t irk[16]);

/**
 *    @brief BLE Host SMP get local IRK.
 *
 *    @param[out] irk Local IRK.
 *
 *    @return None.
 */
void ble_host_smp_get_local_irk(uint8_t irk[16]);

/**
 *    @brief BLE Host SMP set OOB(out-of-band legacy pairing support) value.
 *
 *    @param[in] conn_handle Connection handle.
 *    @param[in] oob_value OOB value.
 *
 *    @return BLE_HOST_ERR_SUCC set OOB value success.
 *             - BLE_L2CAP_ERR(BLE_L2CAP_ERR_INVALID_CONN_HANDLE) invalid connection handle.
 *             - BLE_L2CAP_ERR(BLE_L2CAP_ERR_SMP_STATE_INVALID) invalid SMP state, not need to set OOB value.
 */
int ble_host_smp_set_oob_value(uint16_t conn_handle, const uint8_t oob_value[16]);

/**
 *    @brief BLE Host SMP set passkey, support legacy pairing and secure connection pairing.
 *
 *    @param[in] conn_handle Connection handle.
 *    @param[in] passkey Passkey.
 *
 *    @return BLE_HOST_ERR_SUCC set passkey success.
 *             - BLE_L2CAP_ERR(BLE_L2CAP_ERR_INVALID_CONN_HANDLE) invalid connection handle.
 *             - BLE_L2CAP_ERR(BLE_L2CAP_ERR_SMP_STATE_INVALID) invalid SMP state, not need to set passkey.
 */
int ble_host_smp_set_passkey(uint16_t conn_handle, uint32_t passkey);

/**
 *    @brief BLE Host SMP start pairing peripheral connection.
 *
 *    @param[in] conn_handle Connection handle.
 *
 *    @return BLE_HOST_ERR_SUCC start pairing success.
 *             - BLE_L2CAP_ERR(BLE_L2CAP_ERR_INVALID_CONN_HANDLE) invalid connection handle.
 *             - BLE_L2CAP_ERR(BLE_L2CAP_ERR_SMP_NOT_SUPPORTED) not support SMP pairing.
 *             - BLE_L2CAP_ERR(BLE_L2CAP_ERR_SMP_BUSY) SMP busy, not need to start pairing.
 *
 *    @note This function call success, host only send security request to peer device, and wait start pairing.
 */
int ble_host_smp_start_pairing_peripheral(uint16_t conn_handle);

/**
 *    @brief BLE Host SMP send pairing failure with reason is SMP_FAILED_UNSPECIFIED_REASON.
 *
 *    @param[in] conn_handle Connection handle.
 *
 *    @return BLE_HOST_ERR_SUCC start pairing success.
 *
 *    @note This function must call after pairing start and before pairing finish, and send pairing failure with reason is SMP_FAILED_UNSPECIFIED_REASON.
 */
int ble_host_smp_send_pairing_failed_user(uint16_t conn_handle);

/**
 *    @brief BLE Host SMP user reject pairing hook function.
 *
 *    @param[in] conn_handle Connection handle.
 *
 *    @return True reject pairing, False not reject pairing.
 */
// bool ble_host_smp_peripheral_reject_pairing_hook(uint16_t conn_handle);

/**
 *    @brief BLE Host SMP start pairing central connection.
 *
 *    @param[in] conn_handle Connection handle.
 *
 *    @return BLE_HOST_ERR_SUCC start pairing success.
 *             - BLE_L2CAP_ERR(BLE_L2CAP_ERR_INVALID_CONN_HANDLE) invalid connection handle.
 *             - BLE_L2CAP_ERR(BLE_L2CAP_ERR_SMP_NOT_SUPPORTED) not support SMP pairing.
 *             - BLE_L2CAP_ERR(BLE_L2CAP_ERR_SMP_BUSY) SMP busy, not need to start pairing.
 *
 *    @note This function call success, if not pairing device, host will send pairing request to peer device, and start pairing.
 *                                      If paired device, host will start encryption.
 */
int ble_host_smp_start_pairing_central(uint16_t conn_handle);

/**
 *    @brief BLE Host SMP start reconnection central connection.
 *
 *    @param[in] conn_handle Connection handle.
 *
 *    @return BLE_HOST_ERR_SUCC start reconnection success.
 *             - BLE_L2CAP_ERR(BLE_L2CAP_ERR_INVALID_CONN_HANDLE) invalid connection handle.
 *             - BLE_L2CAP_ERR(BLE_L2CAP_ERR_SMP_NOT_SUPPORTED) not support SMP pairing.
 *             - BLE_L2CAP_ERR(BLE_L2CAP_ERR_SMP_BUSY) SMP busy, not need to start pairing.
 *             - BLE_L2CAP_ERR(BLE_L2CAP_ERR_SMP_NON_STORE) not support non-store pairing.
 *
 *    @note This function call success, if not pairing device, host return BLE_L2CAP_ERR(BLE_L2CAP_ERR_SMP_NON_STORE).
 *                                      If paired device, host will start encryption.
 */
int ble_host_smp_start_reconnect_central(uint16_t conn_handle);

/**
 *    @brief BLE Host SMP start new pairing central connection.
 *
 *    @param[in] conn_handle Connection handle.
 *
 *    @return BLE_HOST_ERR_SUCC start new pairing success.
 *             - BLE_L2CAP_ERR(BLE_L2CAP_ERR_INVALID_CONN_HANDLE) invalid connection handle.
 *             - BLE_L2CAP_ERR(BLE_L2CAP_ERR_SMP_NOT_SUPPORTED) not support SMP pairing.
 *             - BLE_L2CAP_ERR(BLE_L2CAP_ERR_SMP_BUSY) SMP busy, not need to start pairing.
 *
 *    @note This function call success, whether paired device or not, host will send pairing request to peer device, and start pairing.
 */
int ble_host_smp_start_new_pairing_central(uint16_t conn_handle);

/**
 *    @brief BLE Host SMP start pairing.
 *
 *    @param[in] conn_handle Connection handle.
 *
 *    @return BLE_HOST_ERR_SUCC start pairing success.
 *             - BLE_L2CAP_ERR(BLE_L2CAP_ERR_INVALID_CONN_HANDLE) invalid connection handle.
 *             - BLE_L2CAP_ERR(BLE_L2CAP_ERR_SMP_NOT_SUPPORTED) not support SMP pairing.
 *             - BLE_L2CAP_ERR(BLE_L2CAP_ERR_SMP_BUSY) SMP busy, not need to start pairing.
 *
 *    @note This function call success, if role is peripheral, similar called ble_host_smp_start_pairing_peripheral.
 *                                       If role is central, similar called ble_host_smp_start_pairing_central.
 */
int ble_host_smp_start_pairing(uint16_t conn_handle);

/**
 *    @brief BLE Host SMP set random address.
 *
 *    @param[in] addr Random address.
 *
 *    @return None.
 *
 *    @note This function will be called by ble_host_gap_set_set_static_addr api, user should not call it directly.
 */
void ble_host_smp_set_bd_addr_random(const uint8_t addr[6]);

/**
 *    @brief BLE Host SMP generate resolvable private address.
 *
 *    @param[out] addr Resolvable private address.
 *
 *    @return None.
 *
 *    @note Generate a random resolvable private address by local IRK, default local IRK is generate by host layer, using BD ADDR, user can set local IRK.
 *            This function will be called by ble_host_gap_set_resolvable_private_addr api, user should not call it directly.
 */
void ble_host_smp_generate_resolvable_private_addr(uint8_t addr[6]);

/**
 *   @brief Derivation of BR/EDR link key from LE LTK.
 *   @param[in] ltk Pointer to the input LTK.
 *   @param[in] use_ct2 True if both devices set CT2 = 1, False otherwise.
 *   @param[out] link_key Pointer to the output link key.
 *
 *   @note The BR/EDR link key is derived using the SMP Toolbox function h6 and h7.
 */
void ble_host_smp_toolbox_derivation_bt_link_key(const uint8_t ltk[16], bool use_ct2, uint8_t link_key[16]);

/**
 *   @brief Derivation of LE LTK from BR/EDR link key.
 *   @param[in] link_key Pointer to the input link key.
 *   @param[in] use_ct2 True if both devices set CT2 = 1, False otherwise.
 *   @param[out] ltk Pointer to the output LTK.
 *
 *   @note The LTK is derived using the SMP Toolbox function h6 and h7.
 */
void ble_host_smp_toolbox_derivation_le_long_term_key(const uint8_t link_key[16], bool use_ct2, uint8_t ltk[16]);