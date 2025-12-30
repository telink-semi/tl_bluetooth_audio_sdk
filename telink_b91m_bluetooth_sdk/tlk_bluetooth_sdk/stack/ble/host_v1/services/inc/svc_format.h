/********************************************************************************************************
 * @file    svc_format.h
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
#pragma once

#define CHARACTERISTIC_PROPERTIES_LENGTH 1

extern const uint16_t attr16UuidLen;
extern const uint16_t attr128UuidLen;
extern const uint16_t gattIncludeValueLen;

extern const uint16_t characteristicPropertiesLen;

extern const uint8_t charPropRead;
extern const uint8_t charPropReadWrite;
extern const uint8_t charPropReadWriteWithout;
extern const uint8_t charPropReadWriteWriteWithout;
extern const uint8_t charPropReadWriteWriteWithoutNotify;
extern const uint8_t charPropReadWriteNotify;
extern const uint8_t charPropReadWriteWithoutNotify;
extern const uint8_t charPropReadNotify;
extern const uint8_t charPropWrite;
extern const uint8_t charPropWriteWithout;
extern const uint8_t charPropWriteWriteWithout;
extern const uint8_t charPropWriteIndicate;
extern const uint8_t charPropWriteNotifyIndicate;
extern const uint8_t charPropWriteWriteWithoutNotify;
extern const uint8_t charPropNotify;
extern const uint8_t charPropIndicate;

extern const uint8_t  clientCharacteristicConfiguration[2];
extern const uint16_t clientCharacteristicConfigurationLen;

extern uint8_t  clientCharacteristicConfigurationWrite[2];
extern uint16_t clientCharacteristicConfigurationWriteLen;


#define ATTS_CHARACTERISTIC_DECLARATIONS(properties)     \
    {ATT_PERMISSIONS_READ,                               \
     ATT_16_UUID_LEN,                                    \
     (uint8_t *)(size_t)declarationsCharacteristicUuid,  \
     (uint16_t *)(size_t) & characteristicPropertiesLen, \
     CHARACTERISTIC_PROPERTIES_LENGTH,                   \
     (uint8_t *)(size_t) & properties,                   \
     ATTS_SET_ATTR_VALUE_PROPERTIES}

#define SERVICE_CHAR_DEFINE(properties, handle, uuid)        {properties, uint16_t_TO_BYTES(handle), uint16_t_TO_BYTES(uuid)}
#define SERVICE_CHAR_READ(handle, uuid)                      SERVICE_CHAR_DEFINE(CHAR_PROP_READ, handle, uuid)
#define SERVICE_CHAR_READ_WRITE(handle, uuid)                SERVICE_CHAR_DEFINE(CHAR_PROP_READ | CHAR_PROP_WRITE, handle, uuid)

#define SERVICE_CHAR_WRITE(handle, uuid)                     SERVICE_CHAR_DEFINE(CHAR_PROP_WRITE, handle, uuid)
#define SERVICE_CHAR_WRITE_WRITEWITHOUT(handle, uuid)        SERVICE_CHAR_DEFINE(CHAR_PROP_WRITE_WITHOUT_RSP | CHAR_PROP_WRITE, handle, uuid)
#define SERVICE_CHAR_NOTIFY(handle, uuid)                    SERVICE_CHAR_DEFINE(CHAR_PROP_NOTIFY, handle, uuid)
#define SERVICE_CHAR_READ_NOTIFY(handle, uuid)               SERVICE_CHAR_DEFINE(CHAR_PROP_READ | CHAR_PROP_NOTIFY, handle, uuid)
#define SERVICE_CHAR_INDICATE(handle, uuid)                  SERVICE_CHAR_DEFINE(CHAR_PROP_INDICATE, handle, uuid)
#define SERVICE_CHAR_WRITE_INDICATE(handle, uuid)            SERVICE_CHAR_DEFINE(CHAR_PROP_WRITE | CHAR_PROP_INDICATE, handle, uuid)
#define SERVICE_CHAR_WRITE_INDICATE_NOTIFY(handle, uuid)     SERVICE_CHAR_DEFINE(CHAR_PROP_WRITE | CHAR_PROP_NOTIFY | CHAR_PROP_INDICATE, handle, uuid)

#define SERVICE_CHAR_READ_WRITE_WRITEWITHOUT(handle, uuid)   SERVICE_CHAR_DEFINE(CHAR_PROP_READ | CHAR_PROP_WRITE | CHAR_PROP_WRITE_WITHOUT_RSP, handle, uuid)
#define SERVICE_CHAR_WRITE_NOTIFY_INDICATE(handle, uuid)     SERVICE_CHAR_DEFINE(CHAR_PROP_NOTIFY | CHAR_PROP_INDICATE, handle, uuid)
#define SERVICE_CHAR_READ_WRITE_NOTIFY(handle, uuid)         SERVICE_CHAR_DEFINE(CHAR_PROP_READ | CHAR_PROP_WRITE | CHAR_PROP_NOTIFY, handle, uuid)
#define SERVICE_CHAR_READ_WRITEWITHOUT_NOTIFY(handle, uuid)  SERVICE_CHAR_DEFINE(CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RSP | CHAR_PROP_NOTIFY, handle, uuid)
#define SERVICE_CHAR_WRITE_WRITEWITHOUT_NOTIFY(handle, uuid) SERVICE_CHAR_DEFINE(CHAR_PROP_WRITE_WITHOUT_RSP | CHAR_PROP_WRITE | CHAR_PROP_NOTIFY, handle, uuid)
#define SERVICE_CHAR_READ_WRITE_WRITEWITHOUT_NOTIFY(handle, uuid) \
    SERVICE_CHAR_DEFINE(CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RSP | CHAR_PROP_WRITE | CHAR_PROP_NOTIFY, handle, uuid)

//{ATT_PERMISSIONS_READ, ATT_16_UUID_LEN, (uint8_t*)declarationsPrimaryServiceUuid,(uint16_t*)&attr16UuidLen,ATT_16_UUID_LEN,(uint8_t*)serviceUuid,0}
#define ATTS_PRIMARY_SERVICE(serviceUuid)               \
    {ATT_PERMISSIONS_READ,                              \
     ATT_16_UUID_LEN,                                   \
     (uint8_t *)(size_t)declarationsPrimaryServiceUuid, \
     (uint16_t *)(size_t) & attr16UuidLen,              \
     ATT_16_UUID_LEN,                                   \
     (uint8_t *)(size_t)serviceUuid,                    \
     0}
//{ATT_PERMISSIONS_READ, ATT_16_UUID_LEN, (uint8_t*)declarationsSecondaryServiceUuid,(uint16_t*)&attr16UuidLen,ATT_16_UUID_LEN,(uint8_t*)serviceUuid,0}
#define ATTS_SECONDARY_SERVICE(serviceUuid)               \
    {ATT_PERMISSIONS_READ,                                \
     ATT_16_UUID_LEN,                                     \
     (uint8_t *)(size_t)declarationsSecondaryServiceUuid, \
     (uint16_t *)(size_t) & attr16UuidLen,                \
     ATT_16_UUID_LEN,                                     \
     (uint8_t *)(size_t)serviceUuid,                      \
     0}

//{ATT_PERMISSIONS_READ, ATT_16_UUID_LEN, (uint8_t*)declarationsPrimaryServiceUuid,(uint16_t*)&attr128UuidLen,ATT_128_UUID_LEN,(uint8_t*)serviceUuid,0}
#define ATTS_PRIMARY_SERVICE_128(serviceUuid)           \
    {ATT_PERMISSIONS_READ,                              \
     ATT_16_UUID_LEN,                                   \
     (uint8_t *)(size_t)declarationsPrimaryServiceUuid, \
     (uint16_t *)(size_t) & attr128UuidLen,             \
     ATT_128_UUID_LEN,                                  \
     (uint8_t *)(size_t)serviceUuid,                    \
     0}
//{ATT_PERMISSIONS_READ, ATT_16_UUID_LEN, (uint8_t*)declarationsSecondaryServiceUuid,(uint16_t*)&attr128UuidLen,ATT_128_UUID_LEN,(uint8_t*)serviceUuid,0}
#define ATTS_SECONDARY_SERVICE_128(serviceUuid)           \
    {ATT_PERMISSIONS_READ,                                \
     ATT_16_UUID_LEN,                                     \
     (uint8_t *)(size_t)declarationsSecondaryServiceUuid, \
     (uint16_t *)(size_t) & attr128UuidLen,               \
     ATT_128_UUID_LEN,                                    \
     (uint8_t *)(size_t)serviceUuid,                      \
     0}


//{ATT_PERMISSIONS_RDWR,ATT_16_UUID_LEN,  (uint8_t*) descriptorClientCharacteristicConfigurationUuid,   (uint16_t*) &pacsSinkPACCCCLen, sizeof(pacsSinkPACCCC), (uint8_t*)pacsSinkPACCCC,0}
#define ATTS_CCC_DEFINE_COMMON(ccc, cccLen)                              \
    {ATT_PERMISSIONS_RDWR,                                               \
     ATT_16_UUID_LEN,                                                    \
     (uint8_t *)(size_t)descriptorClientCharacteristicConfigurationUuid, \
     (uint16_t *)(size_t) & cccLen,                                      \
     sizeof(ccc),                                                        \
     (uint8_t *)(size_t)ccc,                                             \
     0}
#define ATTS_CCC_DEFINE(ccc)   ATTS_CCC_DEFINE_COMMON(ccc, ccc##Len)
#define ATTS_COMMON_CCC_DEFINE ATTS_CCC_DEFINE(clientCharacteristicConfiguration)
#define ATTS_CCC_DEFINE_WRITE                                            \
    {ATT_PERMISSIONS_RDWR,                                               \
     ATT_16_UUID_LEN,                                                    \
     (uint8_t *)(size_t)descriptorClientCharacteristicConfigurationUuid, \
     (uint16_t *)(size_t) & clientCharacteristicConfigurationWriteLen,   \
     sizeof(clientCharacteristicConfigurationWrite),                     \
     (uint8_t *)(size_t)clientCharacteristicConfigurationWrite,          \
     ATTS_SET_ALLOW_WRITE}

#define ATTS_COMMON_CCC_DEFINE_CB                                \
    {ATT_PERMISSIONS_RDWR,                                       \
     ATT_16_UUID_LEN,                                            \
     (uint8_t *)descriptorClientCharacteristicConfigurationUuid, \
     (uint16_t *)NULL,                                           \
     0,                                                          \
     (uint8_t *)NULL,                                            \
     ATTS_SET_WRITE_CALLBACK | ATTS_SET_READ_CALLBACK}
#define ATTS_COMMON_CCC_DEFINE_WCB                                \
    {ATT_PERMISSIONS_RDWR,                                        \
     ATT_16_UUID_LEN,                                             \
     (uint8_t *)descriptorClientCharacteristicConfigurationUuid,  \
     (uint16_t *)(size_t) & clientCharacteristicConfigurationLen, \
     sizeof(clientCharacteristicConfiguration),                   \
     (uint8_t *)(size_t) & clientCharacteristicConfiguration,     \
     ATTS_SET_WRITE_CALLBACK}


#define ATTS_INCLUDE_DEFINE(value) \
    {ATT_PERMISSIONS_READ, ATT_16_UUID_LEN, (uint8_t *)(size_t)declarationsIncludeUuid, (uint16_t *)(size_t) & gattIncludeValueLen, 6, (uint8_t *)(size_t)value, 0}

#define ATTS_CHAR_UUID_ENCR_WRITE_NULL(properties, uuid)                                                                \
    ATTS_CHARACTERISTIC_DECLARATIONS(properties),                                                                       \
    {                                                                                                                   \
        ATT_PERMISSIONS_ENCRYPT_WRITE, ATT_16_UUID_LEN, (uint8_t *)(size_t)uuid, NULL, 0, NULL, ATTS_SET_WRITE_CALLBACK \
    }

#define ATTS_CHAR_UUID_WRITE_NULL(properties, uuid)                                                             \
    ATTS_CHARACTERISTIC_DECLARATIONS(properties),                                                               \
    {                                                                                                           \
        ATT_PERMISSIONS_WRITE, ATT_16_UUID_LEN, (uint8_t *)(size_t)uuid, NULL, 0, NULL, ATTS_SET_WRITE_CALLBACK \
    }


#define ATTS_CHAR_UUID_NOTIF_ONLY(uuid)                               \
    ATTS_CHARACTERISTIC_DECLARATIONS(charPropNotify),                 \
    {                                                                 \
        0, ATT_16_UUID_LEN, (uint8_t *)(size_t)uuid, NULL, 0, NULL, 0 \
    }

#define ATTS_CHAR_UUID_INDICATE_ONLY(uuid) ATTS_CHAR_UUID_NOTIF_ONLY(uuid)
#define ATTS_CHAR_UUID_DEFINE(perm, uuid, valueLen, maxValueLen, value, settings) \
    {perm, ATT_16_UUID_LEN, (uint8_t *)(size_t)uuid, (uint16_t *)(size_t) & valueLen, maxValueLen, (uint8_t *)(size_t)value, settings}


#define ATTS_CHAR_UUID_DEFINE_VALUE_POINTER(properties, perm, uuid, maxValueLen, value, settings) \
    ATTS_CHARACTERISTIC_DECLARATIONS(properties), ATTS_CHAR_UUID_DEFINE(perm, uuid, value##Len, maxValueLen, value, settings)

#define ATTS_CHAR_UUID_DEFINE_VALUE_ENTITY(properties, perm, uuid, value, settings) \
    ATTS_CHARACTERISTIC_DECLARATIONS(properties), ATTS_CHAR_UUID_DEFINE(perm, uuid, value##Len, sizeof(value), &value, settings)


#define ATTS_CHAR_UUID_ENCR_READ_POINTER(properties, uuid, value, settings) ATTS_CHAR_UUID_DEFINE_VALUE_POINTER(properties, ATT_PERMISSIONS_ENCRYPT_READ, uuid, 0, value, settings)
#define ATTS_CHAR_UUID_ENCR_WRITE_POINT(properties, uuid, value, settings) \
    ATTS_CHAR_UUID_DEFINE_VALUE_POINTER(properties, ATT_PERMISSIONS_ENCRYPT_WRITE, uuid, sizeof(value), value, settings)
#define ATTS_CHAR_UUID_ENCR_RDWR_POINT(properties, uuid, value, settings) \
    ATTS_CHAR_UUID_DEFINE_VALUE_POINTER(properties, ATT_PERMISSIONS_ENCRYPT_RDWR, uuid, sizeof(value), value, settings)
#define ATTS_CHAR_UUID_ENCR_READ_POINT_NOCB(properties, uuid, value)            ATTS_CHAR_UUID_ENCR_READ_POINTER(properties, uuid, value, 0)
#define ATTS_CHAR_UUID_ENCR_WRITE_POINT_NOCB(properties, uuid, value)           ATTS_CHAR_UUID_ENCR_WRITE_POINT(properties, uuid, value, 0)
#define ATTS_CHAR_UUID_ENCR_RDWR_POINT_NOCB(properties, uuid, value)            ATTS_CHAR_UUID_ENCR_RDWR_POINT(properties, uuid, value, 0)
#define ATTS_CHAR_UUID_ENCR_READ_POINT_CB(properties, uuid, value)              ATTS_CHAR_UUID_ENCR_READ_POINTER(properties, uuid, value, ATTS_SET_READ_CALLBACK)
#define ATTS_CHAR_UUID_ENCR_WRITE_POINT_CB(properties, uuid, value)             ATTS_CHAR_UUID_ENCR_WRITE_POINT(properties, uuid, value, ATTS_SET_WRITE_CALLBACK)
#define ATTS_CHAR_UUID_ENCR_RDWR_POINT_RCB(properties, uuid, value)             ATTS_CHAR_UUID_ENCR_RDWR_POINT(properties, uuid, value, ATTS_SET_READ_CALLBACK)
#define ATTS_CHAR_UUID_ENCR_RDWR_POINT_WCB(properties, uuid, value)             ATTS_CHAR_UUID_ENCR_RDWR_POINT(properties, uuid, value, ATTS_SET_WRITE_CALLBACK)
#define ATTS_CHAR_UUID_ENCR_RDWR_POINT_RWCB(properties, uuid, value)            ATTS_CHAR_UUID_ENCR_RDWR_POINT(properties, uuid, value, ATTS_SET_WRITE_CALLBACK | ATTS_SET_READ_CALLBACK)

#define ATTS_CHAR_UUID_ENCR_READ_ENTITY(properties, uuid, value, settings)      ATTS_CHAR_UUID_DEFINE_VALUE_ENTITY(properties, ATT_PERMISSIONS_ENCRYPT_READ, uuid, value, settings)
#define ATTS_CHAR_UUID_ENCR_WRITE_ENTITY(properties, uuid, value, settings)     ATTS_CHAR_UUID_DEFINE_VALUE_ENTITY(properties, ATT_PERMISSIONS_ENCRYPT_WRITE, uuid, value, settings)
#define ATTS_CHAR_UUID_ENCR_RDWR_ENTITY(properties, uuid, value, settings)      ATTS_CHAR_UUID_DEFINE_VALUE_ENTITY(properties, ATT_PERMISSIONS_ENCRYPT_RDWR, uuid, value, settings)
#define ATTS_CHAR_UUID_ENCR_READ_ENTITY_NOCB(properties, uuid, value)           ATTS_CHAR_UUID_ENCR_READ_ENTITY(properties, uuid, value, 0)
#define ATTS_CHAR_UUID_ENCR_WRITE_ENTITY_NOCB(properties, uuid, value)          ATTS_CHAR_UUID_ENCR_WRITE_ENTITY(properties, uuid, value, 0)
#define ATTS_CHAR_UUID_ENCR_RDWR_ENTITY_NOCB(properties, uuid, value)           ATTS_CHAR_UUID_ENCR_RDWR_ENTITY(properties, uuid, value, 0)
#define ATTS_CHAR_UUID_ENCR_READ_ENTITY_CB(properties, uuid, value)             ATTS_CHAR_UUID_ENCR_READ_ENTITY(properties, uuid, value, ATTS_SET_READ_CALLBACK)
#define ATTS_CHAR_UUID_ENCR_WRITE_ENTITY_CB(properties, uuid, value)            ATTS_CHAR_UUID_ENCR_WRITE_ENTITY(properties, uuid, value, ATTS_SET_WRITE_CALLBACK)
#define ATTS_CHAR_UUID_ENCR_RDWR_ENTITY_RCB(properties, uuid, value)            ATTS_CHAR_UUID_ENCR_RDWR_ENTITY(properties, uuid, value, ATTS_SET_READ_CALLBACK)
#define ATTS_CHAR_UUID_ENCR_RDWR_ENTITY_WCB(properties, uuid, value)            ATTS_CHAR_UUID_ENCR_RDWR_ENTITY(properties, uuid, value, ATTS_SET_WRITE_CALLBACK)
#define ATTS_CHAR_UUID_ENCR_RDWR_ENTITY_RWCB(properties, uuid, value)           ATTS_CHAR_UUID_ENCR_RDWR_ENTITY(properties, uuid, value, ATTS_SET_WRITE_CALLBACK | ATTS_SET_READ_CALLBACK)


#define ATTS_CHAR_UUIU_NO_READ_WRITE_POINTER(properties, uuid, value, settings) ATTS_CHAR_UUID_DEFINE_VALUE_POINTER(properties, 0, uuid, sizeof(value), value, settings)
#define ATTS_CHAR_UUID_NO_RDWR_POINT_NOCB(properties, uuid, value)              ATTS_CHAR_UUIU_NO_READ_WRITE_POINTER(properties, uuid, value, 0)
#define ATTS_CHAR_UUID_READ_POINTER(properties, uuid, value, settings)          ATTS_CHAR_UUID_DEFINE_VALUE_POINTER(properties, ATT_PERMISSIONS_READ, uuid, 0, value, settings)
#define ATTS_CHAR_UUID_WRITE_POINT(properties, uuid, value, settings)           ATTS_CHAR_UUID_DEFINE_VALUE_POINTER(properties, ATT_PERMISSIONS_WRITE, uuid, sizeof(value), value, settings)
#define ATTS_CHAR_UUID_RDWR_POINT(properties, uuid, value, settings)            ATTS_CHAR_UUID_DEFINE_VALUE_POINTER(properties, ATT_PERMISSIONS_RDWR, uuid, sizeof(value), value, settings)
#define ATTS_CHAR_UUID_READ_POINT_NOCB(properties, uuid, value)                 ATTS_CHAR_UUID_READ_POINTER(properties, uuid, value, 0)
#define ATTS_CHAR_UUID_WRITE_POINT_NOCB(properties, uuid, value)                ATTS_CHAR_UUID_WRITE_POINT(properties, uuid, value, 0)
#define ATTS_CHAR_UUID_RDWR_POINT_NOCB(properties, uuid, value)                 ATTS_CHAR_UUID_RDWR_POINT(properties, uuid, value, ATTS_SET_ALLOW_WRITE)
#define ATTS_CHAR_UUID_READ_POINT_CB(properties, uuid, value)                   ATTS_CHAR_UUID_READ_POINTER(properties, uuid, value, ATTS_SET_READ_CALLBACK)
#define ATTS_CHAR_UUID_WRITE_POINT_CB(properties, uuid, value)                  ATTS_CHAR_UUID_WRITE_POINT(properties, uuid, value, ATTS_SET_WRITE_CALLBACK)
#define ATTS_CHAR_UUID_RDWR_POINT_RCB(properties, uuid, value)                  ATTS_CHAR_UUID_RDWR_POINT(properties, uuid, value, ATTS_SET_READ_CALLBACK)
#define ATTS_CHAR_UUID_RDWR_POINT_WCB(properties, uuid, value)                  ATTS_CHAR_UUID_RDWR_POINT(properties, uuid, value, ATTS_SET_WRITE_CALLBACK)
#define ATTS_CHAR_UUID_RDWR_POINT_RWCB(properties, uuid, value)                 ATTS_CHAR_UUID_RDWR_POINT(properties, uuid, value, ATTS_SET_WRITE_CALLBACK | ATTS_SET_READ_CALLBACK)

#define ATTS_CHAR_UUID_READ_ENTITY(properties, uuid, value, settings)           ATTS_CHAR_UUID_DEFINE_VALUE_ENTITY(properties, ATT_PERMISSIONS_READ, uuid, value, settings)
#define ATTS_CHAR_UUID_WRITE_ENTITY(properties, uuid, value, settings)          ATTS_CHAR_UUID_DEFINE_VALUE_ENTITY(properties, ATT_PERMISSIONS_WRITE, uuid, value, settings)
#define ATTS_CHAR_UUID_RDWR_ENTITY(properties, uuid, value, settings)           ATTS_CHAR_UUID_DEFINE_VALUE_ENTITY(properties, ATT_PERMISSIONS_RDWR, uuid, value, settings)
#define ATTS_CHAR_UUID_READ_ENTITY_NOCB(properties, uuid, value)                ATTS_CHAR_UUID_READ_ENTITY(properties, uuid, value, 0)
#define ATTS_CHAR_UUID_WRITE_ENTITY_NOCB(properties, uuid, value)               ATTS_CHAR_UUID_WRITE_ENTITY(properties, uuid, value, 0)
#define ATTS_CHAR_UUID_RDWR_ENTITY_NOCB(properties, uuid, value)                ATTS_CHAR_UUID_RDWR_ENTITY(properties, uuid, value, 0)
#define ATTS_CHAR_UUID_READ_ENTITY_CB(properties, uuid, value)                  ATTS_CHAR_UUID_READ_ENTITY(properties, uuid, value, ATTS_SET_READ_CALLBACK)
#define ATTS_CHAR_UUID_WRITE_ENTITY_CB(properties, uuid, value)                 ATTS_CHAR_UUID_WRITE_ENTITY(properties, uuid, value, ATTS_SET_WRITE_CALLBACK)
#define ATTS_CHAR_UUID_RDWR_ENTITY_RCB(properties, uuid, value)                 ATTS_CHAR_UUID_RDWR_ENTITY(properties, uuid, value, ATTS_SET_READ_CALLBACK)
#define ATTS_CHAR_UUID_RDWR_ENTITY_WCB(properties, uuid, value)                 ATTS_CHAR_UUID_RDWR_ENTITY(properties, uuid, value, ATTS_SET_WRITE_CALLBACK)
#define ATTS_CHAR_UUID_RDWR_ENTITY_RWCB(properties, uuid, value)                ATTS_CHAR_UUID_RDWR_ENTITY(properties, uuid, value, ATTS_SET_WRITE_CALLBACK | ATTS_SET_READ_CALLBACK)
