/********************************************************************************************************
 * @file    svc_battery.h
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


//BAS: Battery Level Service

typedef enum
{
    DEVICE_IN_CHARGING = 0xBB,
    DEVICE_NO_CHARGING = 0xAE,
} blc_bas_battery_power_state_enum;

/**
 * @brief      for user add default BAS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_addBasGroup(void);

/**
 * @brief      for user remove default BAS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_removeBasGroup(void);

/**
 * @brief      for user register read or write attribute value callback function in BAS service.
 * @param[in]  readCback: read attribute value callback function pointer.
 * @return     none.
 */
void blc_svc_basCbackRegister(atts_r_cb_t readCback);

/**
 * @brief      for use set battery level value.
 * @param[in]  batterylevel - the value that battery level.
 * @return     none.
 */
void blc_svc_basSetBatteryLevel(uint8_t batteryLevel);

/**
 * @brief      for use set battery power state value.
 * @param[in]  powerState - the value that battery power state.
 * @return     none.
 */
void blc_svc_basSetPowerState(blc_bas_battery_power_state_enum powerState);
