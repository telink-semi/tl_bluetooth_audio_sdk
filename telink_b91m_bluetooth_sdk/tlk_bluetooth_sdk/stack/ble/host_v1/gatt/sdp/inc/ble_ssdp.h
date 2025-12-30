/********************************************************************************************************
 * @file    ble_ssdp.h
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


// SSDP: single service discovery procedure.

#define GATT_SSDP_MAX_INCLUDE_SIZE  4

#define GATT_SSDP_SERVICE_NOT_FOUND 0xFF
#define GATT_SSDP_SERVICE_FOUND_END 0x00

/*
 * @param conn_handle --- acl connection handle.
 * @param count --- 0xFF:mean not found service uuid. 0x00: mean found service uuid ending. 0x01-0xFE: found service uuid count.
 * @param start_handle --- service uuid starting attribute handle.
 * @param end_handle --- service uuid ending attribute handle.
 */
typedef void (*gatt_ssdp_found_service_callback)(uint16_t conn_handle, uint8_t count, uint16_t start_handle, uint16_t end_handle);

/*
 * @param conn_handle --- acl connection handle.
 * @param start_handle --- found include uuid start attribute handle.
 * @param end_handle --- found include uuid end attribute handle.
 * @return true --- will discovery include characteristic uuid.
 */
typedef bool (*gatt_ssdp_found_include_callback)(uint16_t conn_handle, uint8_t count, uint16_t start_handle, uint16_t end_handle);

/*
 * @param conn_handle --- acl connection handle.
 * @param uuid --- unknown include uuid.
 * @param start_handle --- found include uuid start attribute handle.
 * @param end_handle --- found include uuid end attribute handle.
 */
typedef void (*gatt_ssdp_found_unknown_include_callback)(uint16_t conn_handle, uint8_t count, const struct att_uuid *uuid, uint16_t start_handle, uint16_t end_handle);

/*
 * @param conn_handle --- acl connection handle.
 * @param service_count --- RFU.
 * @param properties --- characteristic properties. reference ATT_READ_BY_TYPE_RSP.
 * @param value_handle --- characteristic value handle.
 */
typedef void (*gatt_ssdp_found_characteristic_callback)(uint16_t conn_handle, uint8_t service_count, uint8_t properties, uint16_t value_handle);

/*
 * @param conn_handle --- acl connection handle.
 * @param uuid --- characteristic Descriptors uuid. reference ATT_FIND_INFORMATION_RSP.
 * @param attr_handle --- characteristic Descriptors attribute handle.
 */
typedef void (*gatt_ssdp_found_desc_callback)(uint16_t conn_handle, const struct att_uuid *uuid, uint16_t attr_handle);

/*
 * @param conn_handle --- acl connection handle.
 * @param ccc_handle --- CCC value handle.
 * @param result --- subscribe client characteristic configuration result. ATT_ERROR_CODE.
 */
typedef void (*gatt_ssdp_subscribe_ccc_callback)(uint16_t conn_handle, uint16_t ccc_handle, uint8_t result);

/*
 * @param conn_handle --- acl connection handle.
 * @param value_handle --- read characteristic value attribute handle.
 * @param[out] read --- read characteristic value storage location.
 * @param[out] read_len --- read characteristic value length storage location.
 * @param[out] read_max_size --- can read characteristic value maximum length.
 * if read is not NULL, must set read_max_size. read_len Not necessary.
 * use read/read_len/read_max_size, gap layer will write characteristic value to read.
 * use rdCbFunc:GAP layer will GATT layer callback pass to higher level.
 */
typedef void (*gatt_ssdp_start_read_value_callback)(uint16_t conn_handle, uint16_t value_handle, uint8_t **read, uint16_t **read_len, uint16_t *read_max_size);

/*
 * @param conn_handle --- acl connection handle.
 * @param uuid --- unknown characteristic uuid.
 * @param properties --- characteristic properties. reference ATT_READ_BY_TYPE_RSP.
 * @param value_handle --- characteristic value handle.
 */
typedef void (*gatt_ssdp_found_unknown_characteristic_callback)(uint16_t conn_handle, const struct att_uuid *uuid, uint8_t properties, uint16_t value_handle);

/** @brief  GATT Single service discovery service information. */
struct ble_gatt_ssdp_service_info
{
    //service uuid.
    const struct att_uuid *service_uuid;
    //found service uuid callback function.
    gatt_ssdp_found_service_callback service_callback;
};

/** @brief  GATT Single service discovery single characteristic. */
struct gatt_ssdp_characteristic
{
    struct
    {
        //Automatically subscribe to the notify property, if had.
        bool subscribe_notify : 1;
        //Automatically subscribe to the indicate property, if had.
        bool subscribe_indicate : 1;
        //found Descriptors, if had.
        bool found_descriptor : 1;
        //Automatically read characteristic value, if had read property.
        bool read_value : 1;
    };

    //characteristic uuid.
    const struct att_uuid *characteristic_uuid;
    //found characteristic uuid callback function.
    gatt_ssdp_found_characteristic_callback characteristic_callback;
    //found characteristic Descriptors uuid callback function.
    gatt_ssdp_found_desc_callback desc_callback;
    //subscribe client characteristic configuration callback function.
    gatt_ssdp_subscribe_ccc_callback subscribe_ccc_callback;
    //want read characteristic value callback function.
    gatt_ssdp_start_read_value_callback read_value_callback;
};

/** @brief  GATT Single service discovery characteristic information. */
struct gatt_ssdp_characteristic_info
{
    //supported characteristic uuid size.
    uint8_t characteristic_size;
    //discovery characteristic uuid table.
    const struct gatt_ssdp_characteristic *characteristic;
    //found unknown characteristic uuuid callback function.
    gatt_ssdp_found_unknown_characteristic_callback unknown_characteristic_callback;
};

/** @brief  GATT Single service discovery single include. */
struct ble_gatt_ssdp_include
{
    //discovery include service uuid.
    const struct att_uuid *include_uuid;
    //include characteristic uuid table.
    struct gatt_ssdp_characteristic_info characteristic;
    //found include service uuid callback function.
    gatt_ssdp_found_include_callback include_callback;
};

struct ble_gatt_ssdp_include_info
{
    //supported discovery include uuid size.
    uint8_t include_size;
    //include discovery information table.
    const struct ble_gatt_ssdp_include *include[GATT_SSDP_MAX_INCLUDE_SIZE];
    //found unknown include uuid callback function.
    gatt_ssdp_found_unknown_include_callback unknown_include_callback;
};

/** @brief  GATT Single service discovery list. */
struct ble_host_gatt_ssdp_list
{
    //discovery service uuid maximum count.
    uint8_t max_service_count;
    // service uuid information.
    const struct ble_gatt_ssdp_service_info service;
    // characteristic uuid information.
    const struct gatt_ssdp_characteristic_info characteristic_table;
    const struct ble_gatt_ssdp_include_info    include_table;
};

struct ble_gatt_ssdp_no_include_list
{
    // discovery service uuid maximum count.
    uint8_t max_service_count;
    // service uuid information.
    const struct ble_gatt_ssdp_service_info service;
    // characteristic uuid information.
    const struct gatt_ssdp_characteristic_info characteristic_table;
};

/**
 *   @brief  Initialize the single service discovery module.
 *
 *   This function initializes the single service discovery module. It should be called once at the
 *   beginning of the application.
 *
 *   @return None.
 */
void ble_host_gatt_ssdp_init(void);

/**
 *   @brief  BLE GATT Client start discovery specific service, based on the given ssdp_list.
 *
 *   @param[in] conn_handle --- acl connection handle.
 *   @param[in] ssdp_list --- single service discovery list.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *
 *   @note   High layer can discovery multiple services, if gatt layer buffer is enough.
 *           If remote device only support ATT, the module will be discovered services one after another.
 *           If remote device support EATT connection, the module will automatically use the EATT mode for service discovery.
 */
int ble_host_gatt_ssdp_start(uint16_t conn_handle, const struct ble_host_gatt_ssdp_list *ssdp_list);

/**
 *   @brief  BLE GATT Client start discovery specific service, based on the given ssdp_list.
 *             If service has no include, should use this function.
 *
 *   @param[in] conn_handle --- acl connection handle.
 *   @param[in] ssdp_list --- single service discovery list.
 *
 *   @return BLE_HOST_ERR_SUCC if the request is sent successfully, otherwise an error code.
 *
 *   @note   High layer can discovery multiple services, if gatt layer buffer is enough.
 *           If remote device only support ATT, the module will be discovered services one after another.
 *           If remote device support EATT connection, the module will automatically use the EATT mode for service discovery.
 */
int ble_host_gatt_ssdp_start_no_include(uint16_t conn_handle, const struct ble_gatt_ssdp_no_include_list *ssdp_list);
