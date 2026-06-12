/********************************************************************************************************
 * @file    tlk_ota_protocol_common.h
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
#ifndef TLK_OTA_PROTOCOL_COMMON_H
#define TLK_OTA_PROTOCOL_COMMON_H

#include "tlk_ota_types.h"

#define OTA_UINT32L_TO_ARRAY(value, array, offset)      \
    (array)[(offset) + 0] = ((value) & 0xFF);           \
    (array)[(offset) + 1] = ((value) & 0xFF00) >> 8;    \
    (array)[(offset) + 2] = ((value) & 0xFF0000) >> 16; \
    (array)[(offset) + 3] = ((value) & 0xFF000000) >> 24;

#define OTA_ARRAY_TO_UINT32L(array, offset, value) \
    (value) = (array)[(offset) + 3];               \
    (value) <<= 8;                                 \
    (value) |= (array)[(offset) + 2];              \
    (value) <<= 8;                                 \
    (value) |= (array)[(offset) + 1];              \
    (value) <<= 8;                                 \
    (value) |= (array)[(offset) + 0];

#define OTA_ARRAY_TO_UINT16L(array, offset, value) \
    (value) = (array)[(offset) + 1];               \
    (value) <<= 8;                                 \
    (value) |= (array)[(offset) + 0];

#define TLKMW_OTA_COMMON_SAVE_SIGN 0x3E

//note: if change the storage format, the version number must be changed as well.
//example 0x10->0x11
#define TLKMW_OTA_COMMON_SAVE_VER     0x1B

#define TLKMW_TINYSQL_OTA_INFO_ADDR0  0x10000
#define TLKMW_TINYSQL_OTA_INFO_SIZE   0x1000 //8k

#define TLKMW_TINYSQL_IMG_HEADER_ADDR 0x12000 //64kboot + 8k ota info
#define TLKMW_TINYSQL_IMG_HEADER_SIZE 0x1000  //4K
#define TLKMW_OTA_FW_NUMB_MAX         253     //(4k - other image data)

#define TLKMW_OTA_TRANS_MTU_SIZE      256
#define TLKMW_OTA_SHAKE_INTV          0x10

#define TLKMW_OTA_TRANS_TIMEOUT_INTV  100 //100ms
#define TLKMW_OTA_TRANS_TIMEOUT       10  //100ms * 10

#define TLKMW_OTA_NOTIFY_ARRAY_NUM    4
#define TLKMW_OTA_SUPPORT_CHN_NUM     4

typedef struct
{
    uint8_t sign[16];
} sTlk_ota_sign_t;

typedef struct
{
    uint32_t backup_addr;
    uint32_t user_area_addr;

    uint32_t min_system_mode : 4;
    uint32_t min_system_flag : 2;
    uint32_t flag_resv       : 26;

    uint8_t default_app_mode;
    uint8_t switch_app_mode;
    uint8_t resv[2];

    uint8_t break_index;
    uint8_t ota_status;
    uint8_t resv1[2];

    uint32_t break_offset;
    uint32_t ota_total_recv_len;
    uint32_t resv2;

    sTlk_ota_sign_t boot_sign;
    sTlk_ota_sign_t cur_app_sign;
    sTlk_ota_sign_t ota_app_sign;
    sTlk_ota_sign_t factory_app_sign;
} sTlk_boot_and_ota_cfg_t;

enum
{
    TLKMW_OTA_TRANS_CHN_NONE = 0,
    TLKMW_OTA_TRANS_CHN_UART,
    TLKMW_OTA_TRANS_CHN_BT_SPP,
    TLKMW_OTA_TRANS_CHN_BT_ATT,
    TLKMW_OTA_TRANS_CHN_BLE_GENERAL_MODE,
    TLKMW_OTA_TRANS_CHN_BLE_PREVIOUS_MODE,
    TLKMW_OTA_TRANS_CHN_TPSLL_DONGLE,
    TLKMW_OTA_TRANS_CHN_MAX,
};

typedef enum
{
    TLKMW_OTA_PARAM_TYPE_NONE = 0,
    TLKMW_OTA_PARAM_MTU_SIZE,
    TLKMW_OTA_PARAM_SHAKE_INTV,
    TLKMW_OTA_PARAM_TYPE_MAX,
} TLKMW_OTA_PARAM_TYPE;

typedef struct
{
    uint8_t channel;
    uint8_t resv[3];
    uint32_t (*get_ota_param)(uint32_t taskID, uint8_t param_type, void *UserArg);
    int (*send)(uint32_t taskID, uint8_t *pData, uint16_t dataLen, void *UserArg);
    int (*recv)(uint32_t taskID, uint8_t *pData, uint16_t dataLen, void *UserArg);
} sTlkMwUnitIntf_t;

typedef struct
{
    uint16_t threadID; // thread task ID
    uint16_t resv;
    void (*notify)(void *pData, uint8_t dataLen);
} sTlkMwNotifyUnit_t;

typedef struct
{
    uint8_t  optChn;
    uint8_t  resv[3];
    uint32_t busy_taskID;

    sTlkMwUnitIntf_t   intf[TLKMW_OTA_SUPPORT_CHN_NUM];
    sTlkMwNotifyUnit_t notifyCB[TLKMW_OTA_NOTIFY_ARRAY_NUM];
} sTlkMwOtaCommon_t;

typedef struct
{
    uint8_t  channel;
    uint8_t  status;
    uint8_t  reason;
    uint8_t  resv;
    uint32_t taskID; //ota task ID
} sTlkMwNotifyEvent_t;

/**
 * @brief      Initialize OTA common functionality
 * @param      none
 * @return     int - OTA_NONE if success, error code otherwise
 */
int tlkmw_ota_common_init(void);

/**
 * @brief      Get busy task ID for OTA
 * @param      none
 * @return     uint32_t - busy task ID
 */
uint32_t tlkmw_ota_common_get_busy_taskid(void);

/**
 * @brief      Get busy channel for OTA
 * @param[in]  none
 * @param[out] none
 * @return     uint8_t - busy channel
 */
uint8_t tlkmw_ota_common_get_busy_channel(void);


uint32_t tlkmw_ota_common_get_param(uint8_t param_type, void *userArg);
/**
 * @brief      Register channel interface for OTA
 * @param[in]  pInterface - pInterface
 * @return     int - OTA_NONE if success, error code otherwise
 */
int tlkmw_ota_register_chn_interface(sTlkMwUnitIntf_t *pInterface);

/**
 * @brief      Register OTA notification callback
 * @param[in]  threadID - thread identifier
 * @param[in]  notify   - notification function pointer
 * @return     int - OTA_NONE if success, error code otherwise
 */
int tlkmw_ota_register_notify_callback(uint16_t threadID, void (*notify)(void *pData, uint8_t dataLen));

/**
 * @brief      Send data via OTA common interface
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[in]  UserArg - user argument
 * @return     none
 */
void tlkmw_ota_common_send_data(uint8_t *pData, uint16_t dataLen, void *UserArg);

/**
 * @brief      Receive data via OTA common interface
 * @param[in]  taskID  - task identifier
 * @param[in]  channel - communication channel
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @return     none
 */
void tlkmw_ota_common_recv_data(uint32_t taskID, uint8_t channel, uint8_t *pData, uint16_t dataLen);

/**
 * @brief      Write data to flash memory
 * @param[in]  addr - flash address to write to
 * @param[in]  len  - length of data to write
 * @param[in]  buf  - pointer to data buffer
 * @return     none
 */
void tlkmw_ota_common_write(unsigned long addr, unsigned long len, unsigned char *buf);

/**
 * @brief      Get full flash size
 * @param      none
 * @return     int - flash size in bytes
 */
int tlk_nvds_get_full_size(void);

/**
 * @brief      Reset OTA common functionality
 * @param      none
 * @return     none
 */
void tlkmw_ota_common_reset(void);

#endif // #ifndef TLK_OTA_PROTOCOL_COMMON_H
