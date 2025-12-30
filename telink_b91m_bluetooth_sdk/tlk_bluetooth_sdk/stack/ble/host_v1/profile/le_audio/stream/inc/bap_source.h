/********************************************************************************************************
 * @file    bap_source.h
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

struct lea_BASE_BIS_param
{
    uint8_t                            bis_index;
    struct lea_codec_spec_config_param codec_config;
};

struct lea_BASE_BIG_param
{
    uint8_t                            bis_num;
    struct lea_codec_id                codec_id;
    struct lea_codec_spec_config_param codec_config;
    struct lea_metadata_param          metadata;
    const struct lea_BASE_BIS_param   *bis_param;
};

struct lea_broadcast_audio_announcements_param
{
    uint32_t                         presentation_delay; /** < Range: 0x00000000-0x00FFFFFF, units: us */
    uint8_t                          sub_group_num;
    const struct lea_BASE_BIG_param *big_param;
};

/**
 * @brief       Sets the BASE information to the address.
 * @param[in]   p_param - pointer to the BASE information parameters.
 * @param[out]  dst - pointer to the destination buffer.
 * @return      pointer to the destination buffer.
 * @note        The BASE information is set to the address.
 */
uint8_t *lea_bap_set_BASE_to_address(const struct lea_broadcast_audio_announcements_param *p_param, uint8_t *dst);
