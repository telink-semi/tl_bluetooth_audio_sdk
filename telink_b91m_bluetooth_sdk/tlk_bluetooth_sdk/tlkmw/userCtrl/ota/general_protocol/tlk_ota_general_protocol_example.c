/********************************************************************************************************
 * @file    tlk_ota_general_protocol_example.c
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

static sTlkMwOta_t                 sTlkMwOtaCtrl;
sTlk_total_fw_descriptors_t        sTlkMwCurImgHeader;
static sTlk_total_fw_descriptors_t sTlkMwRecvImgHeader;

/**
 * @brief   Reset the OTA control module to initial state
 * @param   None
 * @return  None
 */
void tlk_ota_general_protocol_detail_reset(void)
{
    tlkmw_free_total_fw_descptors(&sTlkMwRecvImgHeader, sTlkMwOtaCtrl.ota_intf);
    if (sTlkMwOtaCtrl.timer != NULL && tlk_ota_timer_porting_destroy(sTlkMwOtaCtrl.timer)) {
        sTlkMwOtaCtrl.timer = NULL;
    }

    if (sTlkMwOtaCtrl.p_cache_buffer != NULL && sTlkMwOtaCtrl.ota_intf->nvds_ota_free != NULL) {
        sTlkMwOtaCtrl.ota_intf->nvds_ota_free(sTlkMwOtaCtrl.p_cache_buffer);
        sTlkMwOtaCtrl.p_cache_buffer = NULL;
    }

    OTA_MEMSET(&sTlkMwOtaCtrl, 0, sizeof(sTlkMwOtaCtrl) - sizeof(nvds_ota_Interface_t *));
    sTlkMwOtaCtrl.status = TLK_OTA_STATUS_IDLE;
}

/**
 * @brief       Align the given address upward to the specified size boundary
 * @param[in]   addr - The address to align
 * @param[in]   size - The alignment size (must be power of 2)
 * @return      The aligned address
 */
uint32_t tlkmw_ota_addr_align_up(uint32_t addr, uint32_t size)
{
    if ((addr & (size - 1)) == 0) {
        return addr;
    } else {
        return (addr + size - 1) & ~(size - 1);
    }
}

/**
 * @brief   Update the OTA status and notify the application
 * @param   pData - Pointer to the data containing status and reason
 * @param   dataLen - Length of the data in bytes
 * @param   userArg - User argument (not used)
 * @return  None
 */
void tlkmw_ota_update_ota_status(uint8_t *pData, uint16_t dataLen, void *userArg)
{
    (void)userArg;

    if (dataLen < 2) {
        return;
    }

    uint8_t status = pData[0];
    uint8_t reason = pData[1];

    /*Save OTA status to NVDS.*/
    sTlk_boot_and_ota_cfg_t boot_ota_cfg;
    if (sTlkMwOtaCtrl.ota_intf->nvds_ota_user_load == NULL) {
        return;
    }
    if (sTlkMwOtaCtrl.ota_intf->nvds_ota_user_load((uint8_t *)&boot_ota_cfg, sizeof(sTlk_boot_and_ota_cfg_t), NULL) != OTA_NONE) {
        return;
    }
    boot_ota_cfg.ota_status = status;
    sTlkMwOtaCtrl.ota_intf->nvds_ota_user_save((uint8_t *)&boot_ota_cfg, sizeof(sTlk_boot_and_ota_cfg_t), NULL);

    /*Notify OTA status to APP.*/
    sTlkMwNotifyEvent_t event = {
        .status  = status,
        .reason  = reason,
        .channel = sTlkMwOtaCtrl.channel,
    };
    tlk_ota_notify_status((uint8_t *)&event, sizeof(sTlkMwNotifyEvent_t), NULL);
}

/**
 * @brief   Send OTA transfer end indication and update OTA status
 * @param   result - The result of OTA operation
 * @param   userArg - User argument (not used)
 * @return  None
 */
void tlkmw_ota_send_trans_end(uint8_t result, void *userArg)
{
    (void)userArg;
    uint8_t  pBuffer[4];
    uint16_t buffLen = 0;
    uint8_t  status;

    /*Send OTA end to APP.*/
    pBuffer[buffLen++] = result & 0xFF;
    tlk_ota_general_protocol_send_data(TLK_OTA_OPC_OTA_TRANS_END, pBuffer, buffLen, NULL);
    OTA_PRINTF("[OTA] OTA end, result = %d", result);

    /*Convert result to firmware status*/
    if (result == TLK_OTA_RESULT_SUCCESS) {
        status = TLK_FIRMWARE_OTA_SUCCESS;
    } else {
        status = TLK_FIRMWARE_OTA_FAIL;
    }

    /*Update OTA status in NVDS and notify application*/
    buffLen            = 0;
    pBuffer[buffLen++] = status;
    pBuffer[buffLen++] = result;
    tlkmw_ota_update_ota_status(pBuffer, buffLen, NULL);
}

/**
 * @brief   Check if there is enough valid space for OTA update
 * @param   None
 * @return  true if there is enough space, false otherwise
 */
static bool tlkmw_ota_valid_size_is_enough(void)
{
    sTlk_boot_and_ota_cfg_t boot_ota_cfg;

    if (sTlkMwOtaCtrl.ota_intf->nvds_ota_user_load == NULL ||
        sTlkMwOtaCtrl.ota_intf->nvds_ota_user_load((uint8_t *)&boot_ota_cfg, sizeof(sTlk_boot_and_ota_cfg_t), NULL) != OTA_NONE) {
        return false;
    }
    uint32_t need_size = 0;
    if (sTlkMwRecvImgHeader.total_size > sTlkMwCurImgHeader.total_size) {
        need_size = sTlkMwRecvImgHeader.total_size - sTlkMwCurImgHeader.total_size + sTlkMwRecvImgHeader.total_size;
    } else {
        need_size = sTlkMwRecvImgHeader.total_size;
    }

    uint32_t                           total_flash_size = tlk_nvds_get_full_size();
    uint32_t                           user_info_size   = total_flash_size - boot_ota_cfg.user_area_addr;
    uint32_t                           cur_used_size    = TLKMW_TINYSQL_IMG_HEADER_ADDR + TLKMW_TINYSQL_IMG_HEADER_SIZE; //boot + ota info + total img size
    uint32_t                           cur_offset       = cur_used_size;
    struct sTlk_fw_descriptors_list_t *pNode            = sTlkMwCurImgHeader.fw_descpts_list;

    OTA_PRINTF("total_flash_size = %x, user_info_size = %x, cur_used_size = %x, need_size = %x", total_flash_size, user_info_size, cur_used_size, need_size);

    while (pNode != NULL) {
        uint32_t fw_start_addr = pNode->fw_img.start_addr + TLKMW_TINYSQL_IMG_HEADER_ADDR; //Test
        uint32_t fw_size       = pNode->fw_img.fw_size;
        if ((fw_start_addr - cur_offset) < need_size) {
            cur_used_size += (fw_start_addr - cur_offset + fw_size);
            cur_offset += (fw_start_addr - cur_offset + fw_size);
        } else {
            cur_used_size += fw_size;
            cur_offset             = (fw_start_addr + fw_size);
            sTlkMwOtaCtrl.backAddr = tlkmw_ota_addr_align_up(cur_offset, 0x1000);
        }
        pNode = pNode->pNext;
    }

    if (cur_offset < sTlkMwRecvImgHeader.total_size) {
        cur_offset += sTlkMwRecvImgHeader.total_size - cur_offset;
    }

    if (sTlkMwOtaCtrl.backAddr == 0) {
        sTlkMwOtaCtrl.backAddr = tlkmw_ota_addr_align_up(cur_offset, 0x1000);
    }

    if (sTlkMwRecvImgHeader.total_size > sTlkMwCurImgHeader.total_size) {
        sTlkMwOtaCtrl.backAddr = tlkmw_ota_addr_align_up(sTlkMwOtaCtrl.backAddr + sTlkMwRecvImgHeader.total_size - sTlkMwCurImgHeader.total_size, 0x1000);
    }

    uint32_t valid_size = total_flash_size - user_info_size - cur_used_size;
    if (valid_size > total_flash_size) {
        valid_size = 0;
    }

    OTA_PRINTF("total_flash_size = %x, user_info_size = %x, cur_used_size = %x, need_size = %x, backup_addr = %x", total_flash_size, user_info_size, cur_used_size, need_size,
               sTlkMwOtaCtrl.backAddr);
    if (valid_size < need_size) {
        return false;
    }

    return true;
}

/**
 * @brief      Save OTA breakpoint information to NVDS storage
 * @param      None
 * @return     None
 * @note       This function saves the current firmware index and offset to NVDS 
 *             to allow resuming OTA updates from the breakpoint in case of interruption
 */
static void tlk_ota_general_save_breakpoint()
{
    /*Save breakpoint info to NVDS.*/
    sTlk_boot_and_ota_cfg_t boot_ota_cfg;
    if (sTlkMwOtaCtrl.ota_intf->nvds_ota_user_load == NULL || sTlkMwOtaCtrl.ota_intf->nvds_ota_user_save == NULL) {
        return;
    }
    if (sTlkMwOtaCtrl.ota_intf->nvds_ota_user_load((uint8_t *)&boot_ota_cfg, sizeof(sTlk_boot_and_ota_cfg_t), NULL) != OTA_NONE) {
        return;
    }

    boot_ota_cfg.break_index  = sTlkMwOtaCtrl.curFwNum;
    boot_ota_cfg.break_offset = sTlkMwOtaCtrl.flash_save_size & (~(4096 - 1));
    boot_ota_cfg.backup_addr  = sTlkMwOtaCtrl.backAddr;
    sTlkMwOtaCtrl.ota_intf->nvds_ota_user_save((uint8_t *)&boot_ota_cfg, sizeof(sTlk_boot_and_ota_cfg_t), NULL);
}

/**
 * @brief   Timer callback function for OTA operations
 * @param   pTimer - Timer handle (not used)
 * @param   userArg - User argument (not used)
 * @return  None
 */
static void tlk_ota_general_protocol_example_timer_callback(tlk_ota_timer_handle_t pTimer, void *userArg)
{
    (void)pTimer;
    (void)userArg;

    // OTA_PRINTF("tlk_ota_general_protocol_example_timer_callback");

    if (sTlkMwOtaCtrl.timeout > 0) {
        sTlkMwOtaCtrl.timeout--;
    } else {
        OTA_PRINTF("[OTA] OTA data timeout.");
        tlk_ota_general_save_breakpoint();
        tlkmw_ota_send_trans_end(TLK_OTA_RESULT_TIMEOUT, NULL);
        tlk_ota_general_protocol_reset();
    }
}

static void tlkmw_ota_common_data_cache_deal(uint8_t *pData, uint16_t dataLen, bool isLast)
{
    if (sTlkMwOtaCtrl.p_cache_buffer == NULL || sTlkMwOtaCtrl.ota_intf->nvds_ota_write == NULL) {
        OTA_PRINTF("tlkmw_ota_common_data_cache_deal: cache buffer is NULL");
        return;
    }

    // Check if the buffer is cache full.
    if (sTlkMwOtaCtrl.cache_size + dataLen >= TLKMW_OTA_WRITE_CACHE_SIZE) {
        uint16_t spaceLeft = TLKMW_OTA_WRITE_CACHE_SIZE - sTlkMwOtaCtrl.cache_size;
        uint16_t writeSize = (dataLen > spaceLeft) ? spaceLeft : dataLen;

        OTA_MEMCPY(sTlkMwOtaCtrl.p_cache_buffer + sTlkMwOtaCtrl.cache_size, pData, writeSize);
        sTlkMwOtaCtrl.cache_size += writeSize;

        sTlkMwOtaCtrl.ota_intf->nvds_ota_eraseSector(sTlkMwOtaCtrl.backAddr + sTlkMwOtaCtrl.saveOffset);
        sTlkMwOtaCtrl.ota_intf->nvds_ota_write(sTlkMwOtaCtrl.backAddr + sTlkMwOtaCtrl.saveOffset, sTlkMwOtaCtrl.cache_size, sTlkMwOtaCtrl.p_cache_buffer);
        sTlkMwOtaCtrl.saveOffset += sTlkMwOtaCtrl.cache_size;
        sTlkMwOtaCtrl.flash_save_size += sTlkMwOtaCtrl.cache_size;
        sTlkMwOtaCtrl.cache_size = 0;

        // Cache the remaining receive data
        if (dataLen > writeSize) {
            OTA_MEMCPY(sTlkMwOtaCtrl.p_cache_buffer + sTlkMwOtaCtrl.cache_size, pData + writeSize, dataLen - writeSize);
            sTlkMwOtaCtrl.cache_size += (dataLen - writeSize);
        }
    } else {
        OTA_MEMCPY(sTlkMwOtaCtrl.p_cache_buffer + sTlkMwOtaCtrl.cache_size, pData, dataLen);
        sTlkMwOtaCtrl.cache_size += dataLen;
    }

    if (isLast) {
        if (sTlkMwOtaCtrl.cache_size > 0) {
            sTlkMwOtaCtrl.ota_intf->nvds_ota_eraseSector(sTlkMwOtaCtrl.backAddr + sTlkMwOtaCtrl.saveOffset);
            sTlkMwOtaCtrl.ota_intf->nvds_ota_write(sTlkMwOtaCtrl.backAddr + sTlkMwOtaCtrl.saveOffset, sTlkMwOtaCtrl.cache_size, sTlkMwOtaCtrl.p_cache_buffer);
            sTlkMwOtaCtrl.saveOffset += sTlkMwOtaCtrl.cache_size;
            sTlkMwOtaCtrl.flash_save_size += sTlkMwOtaCtrl.cache_size;
            sTlkMwOtaCtrl.cache_size = 0;
        }
    }
}

/**
 * @brief   Write the total image header to the OTA cache
 * @param   None
 * @return  true if successful, false otherwise
 */
static bool tlkmw_ota_write_total_img_header()
{
    uint8_t  pData[4096] = {0}; //max 4k
    uint16_t dataLen     = 0;

    OTA_MEMCPY(pData, &sTlkMwRecvImgHeader, 20);
    dataLen += 20;

    /*Save fw group list.*/
    for (uint8_t index = 0; index < sTlkMwRecvImgHeader.fw_group_number; index++) {
        struct sTlk_fw_group_list_t *pFwGroupListNode = tlkmw_get_fw_group_node_by_index(sTlkMwRecvImgHeader.fw_group_list, index);
        if (pFwGroupListNode == NULL) {
            return false;
        }
        /*bin count*/
        uint32_t bin_count = pFwGroupListNode->bin_count;
        OTA_UINT32L_TO_ARRAY(bin_count, pData, dataLen);
        dataLen += 4;
        /*Save fw info list.*/
        for (uint8_t bin_index = 0; bin_index < bin_count; bin_index++) {
            struct sTlk_fw_info_list_t *pFwInfoListNode = tlkmw_get_fw_info_node_by_index(pFwGroupListNode->fw_info_list, bin_index);
            if (pFwInfoListNode == NULL) {
                return false;
            }
            OTA_UINT32L_TO_ARRAY(pFwInfoListNode->bin_type, pData, dataLen);
            dataLen += 4;
            OTA_UINT32L_TO_ARRAY(pFwInfoListNode->bin_version, pData, dataLen);
            dataLen += 4;
        }
        /*resv*/
        OTA_UINT32L_TO_ARRAY(0, pData, dataLen);
        dataLen += 4;
    }

    for (uint8_t index = 0; index < sTlkMwRecvImgHeader.fw_number; index++) {
        sTlk_fw_descriptors_node_t *fw_info = tlkmw_get_fw_descpts_node_by_index(sTlkMwRecvImgHeader.fw_descpts_list, index);
        if (fw_info == NULL) {
            return false;
        }
        OTA_MEMCPY(pData + dataLen, (uint8_t *)fw_info, sizeof(sTlk_fw_descriptors_node_t));
        dataLen += sizeof(sTlk_fw_descriptors_node_t);
    }

    /*resv*/
    dataLen += 16;

    /*Recalculate total image crc.*/
    uint32_t crc                      = 0xffffffff;
    crc                               = calculate_CRC32(crc, pData, dataLen);
    sTlkMwRecvImgHeader.img_crc.crc32 = crc;

    OTA_MEMCPY(pData + dataLen, &sTlkMwRecvImgHeader.img_crc, sizeof(sTlk_cur_fw_entity_crc_t));
    dataLen += sizeof(sTlk_cur_fw_entity_crc_t);

    tlkmw_ota_common_data_cache_deal(pData, dataLen, true);
    return true;
}

/**
 * @brief   Handle OTA version check request
 * @param   pData - Pointer to received data (not used)
 * @param   dataLen - Length of received data (not used)
 * @param   userArg - User argument (not used)
 * @return  OTA_NONE if successful, error code otherwise
 */
int tlkmw_ota_version_check(uint8_t *pData, uint16_t dataLen, void *userArg)
{
    (void)pData;
    (void)dataLen;
    (void)userArg;

    uint8_t  pBuffer[4];
    uint16_t buffLen = 0;

    uint32_t version   = 0xFFFFFFFF; //Now do not check version, just return 0xFFFFFFFF.
    pBuffer[buffLen++] = version & 0xff;
    pBuffer[buffLen++] = (version >> 8) & 0xff;
    pBuffer[buffLen++] = (version >> 16) & 0xff;
    pBuffer[buffLen++] = (version >> 24) & 0xff;
    tlk_ota_general_protocol_send_data(TLK_OTA_OPC_VERSION_RSP, pBuffer, buffLen, NULL);

    return OTA_NONE;
}

/**
 * @brief   Handle OTA parameter negotiation request
 * @param   pData - Pointer to received data containing requested packet size
 * @param   dataLen - Length of received data
 * @param   userArg - User argument (not used)
 * @return  OTA_NONE if successful, error code otherwise
 */
int tlkmw_ota_param_negotiation(uint8_t *pData, uint16_t dataLen, void *userArg)
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

    if (sTlkMwOtaCtrl.channel == TLKMW_OTA_TRANS_CHN_UART) {
        sTlkMwOtaCtrl.shakeIntv = TLKMW_OTA_SHAKE_UART_INTV;
    } else {
        sTlkMwOtaCtrl.shakeIntv = TLKMW_OTA_SHAKE_WIRELESS_INTV;
    }

    pBuffer[buffLen++] = sTlkMwOtaCtrl.shakeIntv & 0xff;
    pBuffer[buffLen++] = (sTlkMwOtaCtrl.shakeIntv >> 8) & 0xff;

    tlk_ota_general_protocol_send_data(TLK_OTA_OPC_PARAM_NEGO_RSP, pBuffer, buffLen, NULL);

    return OTA_NONE;
}

/**
 * @brief   Handle OTA breakpoint request
 * @param   pData - Pointer to received data (not used)
 * @param   dataLen - Length of received data (not used)
 * @param   userArg - User argument (not used)
 * @return  OTA_NONE if successful, error code otherwise
 */
int tlkmw_ota_break_point_request(uint8_t *pData, uint16_t dataLen, void *userArg)
{
    (void)pData;
    (void)dataLen;
    (void)userArg;

    uint8_t  break_index  = 0xFF;       //0xFF means no break point
    uint32_t break_offset = 0xFFFFFFFF; //0xFFFFFFFF means no break point.

    if (sTlkMwOtaCtrl.fwDataRecvSize > 0) {
        break_index  = sTlkMwOtaCtrl.curFwNum;
        break_offset = sTlkMwOtaCtrl.fwDataRecvSize;
    }

    uint8_t  pBuffer[8];
    uint16_t buffLen = 0;

    pBuffer[buffLen++] = (break_index & 0xFF);
    OTA_UINT32L_TO_ARRAY(break_offset, pBuffer, buffLen);
    buffLen += 4;

    tlk_ota_general_protocol_send_data(TLK_OTA_OPC_BREAK_INFO_RSP, pBuffer, buffLen, NULL);

    return OTA_NONE;
}

/**
 * @brief   Validate and process total firmware descriptors
 * @param   pData - Pointer to received firmware descriptor data
 * @param   dataLen - Length of received data
 * @param   userArg - User argument (not used)
 * @return  OTA_NONE if successful, error code otherwise
 */
int tlkmw_ota_total_fw_descriptors_check(uint8_t *pData, uint16_t dataLen, void *userArg)
{
    (void)userArg;
    if (tlk_ota_parse_total_fw_descriptors(pData, dataLen, &sTlkMwRecvImgHeader, sTlkMwOtaCtrl.ota_intf) != OTA_NONE) {
        return -OTA_PARAMERR;
    }

    if (sTlkMwOtaCtrl.fwDataRecvSize > 0) {
        /*Have break point, verify new total header and last total header.*/
        uint32_t addr     = sTlkMwOtaCtrl.backAddr + FLASH_R_BASE_ADDR;
        uint32_t img_size = *(uint32_t *)(addr + 4);
        uint32_t crc      = 0xffffffff;

        crc = calculate_CRC32(crc, (uint8_t *)addr, img_size - sizeof(sTlk_cur_fw_entity_crc_t));
        if (crc != sTlkMwRecvImgHeader.img_crc.crc32) {
            OTA_PRINTF("[OTA] OTA total header verify failed with last total header, erase break point.");
            /*Erase last total header.*/
            sTlk_boot_and_ota_cfg_t boot_ota_cfg;
            sTlkMwOtaCtrl.ota_intf->nvds_ota_user_load((uint8_t *)&boot_ota_cfg, sizeof(sTlk_boot_and_ota_cfg_t), NULL);

            boot_ota_cfg.break_index  = 0xFF;
            boot_ota_cfg.break_offset = 0xFFFFFFFF;

            sTlkMwOtaCtrl.curFwNum       = 0;
            sTlkMwOtaCtrl.fwDataRecvSize = 0;
            sTlkMwOtaCtrl.backAddr       = 0;

            sTlkMwOtaCtrl.ota_intf->nvds_ota_user_save((uint8_t *)&boot_ota_cfg, sizeof(sTlk_boot_and_ota_cfg_t), NULL);
        } else {
            uint8_t ret = TLK_OTA_RESULT_SUCCESS;
            tlk_ota_general_protocol_send_data(TLK_OTA_OPC_TOTAL_FW_DESCRRIPTOR_RSP, &ret, 1, NULL);
            return OTA_NONE;
        }
    }

    if (tlkmw_ota_valid_size_is_enough() == false) {
        return -OTA_PARAMERR;
    }
    //Some parameter might updates, so we need to reparse and then write total fw descripotrs;
    if (tlkmw_ota_write_total_img_header() == false) {
        return -OTA_WRITEERR;
    }
    uint8_t ret = TLK_OTA_RESULT_SUCCESS;
    tlk_ota_general_protocol_send_data(TLK_OTA_OPC_TOTAL_FW_DESCRRIPTOR_RSP, &ret, 1, NULL);

    return OTA_NONE;
}

/**
 * @brief   Send firmware image descriptor request indication
 * @param   None
 * @return  None
 */
static void tlkmw_ota_send_fw_img_ind()
{
    uint8_t  pBuffer[4];
    uint16_t buffLen = 0;
    uint32_t offset  = 0;

    if (sTlkMwRecvImgHeader.fw_descpts_list == NULL) {
        tlkmw_ota_send_trans_end(TLK_OTA_RESULT_CUR_FW_DESCRRIPTORS_ERR, NULL);
        return;
    }

    sTlk_fw_descriptors_node_t *pFwInfo = tlkmw_get_fw_descpts_node_by_index(sTlkMwRecvImgHeader.fw_descpts_list, sTlkMwOtaCtrl.curFwNum);
    if (pFwInfo == NULL) {
        tlkmw_ota_send_trans_end(TLK_OTA_RESULT_CUR_FW_DESCRRIPTORS_ERR, NULL);
        return;
    }

    offset = pFwInfo->start_addr;

    OTA_UINT32L_TO_ARRAY(offset, pBuffer, buffLen);
    buffLen += 4;
    tlk_ota_general_protocol_send_data(TLK_OTA_OPC_CUR_FW_DESCRRIPTOR_REQ, pBuffer, buffLen, NULL);
}

/**
 * @brief   Handle start OTA request
 * @param   pData - Pointer to received data (not used)
 * @param   dataLen - Length of received data (not used)
 * @param   userArg - User argument (not used)
 * @return  OTA_NONE if successful, error code otherwise
 */
int tlkmw_ota_start_ota_deal(uint8_t *pData, uint16_t dataLen, void *userArg)
{
    (void)pData;
    (void)dataLen;
    (void)userArg;

    uint8_t ret = TLK_OTA_RESULT_SUCCESS;

    tlk_ota_general_protocol_send_data(TLK_OTA_OPC_START_OTA_RSP, &ret, 1, NULL);

    tlkmw_ota_send_fw_img_ind();

    return OTA_NONE;
}

/**
 * @brief   Parse and validate current firmware descriptor
 * @param   pData - Pointer to firmware descriptor data
 * @param   dataLen - Length of descriptor data
 * @return  TLK_OTA_RESULT_SUCCESS if successful, error code otherwise
 */
static uint8_t tlkmw_ota_cur_fw_descriptor_parse(uint8_t *pData, uint16_t dataLen)
{
    if (pData == NULL || dataLen < sizeof(sTlk_cur_fw_descriptor_header_t)) {
        return TLK_OTA_RESULT_CUR_FW_DESCRRIPTORS_ERR;
    }

    /*check crc.*/
    sTlk_cur_fw_entity_crc_t recv_crc;

    uint32_t cal_crc = 0xFFFFFFFF;
    cal_crc          = calculate_CRC32(cal_crc, pData, dataLen - sizeof(sTlk_cur_fw_entity_crc_t));
    OTA_MEMCPY(&recv_crc.crc32, pData + sizeof(sTlk_cur_fw_descriptor_header_t) - sizeof(sTlk_cur_fw_entity_crc_t), sizeof(sTlk_cur_fw_entity_crc_t));

    if (recv_crc.crc32 != cal_crc) {
        OTA_PRINTF("OTA img header crc error img_crc-[0x%x]", cal_crc);
        return TLK_OTA_RESULT_CUR_FW_DESCRRIPTORS_ERR;
    }

    /*Check fw img info with current fw info.*/
    sTlk_fw_descriptors_node_t *pFwInfo = tlkmw_get_fw_descpts_node_by_index(sTlkMwCurImgHeader.fw_descpts_list, sTlkMwOtaCtrl.curFwNum);

    if (pFwInfo != NULL) {
        uint32_t cur_fw_img_addr = pFwInfo->start_addr + TLKMW_TINYSQL_IMG_HEADER_ADDR + FLASH_R_BASE_ADDR;
        uint32_t cur_fw_pid      = *(uint32_t *)cur_fw_img_addr;
        uint32_t cur_fw_vid      = *(uint32_t *)(cur_fw_img_addr + 4);

        uint32_t recv_fw_pid, recv_fw_vid;

        OTA_ARRAY_TO_UINT32L(pData, 0, recv_fw_pid);
        OTA_ARRAY_TO_UINT32L(pData, 4, recv_fw_vid);

        if (cur_fw_pid != recv_fw_pid || cur_fw_vid != recv_fw_vid) {
            return TLK_OTA_RESULT_FIRMWARE_MARK_ERR;
        }
    }

    /*Do not have break point*/
    if (sTlkMwOtaCtrl.fwDataRecvSize == 0) {
        tlkmw_ota_common_data_cache_deal(pData, dataLen, false); // Cache fw img header to buffer.
    }
    return OTA_NONE;
}

/**
 * @brief   Handle current firmware descriptor check request
 * @param   pData - Pointer to received firmware descriptor data
 * @param   dataLen - Length of received data
 * @param   userArg - User argument (not used)
 * @return  OTA_NONE if successful, error code otherwise
 */
int tlkmw_ota_cur_fw_descriptor_check(uint8_t *pData, uint16_t dataLen, void *userArg)
{
    (void)userArg;
    uint8_t  pBuffer[4];
    uint16_t buffLen = 0;

    int ret = tlkmw_ota_cur_fw_descriptor_parse(pData, dataLen);

    sTlk_fw_descriptors_node_t *pFwInfo = tlkmw_get_fw_descpts_node_by_index(sTlkMwRecvImgHeader.fw_descpts_list, sTlkMwOtaCtrl.curFwNum);
    if (ret != TLK_OTA_RESULT_SUCCESS || pFwInfo == NULL) {
        tlkmw_ota_send_trans_end(TLK_OTA_RESULT_CUR_FW_DESCRRIPTORS_ERR, NULL);
        return -OTA_PARSEERR;
    }

    /*Verify success, start request fw data.*/
    sTlkMwOtaCtrl.fwDataRecvNumb = 0;

    if (sTlkMwOtaCtrl.fwDataRecvSize > 0) { //have break point
        sTlkMwOtaCtrl.fwDataTotalSize   = pFwInfo->fw_size;
        sTlkMwOtaCtrl.fwDataStartOffset = pFwInfo->start_addr + sTlkMwOtaCtrl.fwDataRecvSize;
        sTlkMwOtaCtrl.saveOffset        = pFwInfo->start_addr + sTlkMwOtaCtrl.fwDataRecvSize;
    } else {
        sTlkMwOtaCtrl.fwDataRecvSize    = sizeof(sTlk_cur_fw_descriptor_header_t);
        sTlkMwOtaCtrl.fwDataTotalSize   = pFwInfo->fw_size;
        sTlkMwOtaCtrl.fwDataStartOffset = pFwInfo->start_addr + sizeof(sTlk_cur_fw_descriptor_header_t);
        sTlkMwOtaCtrl.saveOffset        = pFwInfo->start_addr;
    }

    OTA_PRINTF("tlkmw_ota_cur_fw_descriptor_check index[%x], recv[%x], offset[%x], start[%x], size[%d]", sTlkMwOtaCtrl.curFwNum, sTlkMwOtaCtrl.fwDataRecvSize,
               sTlkMwOtaCtrl.fwDataStartOffset, pFwInfo->start_addr, sizeof(sTlk_cur_fw_descriptor_header_t));
    ;

    OTA_UINT32L_TO_ARRAY(sTlkMwOtaCtrl.fwDataStartOffset, pBuffer, buffLen);
    buffLen += 4;
    tlk_ota_general_protocol_send_data(TLK_OTA_OPC_CUR_FW_DATA_REQ, pBuffer, buffLen, NULL);

    return OTA_NONE;
}

/**
 * @brief   Process firmware data transfer completion
 * @param   None
 * @return  OTA_NONE if successful, error code otherwise
 */
static int tlkmw_ota_fw_data_ind_apply_deal()
{
    if (sTlkMwOtaCtrl.curFwNum < (sTlkMwRecvImgHeader.fw_number - 1)) {
        /*Last fw trans end, start trans next fw.*/
        sTlkMwOtaCtrl.curFwNum += 1;

        sTlkMwOtaCtrl.fwDataStartOffset = 0; //Used for request data form APP, offset of total bin;
        sTlkMwOtaCtrl.fwDataTotalSize   = 0; //current fw total size;
        sTlkMwOtaCtrl.fwDataRecvSize    = 0; //already received size;
        sTlkMwOtaCtrl.fwDataRecvNumb    = 0; //current fw received number;
        sTlkMwOtaCtrl.fwDataPendNumb    = 0; //current fw pending number;
        sTlkMwOtaCtrl.flash_save_size   = 0; //current fw save size;

        tlkmw_ota_send_fw_img_ind();
    } else {
        OTA_PRINTF("OTA SUCCESS !!!");

        tlkmw_ota_send_trans_end(TLK_OTA_RESULT_SUCCESS, NULL);
    }
    return OTA_NONE;
}

/**
 * @brief   Process received OTA data packets
 * @param   pData - Pointer to received data packet
 * @param   dataLen - Length of received data
 * @return  None
 */
static void tlkmw_ota_recv_trans_data(uint8_t *pData, uint16_t dataLen)
{
    if (pData == NULL || dataLen < 2) {
        tlkmw_ota_send_trans_end(TLK_OTA_RESULT_PDU_LEN_ERR, NULL);
        return;
    }

    uint32_t recvNumb = 0;
    OTA_ARRAY_TO_UINT32L(pData, 0, recvNumb);
    pData += 4;
    dataLen -= 4;

    if (sTlkMwOtaCtrl.fwDataPendNumb != 0 && recvNumb != sTlkMwOtaCtrl.fwDataPendNumb) {
        return;
    } else {
        sTlkMwOtaCtrl.fwDataPendNumb = 0;
    }

    if (recvNumb != sTlkMwOtaCtrl.fwDataRecvNumb) {
        uint8_t  pBuffer[8];
        uint16_t buffLen = 0;
        OTA_UINT32L_TO_ARRAY(sTlkMwOtaCtrl.fwDataStartOffset, pBuffer, buffLen);
        buffLen += 4;
        OTA_UINT32L_TO_ARRAY(sTlkMwOtaCtrl.fwDataRecvNumb, pBuffer, buffLen);
        buffLen += 4;
        tlk_ota_general_protocol_send_data(TLK_OTA_OPC_SYNC_LOSS, pBuffer, buffLen, NULL);
        sTlkMwOtaCtrl.fwDataPendNumb = sTlkMwOtaCtrl.fwDataRecvNumb;
        return;
    }

    bool isLast = false;

    sTlkMwOtaCtrl.fwDataStartOffset += dataLen;
    sTlkMwOtaCtrl.fwDataRecvSize += dataLen;
    sTlkMwOtaCtrl.fwDataRecvNumb += 1;

    if (sTlkMwOtaCtrl.fwDataRecvSize >= sTlkMwOtaCtrl.fwDataTotalSize) {
        isLast = true;
    }

    tlkmw_ota_common_data_cache_deal(pData, dataLen, isLast);

    if (isLast == true) {
        OTA_PRINTF("OTA fw[%d] receive complete,recvLen[%d], totalLen[%d]", sTlkMwOtaCtrl.curFwNum, sTlkMwOtaCtrl.fwDataRecvSize, sTlkMwOtaCtrl.fwDataTotalSize);

        // check fw data crc
        bool                        crc_check = false;
        sTlk_fw_descriptors_node_t *pFwInfo   = tlkmw_get_fw_descpts_node_by_index(sTlkMwRecvImgHeader.fw_descpts_list, sTlkMwOtaCtrl.curFwNum);
        if (pFwInfo != NULL) {
            uint32_t                 cal_crc = 0xFFFFFFFF;
            sTlk_cur_fw_entity_crc_t read_crc;
            uint32_t                 addr     = sTlkMwOtaCtrl.backAddr + pFwInfo->start_addr + FLASH_R_BASE_ADDR;
            uint32_t                 crc_addr = sTlkMwOtaCtrl.backAddr + pFwInfo->start_addr + pFwInfo->fw_size - sizeof(sTlk_cur_fw_entity_crc_t) + FLASH_R_BASE_ADDR;
            cal_crc                           = calculate_CRC32(cal_crc, (uint8_t *)addr, pFwInfo->fw_size - sizeof(sTlk_cur_fw_entity_crc_t));

            OTA_MEMCPY(&read_crc.crc32, (uint8_t *)crc_addr, sizeof(sTlk_cur_fw_entity_crc_t));
            crc_check = (cal_crc == read_crc.crc32) ? true : false;
        }

        if (pFwInfo == NULL || crc_check == false) {
            tlkmw_ota_send_trans_end(TLK_OTA_RESULT_DATA_CRC_ERR, NULL);
            return;
        }

        tlkmw_ota_fw_data_ind_apply_deal();
        return;
    }

    if (sTlkMwOtaCtrl.shakeIntv != 0xFFFF && (sTlkMwOtaCtrl.fwDataRecvNumb % sTlkMwOtaCtrl.shakeIntv == 0)) {
        uint8_t  pBuffer[4];
        uint16_t buffLen = 0;
        OTA_UINT32L_TO_ARRAY(sTlkMwOtaCtrl.fwDataStartOffset, pBuffer, buffLen);
        buffLen += 4;

        tlk_ota_general_protocol_send_data(TLK_OTA_OPC_CUR_FW_DATA_REQ, pBuffer, buffLen, NULL);
    }
}

/**
 * @brief   Handle current firmware data packet
 * @param   pData - Pointer to received firmware data
 * @param   dataLen - Length of received data
 * @param   userArg - User argument (not used)
 * @return  OTA_NONE if successful, error code otherwise
 */
int tlkmw_ota_cur_data_deal(uint8_t *pData, uint16_t dataLen, void *userArg)
{
    (void)userArg;
    tlkmw_ota_recv_trans_data(pData, dataLen);

    return OTA_NONE;
}

/**
 * @brief   Handle bootloader update process
 * @param   None
 * @return  true if bootloader update was attempted, false otherwise
 */
static bool tlkmw_ota_update_bootloader_deal(void)
{
    /*Currently, simultaneous OTA of the bootloader and other firmware is not supported.*/
    if (sTlkMwOtaCtrl.ota_intf->nvds_ota_read == NULL || sTlkMwOtaCtrl.ota_intf->nvds_ota_write == NULL || sTlkMwOtaCtrl.ota_intf->nvds_ota_eraseSector == NULL) {
        return false;
    }

    if (sTlkMwRecvImgHeader.fw_number != 1) {
        return false;
    }

    sTlk_fw_descriptors_node_t *pFwInfo = tlkmw_get_fw_descpts_node_by_index(sTlkMwRecvImgHeader.fw_descpts_list, 0);
    if (pFwInfo == NULL || (pFwInfo->fw_type & 0xFF) != 0x04) { //BINX_BOOTLOADER) {
        return false;
    }

    uint32_t r = core_interrupt_disable();

    /*Double check bootloader crc.*/
    uint32_t                 cal_crc = 0xFFFFFFFF;
    sTlk_cur_fw_entity_crc_t read_crc;
    uint32_t                 addr = sTlkMwOtaCtrl.backAddr + pFwInfo->start_addr + FLASH_R_BASE_ADDR;

    uint32_t crc_addr = sTlkMwOtaCtrl.backAddr + pFwInfo->start_addr + pFwInfo->fw_size - sizeof(sTlk_cur_fw_entity_crc_t) + FLASH_R_BASE_ADDR;
    cal_crc           = calculate_CRC32(cal_crc, (uint8_t *)addr, pFwInfo->fw_size - sizeof(sTlk_cur_fw_entity_crc_t));

    OTA_MEMCPY(&read_crc.crc32, (uint8_t *)crc_addr, sizeof(sTlk_cur_fw_entity_crc_t));
    if (cal_crc != read_crc.crc32) {
        OTA_PRINTF("tlkmw_ota_update_bootloader_deal crc check error.");
        core_restore_interrupt(r);
        return true;
    }

    /*Update bootloader.*/
    OTA_PRINTF("!!! Update bootloader,please ensure the stability of the system power supply !!!");
    /*TODO: unlock bootloader area.*/
    uint32_t update_size   = pFwInfo->fw_size - sizeof(sTlk_cur_fw_descriptor_header_t) - sizeof(sTlk_cur_fw_entity_crc_t);
    uint32_t update_offset = sTlkMwOtaCtrl.backAddr + pFwInfo->start_addr + sizeof(sTlk_cur_fw_descriptor_header_t);

    cal_crc          = 0xFFFFFFFF;
    uint32_t new_crc = 0xFFFFFFFF;

    addr = sTlkMwOtaCtrl.backAddr + pFwInfo->start_addr + sizeof(sTlk_cur_fw_descriptor_header_t) + FLASH_R_BASE_ADDR;
    /*cal crc of new bootloader before update, only cal fw data.*/
    cal_crc = calculate_CRC32(cal_crc, (uint8_t *)addr, update_size);

    uint8_t buffer[4096];
    for (uint32_t offset = 0; offset < update_size; offset += 0x1000) {
        if (!(offset % 0x1000)) {
            sTlkMwOtaCtrl.ota_intf->nvds_ota_eraseSector(offset);
        }

        uint32_t remaining = update_size - offset;
        uint32_t data_size = (remaining > 0x1000) ? 0x1000 : remaining;

        sTlkMwOtaCtrl.ota_intf->nvds_ota_read(update_offset + offset, data_size, buffer);
        sTlkMwOtaCtrl.ota_intf->nvds_ota_write(offset, data_size, buffer);
    }

    addr = FLASH_R_BASE_ADDR;
    /*cal crc of new bootloader before update, only cal fw data.*/
    new_crc = calculate_CRC32(new_crc, (uint8_t *)addr, update_size);
    if (new_crc == cal_crc) {
        sys_reboot();
    } else {
        OTA_PRINTF("crc check fail after update bootloader, please do not reboot. cal_crc %x, new_crc %x", cal_crc, new_crc);
    }
    core_restore_interrupt(r);

    return true;
}

/**
 * @brief      Handle OTA end deal
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[in]  userArg - user argument
 * @param[out] none
 * @return     int - OTA_NONE if success
 */
int tlkmw_ota_end_deal(uint8_t *pData, uint16_t dataLen, void *userArg)
{
    (void)pData;
    (void)dataLen;
    (void)userArg;
    uint8_t reason = pData[0];

    if (reason != TLK_OTA_RESULT_SUCCESS) {
        tlk_ota_general_save_breakpoint();
    }

    tlk_ota_general_protocol_reset();

    return OTA_NONE;
}

/**
 * @brief   Handle OTA end acknowledge
 * @param   pData - Pointer to received data (not used)
 * @param   dataLen - Length of received data (not used)
 * @param   userArg - User argument (not used)
 * @return  OTA_NONE if successful, error code otherwise
 */
int tlkmw_ota_end_ack_deal(uint8_t *pData, uint16_t dataLen, void *userArg)
{
    (void)pData;
    (void)dataLen;
    (void)userArg;

    if (sTlkMwOtaCtrl.timer != NULL) {
        if (tlk_ota_timer_porting_destroy(sTlkMwOtaCtrl.timer)) {
            sTlkMwOtaCtrl.timer = NULL;
        }
    }

    /*Check whether the upgraded fw is the bootloader. */
    if (tlkmw_ota_update_bootloader_deal() == true) {
        return -OTA_STATUSERR;
    }

    sTlk_boot_and_ota_cfg_t boot_ota_cfg;

    if (sTlkMwOtaCtrl.ota_intf->nvds_ota_user_load == NULL || sTlkMwOtaCtrl.ota_intf->nvds_ota_user_save == NULL) {
        return -OTA_STATUSERR;
    }

    if (sTlkMwOtaCtrl.ota_intf->nvds_ota_user_load((uint8_t *)&boot_ota_cfg, sizeof(sTlk_boot_and_ota_cfg_t), NULL) != OTA_NONE) {
        return -OTA_STATUSERR;
    }

    boot_ota_cfg.ota_status  = TLK_FIRMWARE_OTA_SUCCESS; //OTA Success end and verify success.
    boot_ota_cfg.backup_addr = sTlkMwOtaCtrl.backAddr;   //OTA area, backup addr.

    /*Clean break point.*/
    boot_ota_cfg.break_index  = 0xFF;
    boot_ota_cfg.break_offset = 0xFFFFFFFF;

    if (sTlkMwOtaCtrl.ota_intf->nvds_ota_user_save((uint8_t *)&boot_ota_cfg, sizeof(sTlk_boot_and_ota_cfg_t), NULL) == OTA_NONE) {
        int ret = tlkmw_ota_end_ack_deal_SpecialOperationsr(NULL, 0, NULL);
        if (ret == OTA_NONE) {
            sys_reboot();
        }
    }

    return OTA_NONE;
}

/**
 * @brief   Load current image header from flash
 * @param   None
 * @return  OTA_NONE if successful, error code otherwise
 */
int tlkmw_ota_load_cur_img_header(void)
{
    if (sTlkMwOtaCtrl.ota_intf->nvds_ota_read == NULL) {
        return -OTA_INITERR;
    }

    uint32_t img_header_addr = TLKMW_TINYSQL_IMG_HEADER_ADDR + FLASH_R_BASE_ADDR;

    sTlkMwCurImgHeader.img_version     = *(uint32_t *)img_header_addr;
    sTlkMwCurImgHeader.img_valid_size  = *(uint32_t *)(img_header_addr + 4);
    sTlkMwCurImgHeader.fw_number       = *(uint32_t *)(img_header_addr + 8);
    sTlkMwCurImgHeader.total_size      = *(uint32_t *)(img_header_addr + 12);
    sTlkMwCurImgHeader.fw_group_number = *(uint32_t *)(img_header_addr + 16);

    uint8_t  pData[4096];
    uint16_t dataLen = sTlkMwCurImgHeader.img_valid_size;
    sTlkMwOtaCtrl.ota_intf->nvds_ota_read(TLKMW_TINYSQL_IMG_HEADER_ADDR, dataLen, pData);

    if (tlk_ota_parse_total_fw_descriptors(pData, dataLen, &sTlkMwCurImgHeader, sTlkMwOtaCtrl.ota_intf) == OTA_NONE) {
        OTA_PRINTF("img_version %x, img_size %x, fw_number %x, total_size %x, fw_group_number %x", sTlkMwCurImgHeader.img_version, sTlkMwCurImgHeader.img_valid_size,
                   sTlkMwCurImgHeader.fw_number, sTlkMwCurImgHeader.total_size, sTlkMwCurImgHeader.fw_group_number);

        if (sTlkMwCurImgHeader.img_valid_size > TLKMW_TINYSQL_IMG_HEADER_SIZE) {
            sTlkMwCurImgHeader.img_valid_size = TLKMW_TINYSQL_IMG_HEADER_SIZE;
        }
        if (sTlkMwCurImgHeader.fw_number > TLKMW_OTA_FW_NUMB_MAX) {
            sTlkMwCurImgHeader.fw_number = TLKMW_OTA_FW_NUMB_MAX;
        }
        return OTA_NONE;
    }
    return -OTA_INITERR;
}

/**
 * @brief   Main OTA protocol handler function
 * @param   channel - Communication channel identifier
 * @param   opcode - OTA operation code
 * @param   pData - Pointer to received data
 * @param   dataLen - Length of received data
 * @param   UserArg - User argument
 * @return  None
 */
void tlk_ota_general_protocol_deal(uint8_t channel, uint8_t opcode, uint8_t *pData, uint16_t dataLen, void *UserArg)
{
    if (sTlkMwOtaCtrl.channel != 0 && sTlkMwOtaCtrl.channel != channel) {
        return;
    }

    sTlkMwOtaCtrl.channel = channel;

    uint8_t status = TLK_OTA_RESULT_SUCCESS;
    switch (opcode) {
    case TLK_OTA_OPC_VERSION_REQ:
    {
        if (sTlkMwOtaCtrl.status != TLK_OTA_STATUS_IDLE) {
            status = TLK_OTA_RESULT_OTA_INCOMPLETE;
            break;
        }
        if (tlkmw_ota_version_check(pData, dataLen, UserArg) == OTA_NONE) {
            sTlkMwOtaCtrl.status = TLK_OTA_STATUS_BUSY;
            /* Create timer */
            if (sTlkMwOtaCtrl.timer != NULL && tlk_ota_timer_porting_destroy(sTlkMwOtaCtrl.timer)) {
                sTlkMwOtaCtrl.timer = NULL;
            }
            if (sTlkMwOtaCtrl.timer == NULL &&
                tlk_ota_timer_porting_create(&sTlkMwOtaCtrl.timer, TLKMW_OTA_TRANS_TIMEOUT_INTV, tlk_ota_general_protocol_example_timer_callback, NULL)) {
                tlk_ota_timer_porting_start(sTlkMwOtaCtrl.timer);
                sTlkMwOtaCtrl.timeout = TLKMW_OTA_TRANS_TIMEOUT;
            }
            /*Malloc buffer to cache data to write to flash*/
            sTlkMwOtaCtrl.p_cache_buffer = (uint8_t *)sTlkMwOtaCtrl.ota_intf->nvds_ota_malloc(TLKMW_OTA_WRITE_CACHE_SIZE);
            if (sTlkMwOtaCtrl.p_cache_buffer == NULL) {
                status = TLK_OTA_RESULT_VERSION_COMPARE_ERR;
            }

            /*Load breadpoint info from NVDS*/
            // sTlk_boot_and_ota_cfg_t boot_ota_cfg;
            // if (sTlkMwOtaCtrl.ota_intf->nvds_ota_user_load != NULL &&
            // 	sTlkMwOtaCtrl.ota_intf->nvds_ota_user_load((uint8_t*)&boot_ota_cfg, sizeof(sTlk_boot_and_ota_cfg_t), NULL) == OTA_NONE) {
            // 	if (boot_ota_cfg.break_index != 0xFF && boot_ota_cfg.break_offset != 0xFFFFFFFF) {
            // 		sTlkMwOtaCtrl.curFwNum = boot_ota_cfg.break_index;
            // 		sTlkMwOtaCtrl.fwDataRecvSize = boot_ota_cfg.break_offset;//last transfer already saved size.
            // 		sTlkMwOtaCtrl.backAddr = boot_ota_cfg.backup_addr;
            // 	}
            // } else {
            // 	status = TLK_OTA_RESULT_BREAK_POINT_ERR;
            // }

            OTA_PRINTF("%s, curFwNum[%x], fwDataRecvSize[%x], backAddr[%x]", __func__, sTlkMwOtaCtrl.curFwNum, sTlkMwOtaCtrl.fwDataRecvSize, sTlkMwOtaCtrl.backAddr);

            uint8_t pBuffer[4] = {0};
            uint8_t buffLen    = 0;
            pBuffer[buffLen++] = TLK_FIRMWARE_OTA_ING;
            pBuffer[buffLen++] = 0;
            tlkmw_ota_update_ota_status(pBuffer, buffLen, NULL);
            //TODO:special
            tlkmw_ota_start_customer_init();
        } else {
            status = TLK_OTA_RESULT_VERSION_COMPARE_ERR;
        }
    } break;
    case TLK_OTA_OPC_PARAM_NEGO_REQ:
    {
        if (tlkmw_ota_param_negotiation(pData, dataLen, UserArg) != OTA_NONE) {
            status = TLK_OTA_RESULT_PARAM_NEGO_ERR;
        }
    } break;
    case TLK_OTA_OPC_BREAK_INFO_REQ:
    {
        if (tlkmw_ota_break_point_request(pData, dataLen, UserArg) != OTA_NONE) {
            status = TLK_OTA_RESULT_BREAK_POINT_ERR;
        }
    } break;
    case TLK_OTA_OPC_TOTAL_FW_DESCRRIPTOR_NOTIFY:
    {
        if (tlkmw_ota_total_fw_descriptors_check(pData, dataLen, UserArg) != OTA_NONE) {
            status = TLK_OTA_RESULT_TOTAL_FW_DESCRRIPTORS_ERR;
        }
    } break;
    case TLK_OTA_OPC_START_OTA_REQ:
    {
        tlkmw_ota_start_ota_deal(pData, dataLen, UserArg);
    } break;
    case TLK_OTA_OPC_CUR_FW_DESCRRIPTOR_RSP:
    {
        if (tlkmw_ota_cur_fw_descriptor_check(pData, dataLen, UserArg) != OTA_NONE) {
            status = TLK_OTA_RESULT_CUR_FW_DESCRRIPTORS_ERR;
        }
    } break;
    case TLK_OTA_OPC_CUR_FW_DATA_RSP:
    {
        if (tlkmw_ota_cur_data_deal(pData, dataLen, UserArg) != OTA_NONE) {
            status = TLK_OTA_RESULT_FW_CHECK_ERR;
        }
    } break;
    case TLK_OTA_OPC_OTA_TRANS_END:
        tlkmw_ota_end_deal(pData, dataLen, UserArg);
        break;
    case TLK_OTA_OPC_OTA_TRANS_END_ACK:
        /*Pre-remove the timer to avoid timeouts from prolonged end ACK processing.*/
        tlkmw_ota_end_ack_deal(pData, dataLen, UserArg);
        tlk_ota_general_protocol_reset();
        return;
    default:
        OTA_PRINTF("[OTA] unknow opcode:%d", opcode);
        return;
    }

    if (status != TLK_OTA_RESULT_SUCCESS) {
        tlkmw_ota_send_trans_end(status, UserArg);
    }

    sTlkMwOtaCtrl.timeout = TLKMW_OTA_TRANS_TIMEOUT;
}

/**
 * @brief   Initialize the OTA protocol detail module
 * @param   pInterface - Pointer to OTA interface structure
 * @return  OTA_NONE if successful, error code otherwise
 */
int tlk_ota_general_protocol_detail_init(nvds_ota_Interface_t *pInterface)
{
    if (pInterface == NULL || pInterface->nvds_ota_malloc == NULL) {
        return -OTA_INITERR;
    }

    OTA_MEMSET(&sTlkMwOtaCtrl, 0, sizeof(sTlkMwOta_t));

    sTlkMwOtaCtrl.ota_intf = pInterface;
    sTlkMwOtaCtrl.status   = TLK_OTA_STATUS_IDLE;
    return tlkmw_ota_load_cur_img_header();
}

/**
 * @brief   Get pointer to the received image header structure
 * @param   None
 * @return  Pointer to the received image header structure
 */
sTlk_total_fw_descriptors_t *tlkmw_get_recvImgHeaderAddr(void)
{
    return &sTlkMwRecvImgHeader;
}

/**
 * @brief   Get pointer to the OTA control structure
 * @param   None
 * @return  Pointer to the OTA control structure
 */
sTlkMwOta_t *tlkmw_get_otaCtrl(void)
{
    return &sTlkMwOtaCtrl;
}
