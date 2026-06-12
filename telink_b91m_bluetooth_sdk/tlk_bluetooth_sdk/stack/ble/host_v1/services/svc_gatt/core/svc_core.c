/********************************************************************************************************
 * @file    svc_core.c
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
#include <string.h>

#include "common/types.h"
#include "common/utility.h"
#include "common/compiler.h"

#include "../../../l2cap/att/inc/ble_att_uuid.h"
#include "../../../l2cap/att/inc/ble_att_service.h"

#include "../../../l2cap/att/inc/uuid16bit.h"

#include "../../../gatt/gatts/inc/ble_gatts.h"

#include "../../inc/svc.h"
#include "../../inc/svc_format.h"

#include "../svc_gatt.h"

#ifndef DEFAULT_DEV_APPEARANCE
#define DEFAULT_DEV_APPEARANCE 0x0000 /** < unknown device appearance */
#endif

#ifndef DEFAULT_DEV_NAME
#define DEFAULT_DEV_NAME "Default Device"
#endif

#ifndef SERVER_EATT_SUPPORTED
#define SERVER_EATT_SUPPORTED 0
#endif

#ifndef CLIENT_ROBUST_CACHING
#define CLIENT_ROBUST_CACHING 0
#endif

#ifndef CLIENT_EATT_BEARER
#define CLIENT_EATT_BEARER 0
#endif

#ifndef CLIENT_MULTI_HANDLE_VALUE_NOTIFICATION
#define CLIENT_MULTI_HANDLE_VALUE_NOTIFICATION 0
#endif

#define GAP_START_HDL SERVICE_GENERIC_ACCESS_HDL

_attribute_data_retention_ static char     defaultDevName[32] = DEFAULT_DEV_NAME;
_attribute_data_retention_ static uint16_t defaultDevNameLen  = sizeof(DEFAULT_DEV_NAME) - 1;

_attribute_data_retention_ static uint16_t defaultAppearance    = DEFAULT_DEV_APPEARANCE;
static const uint16_t                      defaultAppearanceLen = sizeof(defaultAppearance);

static uint16_t       defaultPeriConnParameters[]  = {20, 40, 0, 100}; //gap_periConnectParams_t
static const uint16_t defaultPeriConnParametersLen = sizeof(defaultPeriConnParameters);

/*
 * @brief the structure for default GAP service List.
 */
static const struct atts_attribute gapList[] = {
    ATTS_PRIMARY_SERVICE(serviceGenericAccessAttUuid),

    //device name
    ATTS_CHAR_UUID_READ_POINT_NOCB(charPropRead, characteristicDeviceNameAttUuid, defaultDevName),

    //Appearance
    ATTS_CHAR_UUID_READ_ENTITY_NOCB(charPropRead, characteristicAppearanceAttUuid, defaultAppearance),

    //period connect parameter
    ATTS_CHAR_UUID_READ_ENTITY_NOCB(charPropRead, characteristicPeripheralPreferredConnParamAttUuid, defaultPeriConnParameters),
};


/*
 * @brief the structure for default GAP service group.
 */
_attribute_data_retention_ static struct atts_group svcGapGroup = {NULL, gapList, NULL, NULL, GAP_START_HDL, 0};

// GATT service
#define GATT_START_HDL SERVICE_GENERIC_ATTRIBUTE_HDL

struct server_supported_features
{
    uint8_t eattSupported : 1; /** EATT supported flag */
    uint8_t reserved      : 7; /** reserved */
};

struct client_supported_features
{
    uint8_t robustCaching                : 1;
    uint8_t eattBearer                   : 1;
    uint8_t multiHandleValueNotification : 1;
    uint8_t reserved                     : 5;
};

_attribute_data_retention_ static uint16_t gattServiceChangeVal[2] = {0x0000, 0x0000};
static const uint16_t                      gattServiceChangeValLen = sizeof(gattServiceChangeVal);

static const struct server_supported_features serverSuppFeatVal = {
    .eattSupported = SERVER_EATT_SUPPORTED,
    .reserved      = 0,
};
static const uint16_t serverSuppFeatValLen = sizeof(struct server_supported_features);

static const struct client_supported_features clientSuppFeatVal = {
    .robustCaching                = CLIENT_ROBUST_CACHING,
    .eattBearer                   = CLIENT_EATT_BEARER,
    .multiHandleValueNotification = CLIENT_MULTI_HANDLE_VALUE_NOTIFICATION,
    .reserved                     = 0,
};
static const uint16_t clientSuppFeatValLen = sizeof(struct client_supported_features);

_attribute_data_retention_ static uint8_t databaseHashVal[16]; //database hash calculation
static const uint16_t                     databaseHashValLen = 0x10;

/*
 * @brief the structure for default GATT service List.
 */
static const struct atts_attribute gattList[] = {
    ATTS_PRIMARY_SERVICE(serviceGenericAttributeAttUuid),

    //service change
    ATTS_CHAR_UUID_NO_RDWR_POINT_NOCB(charPropIndicate, characteristicServiceChangedAttUuid, gattServiceChangeVal),
    ATTS_COMMON_CCC_DEFINE,

    //serverSuppFeat
    ATTS_CHAR_UUID_READ_ENTITY_NOCB(charPropRead, characteristicServerSupportedFeaturesAttUuid, serverSuppFeatVal),

    //clientSuppFeat
    ATTS_CHAR_UUID_RDWR_ENTITY_RWCB(charPropReadWriteWriteWithout, characteristicClientSupportedFeaturesAttUuid, clientSuppFeatVal),

    //database Hash
    ATTS_CHAR_UUID_READ_ENTITY_NOCB(charPropRead, characteristicDatabaseHashAttUuid, databaseHashVal),
};

/*
 * @brief the structure for default GATT service group.
 */
_attribute_data_retention_ static struct atts_group svcGattGroup = {
    NULL, gattList, NULL, NULL, GATT_START_HDL, 0,
};

/**
 * @brief      for user add default GATT and GAP service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_addCoreGroup(void)
{
    svcGapGroup.endHandle  = svcGapGroup.startHandle + ARRAY_SIZE(gapList) - 1;
    svcGattGroup.endHandle = svcGattGroup.startHandle + ARRAY_SIZE(gattList) - 1;
    blc_gatts_addAttributeServiceGroup(&svcGapGroup);
    blc_gatts_addAttributeServiceGroup(&svcGattGroup);
}

/**
 * @brief      for user remove default GATT and GAP service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_removeCoreGroup(void)
{
    blc_gatts_removeAttributeServiceGroup(GAP_START_HDL);
    blc_gatts_removeAttributeServiceGroup(GATT_START_HDL);
}

/**
 * @brief      for user calculate database hash value(core version >= 5.1).
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_calculateDatabaseHash(void)
{
    blc_gatts_calculateDatabaseHash(0xFFFF, databaseHashVal);
}

/**
 * @brief     for user set device name.
 * @param[in] name: user device name.
 * @return    none.
 */
void blc_svc_setDeviceName(const char *name)
{
    strncpy(defaultDevName, name, sizeof(defaultDevName) - 1);
    defaultDevNameLen = strlen(defaultDevName);
}

/**
 * @brief     for user set appearance.
 * @param[in] appearance: user appearance.
 * @return    none.
 */
void blc_svc_setAppearance(uint16_t appearance)
{
    defaultAppearance = appearance;
}

/**
 * @brief     for user set peripheral preferred parameters.
 * @param[in] minInterval: user minInterval, unit is 1.25ms
 * @param[in] maxInterval: user maxInterval, unit is 1.25ms
 * @param[in] latency: user latency, unit is connection events.
 * @param[in] timeout: user timeout, unit is 10ms.
 * @return    none.
 */
void blc_svc_setPeripheralPreferredParameters(uint16_t minInterval, uint16_t maxInterval, uint16_t latency, uint16_t timeout)
{
    defaultPeriConnParameters[0] = minInterval;
    defaultPeriConnParameters[1] = maxInterval;
    defaultPeriConnParameters[2] = latency;
    defaultPeriConnParameters[3] = timeout;
}
