/********************************************************************************************************
 * @file    cap_device_table.h
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
#ifndef CAP_DEVICE_TABLE_H
#define CAP_DEVICE_TABLE_H

struct scan_device_info
{
    uint8_t addr[6];
    uint8_t addr_type;
    char    name[32];
    bool    rsi_flags;
    uint8_t rsi[6]; // Resolving Set Identifier
};

struct connect_device_info
{
    uint16_t conn_handle;
    uint16_t cis_handle;
    uint8_t  cis_streaming;
    uint8_t  cis_created;
    uint8_t  addr[6];
    uint8_t  addr_type;

    struct
    {
        uint16_t csis_support : 1;
        uint16_t ascs_support : 1;
        uint16_t pacs_support : 1;
        uint16_t hid_support  : 1;
    };

    bool    sirk_flags;
    uint8_t sirk[16];
};

#define MAX_SET_NUMBER 2

struct paired_device_info
{
    uint8_t device_id; //flash
    uint8_t set_num;   //flash

    struct
    {
        uint8_t music_index; //flash
        uint8_t voice_index; //flash
    } prefer_codec;          //flash

    struct
    {
        uint32_t smp_index; //flash
        uint8_t  set_index;
        uint16_t cis_handle;
        uint8_t  addr[6];
        uint8_t  addr_type;
    } set_info[MAX_SET_NUMBER];
};

/**
 * @brief       Initializes the CAP device tables in memory.
 * @return      none.
 * @note        Call once during CAP stack startup.
 */
void cap_device_table_device_init(void);

/**
 * @brief       Notifies that paired device data should be persisted to flash.
 * @return      none.
 * @note        Typically triggered after modifications to the paired table.
 */
void cap_device_table_paired_device_save_to_flash_notify(void);

/**
 * @brief       Inserts or updates a scanned device entry.
 * @param[in]   addr        - pointer to the peer address (6 bytes).
 * @param[in]   addr_type   - peer address type.
 * @param[in]   name        - pointer to the device name string.
 * @param[in]   rsi_flags   - flags indicating RSI validity.
 * @param[in]   rsi         - pointer to the RSI buffer (6 bytes).
 * @return      0 on success, negative value on failure.
 */
int cap_device_table_scan_device_insert(const uint8_t *addr, uint8_t addr_type, const char *name, uint8_t rsi_flags, const uint8_t *rsi);

/**
 * @brief       Finds a scanned device by address.
 * @param[in]   addr    - pointer to the peer address (6 bytes).
 * @return      Pointer to the scan device info, or NULL if not found.
 */
struct scan_device_info *cap_device_table_scan_device_find(const uint8_t *addr);

/**
 * @brief       Inserts an entry into the connected device table.
 * @param[in]   addr        - pointer to the peer address (6 bytes).
 * @param[in]   addr_type   - peer address type.
 * @param[in]   handle      - ACL connection handle.
 * @return      0 on success, negative value on failure.
 */
int cap_device_table_conn_device_insert(const uint8_t *addr, uint8_t addr_type, uint16_t handle);

/**
 * @brief       Finds a connected device by ACL handle.
 * @param[in]   acl_handle  - ACL connection handle.
 * @return      Pointer to the connect device info, or NULL if not found.
 */
struct connect_device_info *cap_device_table_conn_device_find_by_handle(const uint16_t acl_handle);

/**
 * @brief       Finds a connected device by address.
 * @param[in]   addr    - pointer to the peer address (6 bytes).
 * @return      Pointer to the connect device info, or NULL if not found.
 */
struct connect_device_info *cap_device_table_conn_device_find_by_addr(const uint8_t *addr);

/**
 * @brief       Finds a connected device by SIRK.
 * @param[in]   addr    - pointer to the peer address (6 bytes).
 * @param[in]   sirk    - pointer to the SIRK buffer (16 bytes).
 * @return      Pointer to the connect device info, or NULL if not found.
 */
struct connect_device_info *cap_device_table_conn_device_find_by_sirk(const uint8_t *addr, const uint8_t *sirk);

/**
 * @brief       Finds a connected device by table index.
 * @param[in]   index   - zero-based index.
 * @return      Pointer to the connect device info, or NULL if out of range.
 */
struct connect_device_info *cap_device_table_conn_device_find_by_index(uint8_t index);

/**
 * @brief       Removes a connected device entry by ACL handle.
 * @param[in]   handle  - ACL connection handle.
 * @return      none.
 */
void cap_device_table_conn_device_remove_by_handle(uint16_t handle);

/**
 * @brief       Removes a connected device entry by address.
 * @param[in]   addr    - pointer to the peer address (6 bytes).
 * @return      none.
 */
void cap_device_table_conn_device_remove_by_addr(const uint8_t *addr);

/**
 * @brief       Gets the number of CIS connections for a CSIS set ID.
 * @param[in]   id  - CSIS set identifier.
 * @return      Number of matching connections.
 */
uint8_t cap_device_table_get_cis_conn_device_num_by_id(uint8_t id);

/**
 * @brief       Gets the number of connections associated with a CSIS set ID.
 * @param[in]   id  - CSIS set identifier.
 * @return      Number of matching connections.
 */
uint8_t cap_device_table_get_conn_device_num_by_id(uint8_t id);

/**
 * @brief       Gets the number of connections sharing the same ACL handle.
 * @param[in]   handle  - ACL connection handle.
 * @return      Count of matching entries.
 */
uint8_t cap_device_table_get_conn_device_num_by_handle(uint16_t handle);

/**
 * @brief       Returns the size of the connected device table.
 * @return      Number of entries currently stored.
 */
int cap_device_table_conn_device_size(void);

/**
 * @brief       Removes a paired device entry by address.
 * @param[in]   addr    - pointer to the peer address (6 bytes).
 * @return      none.
 */
void cap_device_table_paired_device_remove(const uint8_t *addr);

/**
 * @brief       Finds a paired device by address and returns its set index.
 * @param[in]   addr        - pointer to the peer address (6 bytes).
 * @param[out]  set_index   - pointer to store the set index.
 * @return      Pointer to the paired device info, or NULL if not found.
 */
struct paired_device_info *cap_device_table_paired_device_find_by_addr_1(const uint8_t *addr, uint8_t *set_index);

/**
 * @brief       Inserts or updates a paired device entry.
 * @param[in]   device_id   - identifier stored in flash.
 * @param[in]   addr        - pointer to the peer address (6 bytes).
 * @param[in]   addr_type   - peer address type.
 * @param[in]   set_number  - CSIS set number.
 * @return      0 on success, negative value on failure.
 */
int cap_device_table_paired_device_insert(uint8_t device_id, const uint8_t *addr, uint8_t addr_type, uint16_t set_number);

/**
 * @brief       Finds a paired device by ACL connection handle.
 * @param[in]   handle  - ACL connection handle.
 * @return      Pointer to the paired device info, or NULL if not found.
 */
struct paired_device_info *cap_device_table_paired_device_find_by_connect_handle(uint16_t handle);

/**
 * @brief       Finds a paired device by address.
 * @param[in]   addr    - pointer to the peer address (6 bytes).
 * @return      Pointer to the paired device info, or NULL if not found.
 */
struct paired_device_info *cap_device_table_paired_device_find_by_addr(const uint8_t *addr);

/**
 * @brief       Finds a paired device by SMP index.
 * @param[in]   smp_index   - SMP pairing index.
 * @return      Pointer to the paired device info, or NULL if not found.
 */
struct paired_device_info *cap_device_table_paired_device_find_by_smp_index(uint32_t smp_index);

/**
 * @brief       Finds a paired device by its stored ID.
 * @param[in]   id  - paired device identifier.
 * @return      Pointer to the paired device info, or NULL if not found.
 */
struct paired_device_info *cap_device_table_paired_device_find_by_id(uint8_t id);

/**
 * @brief       Retrieves the CSIS set index associated with a paired address.
 * @param[in]   addr    - pointer to the peer address (6 bytes).
 * @return      Set index value, or 0xFF if not found.
 */
uint8_t cap_device_table_paired_device_get_set_index(const uint8_t *addr);

/**
 * @brief       Retrieves the paired device ID associated with an address.
 * @param[in]   addr    - pointer to the peer address (6 bytes).
 * @return      Device ID value, or 0xFF if not found.
 */
uint8_t cap_device_table_paired_device_get_id_by_addr(const uint8_t *addr);

/**
 * @brief       Returns the number of stored paired devices.
 * @return      Number of entries currently stored.
 */
int cap_device_table_paired_device_size(void);
#endif //CAP_DEVICE_TABLE_H