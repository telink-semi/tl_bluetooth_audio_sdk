/********************************************************************************************************
 * @file    svc_battery.c
 *
 * @brief   This is the source file for TLSR/TL
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
#include <stddef.h>

#include "common/types.h"
#include "common/utility.h"
#include "common/compiler.h"

#include "../../../l2cap/att/inc/ble_att_uuid.h"
#include "../../../l2cap/att/inc/ble_att_service.h"

#include "../../../l2cap/att/inc/uuid16bit.h"

#include "../../inc/svc.h"
#include "../../inc/svc_format.h"

#include "../svc_gatt.h"

#include "svc_battery.h"

#ifndef BAS_BATTERY_POWER_STATE
#define BAS_BATTERY_POWER_STATE 0
#endif

#define BAS_START_HDL SERVICE_BATTERY_HDL

_attribute_ble_data_retention_ static uint8_t basBatteryLevelValue    = 0x64;
static const uint16_t                         basBatteryLevelValueLen = sizeof(basBatteryLevelValue);

#if BAS_BATTERY_POWER_STATE
_attribute_ble_data_retention_ static uint8_t basBatteryPowerStateValue    = DEVICE_IN_CHARGING;
static const uint16_t                         basBatteryPowerStateValueLen = sizeof(basBatteryPowerStateValue);
#endif

/*
 * @brief the structure for default BAS service List.
 */
static const struct atts_attribute basList[] = {
    ATTS_PRIMARY_SERVICE(serviceBatteryUuid),

    //Battery level
    ATTS_CHAR_UUID_READ_ENTITY_CB(charPropReadNotify, characteristicBatteryLevelUuid, basBatteryLevelValue),
    ATTS_COMMON_CCC_DEFINE,

#if BAS_BATTERY_POWER_STATE
    //Battery Power State
    ATTS_CHAR_UUID_READ_ENTITY_CB(charPropReadNotify, characteristicBatteryPowerStateUuid, basBatteryPowerStateValue),
    ATTS_COMMON_CCC_DEFINE,
#endif
};

/*
 * @brief the structure for default BAS service group.
 */
_attribute_ble_data_retention_ static struct atts_group svcBasGroup = {
    NULL, basList, NULL, NULL, BAS_START_HDL, 0,
};

const uint16_t basIncludeVal[3] = {SERVICE_BATTERY_HDL, SERVICE_BATTERY_HDL + ARRAY_SIZE(basList) - 1, SERVICE_UUID_BATTERY};

/**
 * @brief      for user add default BAS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_addBasGroup(void)
{
    svcBasGroup.endHandle = svcBasGroup.startHandle + ARRAY_SIZE(basList) - 1;
    blc_gatts_addAttributeServiceGroup(&svcBasGroup);
}

/**
 * @brief      for user remove default BAS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_removeBasGroup(void)
{
    blc_gatts_removeAttributeServiceGroup(BAS_START_HDL);
}

/**
 * @brief      for user register read or write attribute value callback function in BAS service.
 * @param[in]  readCback: read attribute value callback function pointer.
 * @return     none.
 */
void blc_svc_basCbackRegister(atts_r_cb_t readCback)
{
    svcBasGroup.readCallback = readCback;
}

/**
 * @brief      for use set battery level value.
 * @param[in]  batterylevel - the value that battery level.
 * @return     none.
 */
void blc_svc_basSetBatteryLevel(uint8_t batteryLevel)
{
    if (batteryLevel <= 100) {
        basBatteryLevelValue = batteryLevel;
    }
}

/**
 * @brief      for use set battery power state value.
 * @param[in]  powerState - the value that battery power state.
 * @return     none.
 */
void blc_svc_basSetPowerState(blc_bas_battery_power_state_enum powerState)
{
#if BAS_BATTERY_POWER_STATE
    basBatteryPowerStateValue = powerState;
#else
    (void)powerState;
#endif
}
