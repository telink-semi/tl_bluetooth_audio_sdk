/********************************************************************************************************
 * @file    tlk_ota_general_protocol_example.h
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
#ifndef TLKMW_OTA_PROTO_DETAIL_H
#define TLKMW_OTA_PROTO_DETAIL_H

enum
{
    TLK_OTA_STATUS_NOINIT = 0,
    TLK_OTA_STATUS_IDLE,
    TLK_OTA_STATUS_BUSY,
};

typedef struct
{
    uint8_t  curFwNum;
    uint8_t  timeout;
    uint8_t  status;
    uint8_t  channel;
    uint16_t shakeIntv;
    uint16_t cache_size;

    uint32_t backAddr;
    uint32_t saveOffset;

    /*Single Fw trans*/
    uint32_t fwDataStartOffset; //Used for request data form APP, offset of total bin;
    uint32_t fwDataTotalSize;   //current fw total size;
    uint32_t fwDataRecvSize;    //already received size;
    uint32_t fwDataRecvNumb;    //current fw received number;
    uint32_t fwDataPendNumb;    //current fw pending number;
    uint32_t flash_save_size;   //current fw save size;

    tlk_ota_timer_handle_t timer;

    uint8_t              *p_cache_buffer; //Cache fw data, when the cache reaches the limit of TLKMW_OTA_WRITE_CACHE_SIZE, it is written to the flash.
    nvds_ota_Interface_t *ota_intf;
} sTlkMwOta_t;

/**
 * @brief   Load current image header from flash
 * @param   None
 * @return  OTA_NONE if successful, error code otherwise
 */
int tlkmw_ota_load_cur_img_header(void);

/**
 * @brief   Initialize the OTA protocol detail module
 * @param   pInterface - Pointer to OTA interface structure
 * @return  OTA_NONE if successful, error code otherwise
 */
int tlk_ota_general_protocol_detail_init(nvds_ota_Interface_t *pInterface);

/**
 * @brief   Get pointer to the OTA control structure
 * @param   None
 * @return  Pointer to the OTA control structure
 */
sTlkMwOta_t *tlkmw_get_otaCtrl(void);

/**
 * @brief   Update the OTA status and notify the application
 * @param   pData - Pointer to the data containing status and reason
 * @param   dataLen - Length of the data in bytes
 * @param   userArg - User argument (not used)
 * @return  None
 */
void tlkmw_ota_update_ota_status(uint8_t *pData, uint16_t dataLen, void *userArg);

/**
 * @brief   Reset the OTA control module to initial state
 * @param   None
 * @return  None
 */
void tlk_ota_general_protocol_detail_reset(void);

#endif // #ifndef TLKMW_OTA_PROTO_DETAIL_H
