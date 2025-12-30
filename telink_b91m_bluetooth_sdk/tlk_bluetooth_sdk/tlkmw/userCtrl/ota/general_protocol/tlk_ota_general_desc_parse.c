/********************************************************************************************************
 * @file    tlk_ota_general_desc_parse.c
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

/**
 * @brief      Get firmware descriptor node by index from firmware descriptor list
 * @param[in]  fw_descpts_list - Pointer to firmware descriptor list
 * @param[in]  index           - Index of the node to retrieve
 * @return     sTlk_fw_descriptors_node_t* - Pointer to the firmware descriptor node, or NULL if not found
 */
sTlk_fw_descriptors_node_t *tlkmw_get_fw_descpts_node_by_index(struct sTlk_fw_descriptors_list_t *fw_descpts_list, int index)
{
    if (fw_descpts_list == NULL) {
        return NULL;
    }

    struct sTlk_fw_descriptors_list_t *current      = fw_descpts_list;
    int                                currentIndex = 0;

    while (current != NULL) {
        if (currentIndex == index) {
            return &(current->fw_img);
        }
        current = current->pNext;
        currentIndex++;
    }

    return NULL;
}

/**
 * @brief      Get firmware group node by index from firmware group list
 * @param[in]  fw_group_list - Pointer to firmware group list
 * @param[in]  index         - Index of the node to retrieve
 * @return     struct sTlk_fw_group_list_t* - Pointer to the firmware group node, or NULL if not found
 */
struct sTlk_fw_group_list_t *tlkmw_get_fw_group_node_by_index(struct sTlk_fw_group_list_t *fw_group_list, int index)
{
    if (fw_group_list == NULL) {
        return NULL;
    }

    struct sTlk_fw_group_list_t *current      = fw_group_list;
    int                          currentIndex = 0;

    while (current != NULL) {
        if (currentIndex == index) {
            return current;
        }
        current = current->pNext;
        currentIndex++;
    }

    return NULL;
}

/**
 * @brief      Get firmware info node by index from firmware info list
 * @param[in]  fw_info_list - Pointer to firmware info list
 * @param[in]  index        - Index of the node to retrieve
 * @return     struct sTlk_fw_info_list_t* - Pointer to the firmware info node, or NULL if not found
 */
struct sTlk_fw_info_list_t *tlkmw_get_fw_info_node_by_index(struct sTlk_fw_info_list_t *fw_info_list, int index)
{
    if (fw_info_list == NULL) {
        return NULL;
    }

    struct sTlk_fw_info_list_t *current      = fw_info_list;
    int                         currentIndex = 0;

    while (current != NULL) {
        if (currentIndex == index) {
            return current;
        }
        current = current->pNext;
        currentIndex++;
    }

    return NULL;
}

/**
 * @brief      Free firmware group list memory
 * @param[in]  pTotalFwDescptors - Pointer to total firmware descriptors
 * @param[in]  pInterface        - Pointer to NVDS OTA interface
 * @return     none
 */
static void tlkmw_ota_free_fw_group_list(sTlk_total_fw_descriptors_t *pTotalFwDescptors, nvds_ota_Interface_t *pInterface)
{
    if (pTotalFwDescptors == NULL || pInterface == NULL || pInterface->nvds_ota_free == NULL) {
        return;
    }

    struct sTlk_fw_group_list_t *pFwGroupListNode = pTotalFwDescptors->fw_group_list;

    /*free fw_group_list*/
    while (pFwGroupListNode != NULL) {
        struct sTlk_fw_group_list_t *pFwGroupListNext = pFwGroupListNode->pNext;

        /*free fw_info_list*/
        struct sTlk_fw_info_list_t *pFwInfoListNode = pFwGroupListNode->fw_info_list;
        while (pFwInfoListNode != NULL) {
            struct sTlk_fw_info_list_t *pFwInfoListNext = pFwInfoListNode->pNext;
            pInterface->nvds_ota_free(pFwInfoListNode);
            pFwInfoListNode = pFwInfoListNext;
        }

        pInterface->nvds_ota_free(pFwGroupListNode);
        pFwGroupListNode = pFwGroupListNext;
    }

    pTotalFwDescptors->fw_group_list = NULL;
}

/**
 * @brief      Parse firmware group list from data buffer
 * @param[in]  pData             - Pointer to data buffer
 * @param[in]  dataLen           - Length of data buffer
 * @param[in]  offset            - Pointer to offset in data buffer
 * @param[out] pTotalFwDescptors - Pointer to total firmware descriptors
 * @param[in]  pInterface        - Pointer to NVDS OTA interface
 * @return     int - OTA_NONE if success, error code otherwise
 */
int tlkmw_parse_fw_group_list(uint8_t *pData, uint32_t dataLen, uint32_t *offset, sTlk_total_fw_descriptors_t *pTotalFwDescptors, nvds_ota_Interface_t *pInterface)
{
    if (pData == NULL || offset == NULL || pTotalFwDescptors == NULL || pInterface == NULL || pInterface->nvds_ota_malloc == NULL) {
        return -OTA_PARAMERR;
    }

    pTotalFwDescptors->fw_group_list = (struct sTlk_fw_group_list_t *)pInterface->nvds_ota_malloc(sizeof(struct sTlk_fw_group_list_t));
    if (pTotalFwDescptors->fw_group_list == NULL) {
        return -OTA_MALLOCERR;
    }
    pTotalFwDescptors->fw_group_list->pNext = NULL;

    struct sTlk_fw_group_list_t *pFwGroupListTail = NULL;

    for (uint8_t index = 0; index < pTotalFwDescptors->fw_group_number; index++) {
        if (pFwGroupListTail != NULL) {
            struct sTlk_fw_group_list_t *pFwGroupListTemp = (struct sTlk_fw_group_list_t *)pInterface->nvds_ota_malloc(sizeof(struct sTlk_fw_group_list_t));
            if (pFwGroupListTemp == NULL) {
                return -OTA_MALLOCERR;
            }
            pFwGroupListTemp->pNext = NULL;
            pFwGroupListTail->pNext = pFwGroupListTemp;
            pFwGroupListTail        = pFwGroupListTemp;
        } else {
            pFwGroupListTail = pTotalFwDescptors->fw_group_list;
        }

        uint32_t bin_count = 0;
        OTA_ARRAY_TO_UINT32L(pData, *offset, bin_count);
        pFwGroupListTail->bin_count = bin_count;
        *offset += 4;

        pFwGroupListTail->fw_info_list = (struct sTlk_fw_info_list_t *)pInterface->nvds_ota_malloc(sizeof(struct sTlk_fw_info_list_t));
        if (pFwGroupListTail->fw_info_list == NULL) {
            return -OTA_MALLOCERR;
        }
        pFwGroupListTail->fw_info_list->pNext       = NULL;
        struct sTlk_fw_info_list_t *pFwInfoListTail = NULL;

        while (bin_count > 0) {
            if (pFwInfoListTail != NULL) {
                struct sTlk_fw_info_list_t *pFwInfoListTemp = (struct sTlk_fw_info_list_t *)pInterface->nvds_ota_malloc(sizeof(struct sTlk_fw_info_list_t));
                if (pFwInfoListTemp == NULL) {
                    return -OTA_MALLOCERR;
                }
                pFwInfoListTemp->pNext = NULL;
                pFwInfoListTail->pNext = pFwInfoListTemp;
                pFwInfoListTail        = pFwInfoListTemp;
            } else {
                pFwInfoListTail = pFwGroupListTail->fw_info_list;
            }

            OTA_ARRAY_TO_UINT32L(pData, *offset, pFwInfoListTail->bin_type);
            *offset += 4;
            OTA_ARRAY_TO_UINT32L(pData, *offset, pFwInfoListTail->bin_version);
            *offset += 4;
            bin_count--;
        }

        if (*offset > dataLen) {
            return -OTA_PARAMERR;
        }
    }

    *offset += 4; //resv

    if (*offset > dataLen) {
        return -OTA_PARAMERR;
    }

    return OTA_NONE;
}

/**
 * @brief      Print firmware group list for debugging
 * @param[in]  pTotalFwDescptors - Pointer to total firmware descriptors
 * @return     none
 */
void tlkmw_print_fw_group_list(sTlk_total_fw_descriptors_t *pTotalFwDescptors)
{
    if (pTotalFwDescptors == NULL) {
        return;
    }

    struct sTlk_fw_group_list_t *pFwGroupListHead = pTotalFwDescptors->fw_group_list;

    while (pFwGroupListHead != NULL) {
        struct sTlk_fw_group_list_t *pFwGroupListTemp = pFwGroupListHead->pNext;
        OTA_PRINTF("fw_group_list bin_count %d", pFwGroupListHead->bin_count);

        struct sTlk_fw_info_list_t *pFwInfoListHead = pFwGroupListHead->fw_info_list;

        while (pFwInfoListHead != NULL) {
            struct sTlk_fw_info_list_t *pFwInfoListTemp = pFwInfoListHead->pNext;
            OTA_PRINTF("fw_info_list bin_type %d, bin_version %d", pFwInfoListHead->bin_type, pFwInfoListHead->bin_version);
            pFwInfoListHead = pFwInfoListTemp;
        }

        pFwGroupListHead = pFwGroupListTemp;
    }
}

/**
 * @brief      Free firmware descriptors list memory
 * @param[in]  pTotalFwDescptors - Pointer to total firmware descriptors
 * @param[in]  pInterface        - Pointer to NVDS OTA interface
 * @return     none
 */
static void tlkmw_ota_free_fw_descriptors_list(sTlk_total_fw_descriptors_t *pTotalFwDescptors, nvds_ota_Interface_t *pInterface)
{
    if (pTotalFwDescptors == NULL || pInterface == NULL || pInterface->nvds_ota_free == NULL) {
        return;
    }

    struct sTlk_fw_descriptors_list_t *pTemp = pTotalFwDescptors->fw_descpts_list;
    struct sTlk_fw_descriptors_list_t *pNext = NULL;

    while (pTemp != NULL) {
        pNext = pTemp->pNext;
        pInterface->nvds_ota_free(pTemp);
        pTemp = pNext;
    }

    pTotalFwDescptors->fw_descpts_list = NULL;
}

/**
 * @brief      Parse firmware descriptors list from data buffer
 * @param[in]  pData             - Pointer to data buffer
 * @param[in]  dataLen           - Length of data buffer
 * @param[out] pTotalFwDescptors - Pointer to total firmware descriptors
 * @param[in]  pInterface        - Pointer to NVDS OTA interface
 * @return     int - OTA_NONE if success, error code otherwise
 */
int tlkmw_ota_parse_fw_descriptors_list(uint8_t *pData, uint16_t dataLen, sTlk_total_fw_descriptors_t *pTotalFwDescptors, nvds_ota_Interface_t *pInterface)
{
    if (pData == NULL || dataLen == 0 || pTotalFwDescptors == NULL || pInterface == NULL || pInterface->nvds_ota_malloc == NULL) {
        return -OTA_PARAMERR;
    }
    uint32_t offset = 0;

    pTotalFwDescptors->fw_descpts_list = (struct sTlk_fw_descriptors_list_t *)pInterface->nvds_ota_malloc(sizeof(struct sTlk_fw_descriptors_list_t));
    if (pTotalFwDescptors->fw_descpts_list == NULL) {
        return -OTA_MALLOCERR;
    }
    struct sTlk_fw_descriptors_list_t *pNode = pTotalFwDescptors->fw_descpts_list;

    OTA_MEMCPY(&pNode->fw_img, pData + offset, sizeof(sTlk_fw_descriptors_node_t));

    pNode->pNext = NULL;
    offset += sizeof(sTlk_fw_descriptors_node_t);

    while (offset < dataLen) {
        struct sTlk_fw_descriptors_list_t *pTemp = (struct sTlk_fw_descriptors_list_t *)pInterface->nvds_ota_malloc(sizeof(struct sTlk_fw_descriptors_list_t));
        if (pTemp == NULL) {
            return -OTA_MALLOCERR;
        }
        OTA_MEMCPY(&pTemp->fw_img, pData + offset, sizeof(sTlk_fw_descriptors_node_t));
        pTemp->pNext = NULL;
        pNode->pNext = pTemp;
        pNode        = pTemp;
        offset += sizeof(sTlk_fw_descriptors_node_t);
    }

    return OTA_NONE;
}

/**
 * @brief      Print firmware descriptors list for debugging
 * @param[in]  pTotalFwDescptors - Pointer to total firmware descriptors
 * @return     none
 */
void tlkmw_ota_print_fw_descriptors_list(sTlk_total_fw_descriptors_t *pTotalFwDescptors)
{
    if (pTotalFwDescptors == NULL) {
        return;
    }

    struct sTlk_fw_descriptors_list_t *pNode = pTotalFwDescptors->fw_descpts_list;

    while (pNode != NULL) {
        OTA_PRINTF("CUR type[%x],version[%x],start[%x],size[%x]", pNode->fw_img.fw_type, pNode->fw_img.fw_version, pNode->fw_img.start_addr, pNode->fw_img.fw_size);
        pNode = pNode->pNext;
    }
}

/**
 * @brief      Parse total firmware descriptors from data buffer
 * @param[in]  pData             - Pointer to data buffer
 * @param[in]  dataLen           - Length of data buffer
 * @param[out] pTotalFwDescptors - Pointer to total firmware descriptors
 * @param[in]  pInterface        - Pointer to NVDS OTA interface
 * @return     int - OTA_NONE if success, error code otherwise
 */
int tlk_ota_parse_total_fw_descriptors(uint8_t *pData, uint16_t dataLen, sTlk_total_fw_descriptors_t *pTotalFwDescptors, nvds_ota_Interface_t *pInterface)
{
    if (pData == NULL || dataLen < 20 || pTotalFwDescptors == NULL || pInterface == NULL || pInterface->nvds_ota_malloc == NULL) {
        return -OTA_PARAMERR;
    }

    tlkmw_free_total_fw_descptors(pTotalFwDescptors, pInterface);

    /*Verify crc.*/
    uint32_t img_crc = 0xFFFFFFFF;
    img_crc          = calculate_CRC32(img_crc, pData, dataLen - sizeof(sTlk_cur_fw_entity_crc_t));

    memcpy(&pTotalFwDescptors->img_crc.crc32, pData + dataLen - sizeof(sTlk_cur_fw_entity_crc_t), sizeof(sTlk_cur_fw_entity_crc_t));

    if (pTotalFwDescptors->img_crc.crc32 != img_crc) {
        return -OTA_PARAMERR;
    }

    int      ret    = 0;
    uint32_t offset = 0;

    OTA_ARRAY_TO_UINT32L(pData, offset, pTotalFwDescptors->img_version);
    offset += 4;
    OTA_ARRAY_TO_UINT32L(pData, offset, pTotalFwDescptors->img_valid_size);
    offset += 4;
    OTA_ARRAY_TO_UINT32L(pData, offset, pTotalFwDescptors->fw_number);
    offset += 4;
    OTA_ARRAY_TO_UINT32L(pData, offset, pTotalFwDescptors->total_size);
    offset += 4;
    OTA_ARRAY_TO_UINT32L(pData, offset, pTotalFwDescptors->fw_group_number);
    offset += 4;

    /*Parse fw_group_list*/
    ret = tlkmw_parse_fw_group_list(pData, pTotalFwDescptors->img_valid_size, &offset, pTotalFwDescptors, pInterface);
    if (ret != OTA_NONE) {
        tlkmw_ota_free_fw_group_list(pTotalFwDescptors, pInterface);
        return ret;
    }

    /*Parse fw_descpts_list*/
    dataLen -= (offset + 16 + sizeof(sTlk_cur_fw_entity_crc_t)); //16bytes resv + 32bytes crc
    ret = tlkmw_ota_parse_fw_descriptors_list(pData + offset, dataLen, pTotalFwDescptors, pInterface);
    if (ret != OTA_NONE) {
        tlkmw_ota_free_fw_descriptors_list(pTotalFwDescptors, pInterface);
        return ret;
    }

    return OTA_NONE;
}

/**
 * @brief      Free total firmware descriptors memory
 * @param[in]  pTotalFwDescptors - Pointer to total firmware descriptors
 * @param[in]  pInterface        - Pointer to NVDS OTA interface
 * @return     none
 */
void tlkmw_free_total_fw_descptors(sTlk_total_fw_descriptors_t *pTotalFwDescptors, nvds_ota_Interface_t *pInterface)
{
    if (pTotalFwDescptors == NULL || pInterface == NULL || pInterface->nvds_ota_free == NULL) {
        return;
    }

    if (pTotalFwDescptors->fw_descpts_list != NULL) {
        tlkmw_ota_free_fw_descriptors_list(pTotalFwDescptors, pInterface);
    }
    if (pTotalFwDescptors->fw_group_list != NULL) {
        tlkmw_ota_free_fw_group_list(pTotalFwDescptors, pInterface);
    }
}
