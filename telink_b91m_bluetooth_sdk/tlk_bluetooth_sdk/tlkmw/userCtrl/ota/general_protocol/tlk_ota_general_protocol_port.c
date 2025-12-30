/********************************************************************************************************
 * @file    tlk_ota_general_protocol_port.c
 *
 * @brief   This is the source file for TLSR/TL
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
#include "drivers.h"
#include "tlk_ota_general_protocol.h"

static tlk_ota_general_protocol_t ota_general_ptotocol_ctrl;

static const unsigned int CRC32_Table[256] = {
    0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3, 0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988, 0x09B64C2B, 0x7EB17CBD,
    0xE7B82D07, 0x90BF1D91, 0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE, 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7, 0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
    0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5, 0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172, 0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B, 0x35B5A8FA, 0x42B2986C,
    0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59, 0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
    0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924, 0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D, 0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F,
    0x9FBFE4A5, 0xE8B8D433, 0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01, 0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
    0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457, 0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65, 0x4DB26158, 0x3AB551CE,
    0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB, 0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0, 0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
    0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F, 0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81,
    0xB7BD5C3B, 0xC0BA6CAD, 0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A, 0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683, 0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
    0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1, 0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7, 0xFED41B76, 0x89D32BE0,
    0x10DA7A5A, 0x67DD4ACC, 0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5, 0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
    0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79, 0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236, 0xCC0C7795, 0xBB0B4703,
    0x220216B9, 0x5505262F, 0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D, 0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
    0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713, 0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38, 0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21, 0x86D3D2D4, 0xF1D4E242,
    0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777, 0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
    0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2, 0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB, 0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5,
    0x47B2CF7F, 0x30B5FFE9, 0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF, 0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
    0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D};

/**
 * @brief      Calculate CRC32 checksum
 * @param[in]  crc     - initial CRC value
 * @param[in]  pStart  - pointer to data buffer
 * @param[in]  uSize   - size of data
 * @param[out] none
 * @return     unsigned int - calculated CRC32 checksum
 */
unsigned int calculate_CRC32(unsigned int crc, unsigned char *pStart, unsigned int uSize)
{
    unsigned int   uCRCValue;
    unsigned char *pData;

    /* init the start value */
    uCRCValue = crc;
    pData     = pStart;

    /* calculate CRC */
    while (uSize--) {
        uCRCValue = CRC32_Table[(uCRCValue ^ *pData++) & 0xFF] ^ (uCRCValue >> 8);
    }
    /* XOR the output value */
    return ~uCRCValue;
}

/**
 * @brief      Send data via OTA general protocol
 * @param[in]  opcode  - operation code
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[in]  userArg - user argument
 * @param[out] none
 * @return     none
 */
void tlk_ota_general_protocol_send_data(uint8_t opcode, uint8_t *pData, uint16_t dataLen, void *userArg)
{
    (void)userArg;

    uint8_t  pBuffer[256];
    uint16_t buffLen = 0;

    pBuffer[buffLen++] = 0x01; //OTA Packet Type.
    pBuffer[buffLen++] = (ota_general_ptotocol_ctrl.channel & 0xFF);
    pBuffer[buffLen++] = 0;
    pBuffer[buffLen++] = 0;
    pBuffer[buffLen++] = (opcode & 0xFF);
    pBuffer[buffLen++] = 0;
    pBuffer[buffLen++] = 0;

    if (dataLen != 0) {
        OTA_MEMCPY(pBuffer + buffLen, pData, dataLen);
        buffLen += dataLen;
    }

    pBuffer[2] = (buffLen & 0xFF);
    pBuffer[3] = ((buffLen >> 8) & 0xFF);

    tlkmw_ota_common_send_data(pBuffer, buffLen, userArg);
}

static void tlk_ota_general_protocol_clear_recv_cache()
{
    /*drop data cache.*/
    ota_general_ptotocol_ctrl.recv_cache_len    = 0;
    ota_general_ptotocol_ctrl.recv_cache_index  = 0;
    ota_general_ptotocol_ctrl.recv_cache_opcode = 0;
    OTA_MEMSET(ota_general_ptotocol_ctrl.p_recv_cache_buff, 0, TLKMW_OTA_TRANS_MAX_PACK_SIZE);
}

/**
 * @brief      Reset OTA general protocol
 * @param[in]  none
 * @param[out] none
 * @return     none
 */
__attribute__((weak)) void tlk_ota_general_protocol_reset(void)
{
    tlk_ota_general_protocol_clear_recv_cache();
    ota_general_ptotocol_ctrl.channel = 0;
    tlk_ota_general_protocol_detail_reset();
    tlkmw_ota_common_reset();
}

/**
 * @brief      Handle OTA version check
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[in]  userArg - user argument
 * @param[out] none
 * @return     int - OTA_NONE if success
 */
__attribute__((weak)) int tlkmw_ota_version_check(uint8_t *pData, uint16_t dataLen, void *userArg)
{
    (void)pData;
    (void)dataLen;
    (void)userArg;

    uint8_t  pBuffer[4];
    uint16_t buffLen = 0;

    uint32_t version   = 0xFFFFFFFF;
    pBuffer[buffLen++] = version & 0xff;
    pBuffer[buffLen++] = (version >> 8) & 0xff;
    pBuffer[buffLen++] = (version >> 16) & 0xff;
    pBuffer[buffLen++] = (version >> 24) & 0xff;
    tlk_ota_general_protocol_send_data(TLK_OTA_OPC_VERSION_RSP, pBuffer, buffLen, NULL);

    return OTA_NONE;
}

/**
 * @brief      Handle OTA parameter negotiation
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[in]  userArg - user argument
 * @param[out] none
 * @return     int - OTA_NONE if success
 */
__attribute__((weak)) int tlkmw_ota_param_negotiation(uint8_t *pData, uint16_t dataLen, void *userArg)
{
    (void)dataLen;
    (void)userArg;

    uint16_t recvPktSize = 0;
    OTA_ARRAY_TO_UINT16L(pData, 0, recvPktSize);

    uint8_t  pBuffer[4];
    uint16_t buffLen = 0;

    uint16_t value = TLKMW_OTA_TRANS_MTU_SIZE;
    value          = (recvPktSize > value) ? value : recvPktSize;

    pBuffer[buffLen++] = value & 0xff;
    pBuffer[buffLen++] = (value >> 8) & 0xff;

    value              = TLKMW_OTA_SHAKE_WIRELESS_INTV;
    pBuffer[buffLen++] = value & 0xff;
    pBuffer[buffLen++] = (value >> 8) & 0xff;

    tlk_ota_general_protocol_send_data(TLK_OTA_OPC_PARAM_NEGO_RSP, pBuffer, buffLen, NULL);

    return OTA_NONE;
}

/**
 * @brief      Handle OTA break point request
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[in]  userArg - user argument
 * @param[out] none
 * @return     int - OTA_NONE if success
 */
__attribute__((weak)) int tlkmw_ota_break_point_request(uint8_t *pData, uint16_t dataLen, void *userArg)
{
    (void)pData;
    (void)dataLen;
    (void)userArg;

    uint8_t  pBuffer[8];
    uint16_t buffLen = 0;

    /*Break point - last bin index, 0xFF means no break point.*/
    pBuffer[buffLen++] = 0xFF;
    /*Break point - last bin receive size, 0xFFFFFFFF means no break point.*/
    pBuffer[buffLen++] = 0xFF;
    pBuffer[buffLen++] = 0xFF;
    pBuffer[buffLen++] = 0xFF;
    pBuffer[buffLen++] = 0xFF;

    tlk_ota_general_protocol_send_data(TLK_OTA_OPC_BREAK_INFO_RSP, pBuffer, buffLen, NULL);

    return OTA_NONE;
}

/**
 * @brief      Handle OTA total firmware descriptors check
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[in]  userArg - user argument
 * @param[out] none
 * @return     int - OTA_NONE if success
 */
__attribute__((weak)) int tlkmw_ota_total_fw_descriptors_check(uint8_t *pData, uint16_t dataLen, void *userArg)
{
    (void)pData;
    (void)dataLen;
    (void)userArg;

    uint8_t ret = TLK_OTA_RESULT_SUCCESS; //means verify success

    tlk_ota_general_protocol_send_data(TLK_OTA_OPC_TOTAL_FW_DESCRRIPTOR_RSP, &ret, 1, NULL);

    return OTA_NONE;
}

/**
 * @brief      Handle OTA start request
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[in]  userArg - user argument
 * @param[out] none
 * @return     int - OTA_NONE if success
 */
__attribute__((weak)) int tlkmw_ota_start_ota_deal(uint8_t *pData, uint16_t dataLen, void *userArg)
{
    (void)pData;
    (void)dataLen;
    (void)userArg;

    uint8_t ret = TLK_OTA_RESULT_SUCCESS; //means ready

    tlk_ota_general_protocol_send_data(TLK_OTA_OPC_START_OTA_RSP, &ret, 1, NULL);

    return OTA_NONE;
}

/**
 * @brief      Handle OTA current firmware descriptor check
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[in]  userArg - user argument
 * @param[out] none
 * @return     int - OTA_NONE if success
 */
__attribute__((weak)) int tlkmw_ota_cur_fw_descriptor_check(uint8_t *pData, uint16_t dataLen, void *userArg)
{
    (void)pData;
    (void)dataLen;
    (void)userArg;

    uint8_t ret = TLK_OTA_RESULT_SUCCESS; //means verify success

    tlk_ota_general_protocol_send_data(TLK_OTA_OPC_CUR_FW_DESCRRIPTOR_REQ, &ret, 1, NULL);

    return OTA_NONE;
}

/**
 * @brief      Handle OTA current data deal
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[in]  userArg - user argument
 * @param[out] none
 * @return     int - OTA_NONE if success
 */
__attribute__((weak)) int tlkmw_ota_cur_data_deal(uint8_t *pData, uint16_t dataLen, void *userArg)
{
    (void)pData;
    (void)dataLen;
    (void)userArg;

    return OTA_NONE;
}

/**
 * @brief      Handle OTA end deal
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[in]  userArg - user argument
 * @param[out] none
 * @return     int - OTA_NONE if success
 */
__attribute__((weak)) int tlkmw_ota_end_deal(uint8_t *pData, uint16_t dataLen, void *userArg)
{
    (void)pData;
    (void)dataLen;
    (void)userArg;

    return OTA_NONE;
}

/**
 * @brief      Handle OTA end acknowledgment deal
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[in]  userArg - user argument
 * @param[out] none
 * @return     int - OTA_NONE if success
 */
__attribute__((weak)) int tlkmw_ota_end_ack_deal(uint8_t *pData, uint16_t dataLen, void *userArg)
{
    (void)pData;
    (void)dataLen;
    (void)userArg;

    return OTA_NONE;
}

/**
 * @brief      Send OTA transfer end
 * @param[in]  result  - result code
 * @param[in]  userArg - user argument
 * @param[out] none
 * @return     none
 */
__attribute__((weak)) void tlkmw_ota_send_trans_end(uint8_t result, void *userArg)
{
    (void)result;
    (void)userArg;
}

/**
 * @brief      Handle OTA end acknowledgment deal with special operations
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[in]  userArg - user argument
 * @param[out] none
 * @return     int - OTA_NONE if success
 */
__attribute__((weak)) int tlkmw_ota_end_ack_deal_SpecialOperationsr(uint8_t *pData, uint16_t dataLen, void *userArg)
{
    (void)pData;
    (void)dataLen;
    (void)userArg;

    return OTA_NONE;
}

/**
 * @brief      Initialize customer OTA start
 * @param[in]  none
 * @param[out] none
 * @return     int - OTA_NONE if success
 */
__attribute__((weak)) int tlkmw_ota_start_customer_init(void)
{
    return OTA_NONE;
}

/**
 * @brief      Deal with OTA general protocol
 * @param[in]  channel - communication channel
 * @param[in]  opcode  - operation code
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[in]  UserArg - user argument
 * @param[out] none
 * @return     none
 */
__attribute__((weak)) void tlk_ota_general_protocol_deal(uint8_t channel, uint8_t opcode, uint8_t *pData, uint16_t dataLen, void *UserArg)
{
    (void)channel;
    (void)opcode;
    (void)pData;
    (void)dataLen;
    (void)UserArg;
}

/**
 * @brief      Notify OTA status
 * @param[in]  pBuffer - pointer to buffer
 * @param[in]  buffLen - buffer length
 * @param[in]  UserArg - user argument
 * @param[out] none
 * @return     int - negative OTA_DEFINEDERR error code
 */
__attribute__((weak)) int tlk_ota_notify_status(uint8_t *pBuffer, uint32_t buffLen, void *UserArg)
{
    //The user needs to implement.
    (void)pBuffer;
    (void)buffLen;
    (void)UserArg;
    return -OTA_DEFINEDERR;
}

/**
 * @brief      Receive data via OTA general protocol
 * @param[in]  taskID  - task identifier
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[in]  UserArg - user argument
 * @param[out] none
 * @return     int - OTA_NONE if success, error code otherwise
 */
int tlk_ota_general_protocol_recv_data(uint32_t taskID, uint8_t *pData, uint16_t dataLen, void *UserArg)
{
    (void)taskID;
    if (pData == NULL || dataLen < 4) {
        return -OTA_PARAMERR;
    }

    uint16_t offset  = 0;
    uint8_t  channel = pData[0];
    offset += 1;

    //TODO: send OTA busy status to new channel.
    if (ota_general_ptotocol_ctrl.channel != 0 && ota_general_ptotocol_ctrl.channel != channel) {
        return -OTA_CHANNELERR;
    }

    ota_general_ptotocol_ctrl.channel = channel;

    offset += 2; //dataLen

    uint8_t  opcode = pData[3];
    uint16_t info   = 0;

    if (opcode > TLK_OTA_OPC_MAX) {
        return -OTA_PARAMERR;
    }
    offset += 1;

    OTA_ARRAY_TO_UINT16L(pData, offset, info);
    offset += 2;

    uint8_t  pack_flag  = info & 0x03;
    uint16_t pack_index = (info & 0xFFFC) >> 2;

    if (pack_flag > TLK_OTA_PACK_TYPE_END) {
        OTA_PRINTF("[OTA] pack flag error:%d", pack_flag);
        return -OTA_PARAMERR;
    }

    if (pack_flag == TLK_OTA_PACK_TYPE_COMPLETE) {
        tlk_ota_general_protocol_deal(ota_general_ptotocol_ctrl.channel, opcode, pData + offset, dataLen - offset, UserArg);
    } else {
        if ((dataLen + ota_general_ptotocol_ctrl.recv_cache_len) > TLKMW_OTA_TRANS_MAX_PACK_SIZE) {
            OTA_PRINTF("[OTA] cache buff null");
            return -OTA_PARAMERR;
        }

        if (ota_general_ptotocol_ctrl.recv_cache_opcode != TLK_OTA_OPC_NONE && opcode != ota_general_ptotocol_ctrl.recv_cache_opcode) {
            tlk_ota_general_protocol_clear_recv_cache();
            OTA_PRINTF("[OTA] opcode error:%d, cache:%d", opcode, ota_general_ptotocol_ctrl.recv_cache_opcode);
            return -OTA_PARAMERR;
        }

        if (pack_index != ota_general_ptotocol_ctrl.recv_cache_index) {
            tlk_ota_general_protocol_clear_recv_cache();
            OTA_PRINTF("[OTA] pack index error:%d, cache:%d", pack_index, ota_general_ptotocol_ctrl.recv_cache_index);
            return -OTA_PARAMERR;
        }

        if (pack_flag == TLK_OTA_PACK_TYPE_END) {
            OTA_MEMCPY(ota_general_ptotocol_ctrl.p_recv_cache_buff + ota_general_ptotocol_ctrl.recv_cache_len, pData + offset, dataLen - offset);
            ota_general_ptotocol_ctrl.recv_cache_len += (dataLen - offset);
            tlk_ota_general_protocol_deal(opcode, opcode, pData + offset, dataLen - offset, UserArg);
            tlk_ota_general_protocol_clear_recv_cache();
        } else {
            ota_general_ptotocol_ctrl.recv_cache_opcode = opcode;
            ota_general_ptotocol_ctrl.recv_cache_index += 1;
            ;
            OTA_MEMCPY(ota_general_ptotocol_ctrl.p_recv_cache_buff + ota_general_ptotocol_ctrl.recv_cache_len, pData + offset, dataLen - offset);

            ota_general_ptotocol_ctrl.recv_cache_len += (dataLen - offset);
        }
    }

    return OTA_NONE;
}

/**
 * @brief      Initialize OTA general protocol
 * @param[in]  pInterface - pointer to OTA interface
 * @param[out] none
 * @return     int - OTA_NONE if success, error code otherwise
 */
int tlk_ota_general_protocol_init(nvds_ota_Interface_t *pInterface)
{
    OTA_MEMSET(&ota_general_ptotocol_ctrl, 0, sizeof(tlk_ota_general_protocol_t));

    if (pInterface == NULL || pInterface->nvds_ota_malloc == NULL) {
        return -OTA_INITERR;
    }
    ota_general_ptotocol_ctrl.p_recv_cache_buff = (uint8_t *)pInterface->nvds_ota_malloc(TLKMW_OTA_TRANS_MAX_PACK_SIZE);
    if (ota_general_ptotocol_ctrl.p_recv_cache_buff == NULL) {
        return -OTA_INITERR;
    }
    if (tlk_ota_general_protocol_detail_init(pInterface) != OTA_NONE) {
        return -OTA_INITERR;
    }

    tlkmw_ota_register_chn_recv_interface(TLKMW_OTA_TRANS_CHN_UART, tlk_ota_general_protocol_recv_data);
    tlkmw_ota_register_chn_recv_interface(TLKMW_OTA_TRANS_CHN_BT_SPP, tlk_ota_general_protocol_recv_data);
    tlkmw_ota_register_chn_recv_interface(TLKMW_OTA_TRANS_CHN_BT_ATT, tlk_ota_general_protocol_recv_data);
    tlkmw_ota_register_chn_recv_interface(TLKMW_OTA_TRANS_CHN_BLE_GENERAL_MODE, tlk_ota_general_protocol_recv_data);

    return OTA_NONE;
}
