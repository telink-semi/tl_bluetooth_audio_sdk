/********************************************************************************************************
 * @file    tmass.h
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
// TMASS: Telephony and Media Audio Service Server

struct ad_data_tamp_role
{
    uint8_t              length;   /** < Fixed length of 0x05 */
    uint8_t              type;     /** < fixed type of DT_SERVICE_DATA */
    uint16_t             tamsUuid; /** < fixed SERVICE_UUID_TELEPHONY_AND_MEDIA_AUDIO */
    struct tmap_role_tag role;
} __attribute__((packed));

struct ble_tmass_register_param
{
    struct tmap_role_tag role;
};

#define BLE_TMASS_INITIAL_PARAMS(cg, ct, ums, umr, bms, bmr)            \
    (&(const struct ble_tmass_register_param){.role = {                 \
                                                  .cg_support  = (cg),  \
                                                  .ct_support  = (ct),  \
                                                  .ums_support = (ums), \
                                                  .umr_support = (umr), \
                                                  .bms_support = (bms), \
                                                  .bmr_support = (bmr), \
                                                  .reserved    = 0,     \
                                              }})

/**
 *   @brief Register the Telephony and Media Audio Service server control.
 *
 *   @param[in] param Pointer to the structure containing the initial values for the service.
 *
 *   @note This function should be called before any other function in the PACSS module.
 *
 *   @return None.
 */

void ble_lea_register_TMAS_control_server(const struct ble_tmass_register_param *param);
