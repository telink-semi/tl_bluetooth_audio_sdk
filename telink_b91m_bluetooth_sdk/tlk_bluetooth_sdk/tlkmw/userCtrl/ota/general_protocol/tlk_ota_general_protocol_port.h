/********************************************************************************************************
 * @file    tlk_ota_general_protocol_port.h
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
#ifndef TLK_OTA_GENERAL_PROTOCOL_PORT_H
#define TLK_OTA_GENERAL_PROTOCOL_PORT_H

#include "tlk_ota_general_protocol.h"

#define TLKMW_OTA_TRANS_MAX_PACK_SIZE 1024
#define TLKMW_OTA_WRITE_CACHE_SIZE    4096

typedef struct
{
    uint8_t *p_recv_cache_buff;

    uint8_t  channel;
    uint8_t  recv_cache_opcode;
    uint16_t resv;

    uint16_t recv_cache_len;
    uint16_t recv_cache_index;
} tlk_ota_general_protocol_t;

enum
{
    TLK_OTA_OPC_NONE = 0,
    /*APP to Device*/
    TLK_OTA_OPC_VERSION_REQ                 = 0x01,
    TLK_OTA_OPC_PARAM_NEGO_REQ              = 0x02,
    TLK_OTA_OPC_BREAK_INFO_REQ              = 0x03,
    TLK_OTA_OPC_TOTAL_FW_DESCRRIPTOR_NOTIFY = 0x04,
    TLK_OTA_OPC_START_OTA_REQ               = 0x05,
    TLK_OTA_OPC_CUR_FW_DESCRRIPTOR_RSP      = 0x06,
    TLK_OTA_OPC_CUR_FW_DATA_RSP             = 0x07,
    TLK_OTA_OPC_OTA_TRANS_END_ACK           = 0x08,

    /*Device to APP*/
    TLK_OTA_OPC_VERSION_RSP              = 0x41,
    TLK_OTA_OPC_PARAM_NEGO_RSP           = 0x42,
    TLK_OTA_OPC_BREAK_INFO_RSP           = 0x43,
    TLK_OTA_OPC_TOTAL_FW_DESCRRIPTOR_RSP = 0x44,
    TLK_OTA_OPC_START_OTA_RSP            = 0x45,
    TLK_OTA_OPC_CUR_FW_DESCRRIPTOR_REQ   = 0x46,
    TLK_OTA_OPC_CUR_FW_DATA_REQ          = 0x47,
    TLK_OTA_OPC_SYNC_LOSS                = 0x48,

    /*Either side*/
    TLK_OTA_OPC_OTA_TRANS_END = 0x81,

    TLK_OTA_OPC_MAX = TLK_OTA_OPC_OTA_TRANS_END_ACK,
};

typedef enum
{
    TLK_OTA_STAGE_NONE = 0,
    TLK_OTA_STAGE_IDLE,
    TLK_OTA_STAGE_NEGO,
    TLK_OTA_STAGE_BREAK_NEGO,
    TLK_OTA_STAGE_FW_IMG_IND,
    TLK_OTA_STAGE_FW_DATA_TRANS,
    TLK_OTA_STAGE_WAITING_END_ACK,
    TLK_OTA_STAGE_MAX,
} TLK_OTA_STAGE_ENUM;

enum
{
    TLK_OTA_PACK_TYPE_COMPLETE = 0,
    TLK_OTA_PACK_TYPE_START,
    TLK_OTA_PACK_TYPE_CONTINUE,
    TLK_OTA_PACK_TYPE_END,
};

enum
{
    TLK_OTA_RESULT_SUCCESS = 0,
    TLK_OTA_RESULT_DATA_PACKET_SEQ_ERR,
    TLK_OTA_RESULT_PACKET_INVALID,
    TLK_OTA_RESULT_DATA_CRC_ERR,
    TLK_OTA_RESULT_WRITE_FLASH_ERR,
    TLK_OTA_RESULT_OTA_INCOMPLETE,
    TLK_OTA_RESULT_FLOW_ERR,
    TLK_OTA_RESULT_FW_CHECK_ERR,
    TLK_OTA_RESULT_VERSION_COMPARE_ERR,
    TLK_OTA_RESULT_PDU_LEN_ERR,
    TLK_OTA_RESULT_FIRMWARE_MARK_ERR,
    TLK_OTA_RESULT_FW_SIZE_ERR,
    TLK_OTA_RESULT_DATA_PACKET_TIMEOUT,
    TLK_OTA_RESULT_TIMEOUT = 0x0D,
    TLK_OTA_RESULT_CONNECTION_TERMINATE,
    TLK_OTA_RESULT_TOTAL_FW_DESCRRIPTORS_ERR,
    TLK_OTA_RESULT_CUR_FW_DESCRRIPTORS_ERR,
    TLK_OTA_RESULT_PARAM_NEGO_ERR,
    TLK_OTA_RESULT_BREAK_POINT_ERR,
    TLK_OTA_RESULT_MAX = 0xFF,
};

/**
 * @brief      Calculate CRC32 checksum
 * @param[in]  crc     - initial CRC value
 * @param[in]  pStart  - pointer to data buffer
 * @param[in]  uSize   - size of data
 * @return     unsigned int - calculated CRC32 checksum
 */
unsigned int calculate_CRC32(unsigned int crc, unsigned char *pStart, unsigned int uSize);

/**
 * @brief      Reset OTA general protocol
 * @param      none
 * @return     none
 */
void tlk_ota_general_protocol_reset(void);

/**
 * @brief      Handle OTA version check
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[in]  userArg - user argument
 * @return     int - OTA_NONE if success
 */
int tlkmw_ota_version_check(uint8_t *pData, uint16_t dataLen, void *userArg);

/**
 * @brief      Handle OTA parameter negotiation
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[in]  userArg - user argument
 * @return     int - OTA_NONE if success
 */
int tlkmw_ota_param_negotiation(uint8_t *pData, uint16_t dataLen, void *userArg);

/**
 * @brief      Handle OTA break point request
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[in]  userArg - user argument
 * @return     int - OTA_NONE if success
 */
int tlkmw_ota_break_point_request(uint8_t *pData, uint16_t dataLen, void *userArg);

/**
 * @brief      Handle OTA total firmware descriptors check
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[in]  userArg - user argument
 * @return     int - OTA_NONE if success
 */
int tlkmw_ota_total_fw_descriptors_check(uint8_t *pData, uint16_t dataLen, void *userArg);

/**
 * @brief      Handle OTA start request
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[in]  userArg - user argument
 * @return     int - OTA_NONE if success
 */
int tlkmw_ota_start_ota_deal(uint8_t *pData, uint16_t dataLen, void *userArg);

/**
 * @brief      Handle OTA current firmware descriptor check
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[in]  userArg - user argument
 * @return     int - OTA_NONE if success
 */
int tlkmw_ota_cur_fw_descriptor_check(uint8_t *pData, uint16_t dataLen, void *userArg);

/**
 * @brief      Handle OTA current data deal
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[in]  userArg - user argument
 * @return     int - OTA_NONE if success
 */
int tlkmw_ota_cur_data_deal(uint8_t *pData, uint16_t dataLen, void *userArg);

/**
 * @brief      Handle OTA end deal
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[in]  userArg - user argument
 * @return     int - OTA_NONE if success
 */
int tlkmw_ota_end_deal(uint8_t *pData, uint16_t dataLen, void *userArg);

/**
 * @brief      Handle OTA end acknowledgment deal
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[in]  userArg - user argument
 * @return     int - OTA_NONE if success
 */
int tlkmw_ota_end_ack_deal(uint8_t *pData, uint16_t dataLen, void *userArg);

/**
 * @brief      Send OTA transfer end
 * @param[in]  result  - result code
 * @param[in]  userArg - user argument
 * @return     none
 */
void tlkmw_ota_send_trans_end(uint8_t result, void *userArg);

/**
 * @brief      Handle OTA end acknowledgment deal with special operations
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[in]  userArg - user argument
 * @return     int - OTA_NONE if success
 */
int tlkmw_ota_end_ack_deal_SpecialOperationsr(uint8_t *pData, uint16_t dataLen, void *userArg);

/**
 * @brief      Initialize customer OTA start
 * @param      none
 * @return     int - OTA_NONE if success
 */
int tlkmw_ota_start_customer_init(void);

/**
 * @brief      Notify OTA status
 * @param[in]  pBuffer - pointer to buffer
 * @param[in]  buffLen - buffer length
 * @param[in]  UserArg - user argument
 * @return     int - negative OTA_DEFINEDERR error code
 */
int tlk_ota_notify_status(uint8_t *pBuffer, uint32_t buffLen, void *UserArg);

/**
 * @brief      Send data via OTA general protocol
 * @param[in]  opcode  - operation code
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[in]  userArg - user argument
 * @return     none
 */
void tlk_ota_general_protocol_send_data(uint8_t opcode, uint8_t *pData, uint16_t dataLen, void *userArg);

/**
 * @brief      Receive data via OTA general protocol
 * @param[in]  taskID  - task identifier
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[in]  UserArg - user argument
 * @param[out] none
 * @return     int - OTA_NONE if success, error code otherwise
 */
int tlk_ota_general_protocol_recv_data(uint32_t taskID, uint8_t *pData, uint16_t dataLen, void *UserArg);

/**
 * @brief      Initialize OTA general protocol
 * @param[in]  pInterface - pointer to OTA interface
 * @return     int - OTA_NONE if success, error code otherwise
 */
int tlk_ota_general_protocol_init(nvds_ota_Interface_t pInterface);

#endif // #ifndef TLK_OTA_GENERAL_PROTOCOL_PORT_H
