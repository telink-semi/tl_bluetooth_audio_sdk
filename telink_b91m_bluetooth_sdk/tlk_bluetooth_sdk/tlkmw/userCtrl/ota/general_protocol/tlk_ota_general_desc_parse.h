/********************************************************************************************************
 * @file    tlk_ota_general_desc_parse.h
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
#ifndef TLK_OTA_GENERAL_DESC_PARSE_H
#define TLK_OTA_GENERAL_DESC_PARSE_H

#include "tlk_ota_general_protocol.h"

typedef union
{
    uint32_t crc32;
    uint8_t  sha256[20];
    uint8_t  resv[32];
} sTlk_cur_fw_entity_crc_t;

typedef struct
{
    uint32_t                 fw_pid;
    uint32_t                 fw_vid;
    uint32_t                 fw_startAddr;
    uint8_t                  fw_featureMap[8];
    uint8_t                  resv[12];
    sTlk_cur_fw_entity_crc_t fw_crc;
} sTlk_cur_fw_descriptor_header_t;

typedef struct
{
    uint32_t fw_type;
    uint32_t fw_version;
    uint32_t start_addr; //offset
    uint32_t fw_size;
    uint8_t  resv[16];
} sTlk_fw_descriptors_node_t;

struct sTlk_fw_descriptors_list_t
{
    sTlk_fw_descriptors_node_t         fw_img;
    struct sTlk_fw_descriptors_list_t *pNext;
};

struct sTlk_fw_info_list_t
{
    uint32_t                    bin_type;
    uint32_t                    bin_version;
    struct sTlk_fw_info_list_t *pNext;
};

struct sTlk_fw_group_list_t
{
    uint32_t                     bin_count;
    struct sTlk_fw_info_list_t  *fw_info_list;
    uint32_t                     resv;
    struct sTlk_fw_group_list_t *pNext;
};

typedef struct
{
    uint32_t                           img_version;
    uint32_t                           img_valid_size;
    uint32_t                           fw_number;
    uint32_t                           total_size;
    uint32_t                           fw_group_number;
    struct sTlk_fw_group_list_t       *fw_group_list;
    struct sTlk_fw_descriptors_list_t *fw_descpts_list; //FW Descripotrs List
    uint8_t                            recv[16];
    sTlk_cur_fw_entity_crc_t           img_crc;
} sTlk_total_fw_descriptors_t;

/**
 * @brief      Get firmware group node by index
 * @param[in]  fw_group_list - pointer to firmware group list
 * @param[in]  index         - index of the node to retrieve
 * @param[out] none
 * @return     struct sTlk_fw_group_list_t* - pointer to the firmware group node, or NULL if not found
 */
struct sTlk_fw_group_list_t *tlkmw_get_fw_group_node_by_index(struct sTlk_fw_group_list_t *fw_group_list, int index);

/**
 * @brief      Get firmware info node by index
 * @param[in]  fw_info_list - pointer to firmware info list
 * @param[in]  index        - index of the node to retrieve
 * @param[out] none
 * @return     struct sTlk_fw_info_list_t* - pointer to the firmware info node, or NULL if not found
 */
struct sTlk_fw_info_list_t *tlkmw_get_fw_info_node_by_index(struct sTlk_fw_info_list_t *fw_info_list, int index);

/**
 * @brief      Get firmware descriptor node by index
 * @param[in]  fw_descpts_list - pointer to firmware descriptor list
 * @param[in]  index           - index of the node to retrieve
 * @param[out] none
 * @return     sTlk_fw_descriptors_node_t* - pointer to the firmware descriptor node, or NULL if not found
 */
sTlk_fw_descriptors_node_t *tlkmw_get_fw_descpts_node_by_index(struct sTlk_fw_descriptors_list_t *fw_descpts_list, int index);

/**
 * @brief      Free total firmware descriptors memory
 * @param[in]  pTotalFwDescptors - pointer to total firmware descriptors structure
 * @param[in]  pInterface        - pointer to OTA interface
 * @param[out] none
 * @return     none
 */
void tlkmw_free_total_fw_descptors(sTlk_total_fw_descriptors_t *pTotalFwDescptors, nvds_ota_Interface_t pInterface);

/**
 * @brief      Parse total firmware descriptors from data
 * @param[in]  pData             - pointer to data buffer
 * @param[in]  dataLen           - length of data
 * @param[out] pTotalFwDescptors - pointer to total firmware descriptors structure
 * @param[in]  pInterface        - pointer to OTA interface
 * @return     int - OTA_NONE if success, error code otherwise
 */
int tlk_ota_parse_total_fw_descriptors(uint8_t *pData, uint16_t dataLen, sTlk_total_fw_descriptors_t *pTotalFwDescptors, nvds_ota_Interface_t pInterface);
#endif
