/********************************************************************************************************
 * @file    pbp_ext_adv.h
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
struct ad_data_pbp_broadcast_name
{
    struct ltv_data header; //DT_BROADCAST_NAME
    uint8_t         broadcast_name[32];
};

struct pbp_ext_adv_data
{
    const char *complete_name;
    const char *broadcast_name;
    uint32_t    broadcast_id;
    // todo
    uint8_t encryption       : 1;
    uint8_t standard_quality : 1;
    uint8_t high_quality     : 1;
    uint8_t RFU              : 5;
    // TODO: add metadata fields
};

/**
 * @brief       Creates the AD data for the PBP broadcast.
 * @param[in]   ext_adv_data - pointer to the extended advertising data.
 * @param[out]  outData - pointer to the output data.
 * @return      the length of the output data.
 * @note        The output data is the AD data for the PBP broadcast.
 */
int32_t bis_source_create_adv_ad_data(struct pbp_ext_adv_data *ext_adv_data, void *outData);
