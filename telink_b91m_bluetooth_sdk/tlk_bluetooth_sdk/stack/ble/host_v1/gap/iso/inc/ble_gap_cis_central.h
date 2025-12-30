/********************************************************************************************************
 * @file    ble_gap_cis_central.h
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

enum ble_host_gap_cig_sca
{
    BLE_HOST_GAP_CIG_SCA_PPM_251_500 = 0x00,
    BLE_HOST_GAP_CIG_SCA_PPM_151_250 = 0x01,
    BLE_HOST_GAP_CIG_SCA_PPM_101_150 = 0x02,
    BLE_HOST_GAP_CIG_SCA_PPM_76_100  = 0x03,
    BLE_HOST_GAP_CIG_SCA_PPM_51_75   = 0x04,
    BLE_HOST_GAP_CIG_SCA_PPM_31_50   = 0x05,
    BLE_HOST_GAP_CIG_SCA_PPM_21_30   = 0x06,
    BLE_HOST_GAP_CIG_SCA_PPM_0_20    = 0x07,
};

enum ble_host_gap_cig_phy_prefer
{
    BLE_HOST_GAP_CIG_PHY_PREFER_1M    = (1 << (0)),
    BLE_HOST_GAP_CIG_PHY_PREFER_2M    = (1 << (1)),
    BLE_HOST_GAP_CIG_PHY_PREFER_CODED = (1 << (2)),
};

struct ble_host_gap_cis_central_set_cig_params
{
    uint8_t cig_id;         /** < used to identify the CIG. */
    uint8_t worst_case_sca; /** < worst case SCA value, refer to enum ble_host_gap_cig_sca. */
    bool    sequential;     /** < if true, packing is sequential, else, packing is interleaved. */
    bool    framing;        /** < if true, framing is used, else, no framing is used. */

    struct
    {
        uint32_t sdu_interval;          /** < unit is us, range 0x0000FF to 0x0FFFFF. */
        uint16_t max_transport_latency; /** < unit is ms, range 0x0005 to 0x0FA0. */
    } c_to_p, p_to_c;

    uint8_t cis_count; /** < number of CISes to be created. */

    struct
    {
        uint8_t cis_id; /** < CIS ID. */

        struct
        {
            uint16_t max_sdu; /** < Maximum SDU size. */
            uint8_t  phy;     /** < PHY. 0x01 - BLE 1M, 0x02 - BLE 2M, 0x04 - BLE Coded. */
            uint8_t  rtn;     /** < Retransmission number. */
        } cis_c_to_p, cis_p_to_c;
    } cis_param[BLE_HOST_GAP_CIS_SUPPORT_MAX_COUNT];

    ble_host_gap_cis_event_callback event_callback;
};

struct ble_gap_cis_central_evt_established
{
    uint16_t cis_conn_handle; /** < CIS connection handle. */
    uint8_t  status;          /** < Status of CIS establishment. 0x00 - success, other - failure. */
    uint8_t  nse;             /** < Maximum number of subevents in each CIS. */
    uint16_t iso_interval;    /** < ISO interval. range 0x0004 to 0x0C80, unit 1.25ms. */
    uint32_t cig_sync_delay;  /** < CIG synchronization delay. range 0x0000EA to 0x7FFFFF, unit 1us. */
    uint32_t cis_sync_delay;  /** < CIS synchronization delay. range 0x0000EA to 0x7FFFFF, unit 1us. */

    struct central_transmit_param
    {
        uint8_t  phy;               /** < PHY. 0x01 - BLE 1M, 0x02 - BLE 2M, 0x03 - BLE Coded. */
        uint8_t  bn;                /** < The Burst Number. range 0x01 to 0x0F, 0x00 mean no isochronous data. */
        uint8_t  ft;                /** < The Flush Timeout. range 0x01 to 0xFF. */
        uint16_t max_pdu_size;      /** < Maximum PDU size. range 0x0000 to 0x00FB. */
        uint32_t transport_latency; /** < Transport latency. range 0x0000EA to 0x7FFFFF, unit 1us. */
    } c_to_p, p_to_c;
};

struct ble_gap_cis_central_evt_cis_disconnected
{
    uint16_t conn_handle; /** < CIS Connection handle. */
    uint8_t  reason;      /** < Reason of disconnection. */
};

struct ble_gap_cis_central_evt_recv_data
{
    uint16_t conn_handle; /** < CIS Connection handle. */
    uint16_t data_len;
    uint8_t *data;
};

/**
 * @brief       Initialize the BLE host GAP CIS central module.
 * @return      none.
 */
void ble_host_gap_cis_central_init(void);

/**
 * @brief       Create a CIG ID and return it.
 * @return      the created CIG ID.
 */
uint8_t ble_host_gap_cis_central_create_cig_id(void);

/**
 * @brief       Delete a CIG ID.
 * @param[in]   cig_id      - the CIG ID to delete.
 * @return      true if the CIG ID was successfully deleted, false otherwise.
 */
bool ble_host_gap_cis_central_delete_cig_id(uint8_t cig_id);

/**
 * @brief       Set CIG parameters for CIS central.
 * @param[in]   params              - pointer to the CIG parameters structure.
 * @param[out]  cis_conn_handles    - pointer to the array to store CIS connection handles.
 * @return      BLE_HOST_ERR_SUCC if successful, otherwise an error code.
 */
int ble_host_gap_cis_central_set_cig_param(const struct ble_host_gap_cis_central_set_cig_params *params, uint16_t *cis_conn_handles);

/**
 * @brief       Remove a CIG for CIS central.
 * @param[in]   cig_id      - the CIG ID to remove.
 * @return      BLE_HOST_ERR_SUCC if successful, otherwise an error code.
 */
int ble_host_gap_cis_central_remove_cig(uint8_t cig_id);

/**
 * @brief       Create a CIS connection for CIS central.
 * @param[in]   acl_conn_handle    - the ACL connection handle.
 * @param[in]   cis_conn_handle    - the CIS connection handle.
 * @param[in]   callback            - the event callback function.
 * @return      BLE_HOST_ERR_SUCC if successful, otherwise an error code.
 */
int ble_host_gap_cis_central_create_connection(uint16_t acl_conn_handle, uint16_t cis_conn_handle, ble_host_gap_cis_event_callback callback);

/**
 * @brief       Disconnect a CIS connection for CIS central.
 * @param[in]   cis_conn_handle    - the CIS connection handle to disconnect.
 * @return      BLE_HOST_ERR_SUCC if successful, otherwise an error code.
 */
int ble_host_gap_cis_central_disconnect(uint16_t cis_conn_handle);
