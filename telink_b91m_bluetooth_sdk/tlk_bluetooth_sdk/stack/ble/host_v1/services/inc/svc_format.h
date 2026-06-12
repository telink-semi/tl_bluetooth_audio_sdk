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

#define ATTS_ATTRIBUTE_INIT_PARAM(permission, att_uuid, set, max_attr_len, attr_value_len, attr_value) \
    {                                                                                                  \
        .perm         = permission,                                                                    \
        .settings     = set,                                                                           \
        .uuid         = &att_uuid,                                                                     \
        .maxAttrLen   = max_attr_len,                                                                  \
        .attrValueLen = (uint16_t *)(size_t)attr_value_len,                                            \
        .attrValue    = (uint8_t *)(size_t)attr_value,                                                 \
    }

#define ATTS_CHARACTERISTIC_DECLARATIONS(properties)                                                                                                     \
    ATTS_ATTRIBUTE_INIT_PARAM(ATT_PERMISSIONS_READ, declarationsCharacteristicAttUuid, ATTS_SET_ATTR_VALUE_PROPERTIES, CHARACTERISTIC_PROPERTIES_LENGTH, \
                              &characteristicPropertiesLen, &properties)

#define ATTS_DEFINE_SERVICE(serviceType, serviceUuid, serviceUuidLen) \
    ATTS_ATTRIBUTE_INIT_PARAM(ATT_PERMISSIONS_READ, serviceType, ATTS_SET_NONE, serviceUuidLen, &serviceUuid.uuidLength, &serviceUuid.uuid[0])


#define ATTS_PRIMARY_SERVICE(serviceUuid)       ATTS_DEFINE_SERVICE(declarationsPrimaryServiceAttUuid, serviceUuid, ATT_16_UUID_LEN)
#define ATTS_SECONDARY_SERVICE(serviceUuid)     ATTS_DEFINE_SERVICE(declarationsSecondaryServiceAttUuid, serviceUuid, ATT_16_UUID_LEN)
#define ATTS_PRIMARY_SERVICE_128(serviceUuid)   ATTS_DEFINE_SERVICE(declarationsPrimaryServiceAttUuid, serviceUuid, ATT_128_UUID_LEN)
#define ATTS_SECONDARY_SERVICE_128(serviceUuid) ATTS_DEFINE_SERVICE(declarationsSecondaryServiceAttUuid, serviceUuid, ATT_128_UUID_LEN)


#define ATTS_CCC_DEFINE_COMMON(ccc, cccLen)     ATTS_ATTRIBUTE_INIT_PARAM(ATT_PERMISSIONS_RDWR, descriptorClientCharacteristicConfigurationAttUuid, ATTS_SET_NONE, 2, &cccLen, ccc)

#define ATTS_CCC_DEFINE(ccc)                    ATTS_CCC_DEFINE_COMMON(ccc, ccc##Len)
#define ATTS_COMMON_CCC_DEFINE                  ATTS_CCC_DEFINE(clientCharacteristicConfiguration)
#define ATTS_CCC_DEFINE_WRITE                                                                                                                                                \
    ATTS_ATTRIBUTE_INIT_PARAM(ATT_PERMISSIONS_RDWR, descriptorClientCharacteristicConfigurationAttUuid, ATTS_SET_ALLOW_WRITE, 2, &clientCharacteristicConfigurationWriteLen, \
                              clientCharacteristicConfigurationWrite)

#define ATTS_COMMON_CCC_DEFINE_CB \
    ATTS_ATTRIBUTE_INIT_PARAM(ATT_PERMISSIONS_RDWR, descriptorClientCharacteristicConfigurationAttUuid, ATTS_SET_WRITE_CALLBACK | ATTS_SET_READ_CALLBACK, 2, NULL, NULL)

#define ATTS_COMMON_CCC_DEFINE_WCB                                                                                                                                         \
    ATTS_ATTRIBUTE_INIT_PARAM(ATT_PERMISSIONS_RDWR, descriptorClientCharacteristicConfigurationAttUuid, ATTS_SET_WRITE_CALLBACK, 2, &clientCharacteristicConfigurationLen, \
                              &clientCharacteristicConfiguration)

#define ATTS_INCLUDE_DEFINE(value) ATTS_ATTRIBUTE_INIT_PARAM(ATT_PERMISSIONS_READ, declarationsIncludeAttUuid, ATTS_SET_NONE, 6, &gattIncludeValueLen, value)

#define ATTS_CHAR_UUID_ENCR_WRITE_NULL(properties, uuid) \
    ATTS_CHARACTERISTIC_DECLARATIONS(properties), ATTS_ATTRIBUTE_INIT_PARAM(ATT_PERMISSIONS_ENCRYPT_WRITE, uuid, ATTS_SET_WRITE_CALLBACK, 0, NULL, NULL)

#define ATTS_CHAR_UUID_WRITE_NULL(properties, uuid) \
    ATTS_CHARACTERISTIC_DECLARATIONS(properties), ATTS_ATTRIBUTE_INIT_PARAM(ATT_PERMISSIONS_WRITE, uuid, ATTS_SET_WRITE_CALLBACK, 0, NULL, NULL)

#define ATTS_CHAR_UUID_READ_NULL(properties, uuid) \
    ATTS_CHARACTERISTIC_DECLARATIONS(properties), ATTS_ATTRIBUTE_INIT_PARAM(ATT_PERMISSIONS_READ, uuid, ATTS_SET_READ_CALLBACK, 0, NULL, NULL)

#define ATTS_CHAR_UUID_ENCR_READ_NULL(properties, uuid) \
    ATTS_CHARACTERISTIC_DECLARATIONS(properties), ATTS_ATTRIBUTE_INIT_PARAM(ATT_PERMISSIONS_ENCRYPT_READ, uuid, ATTS_SET_READ_CALLBACK, 0, NULL, NULL)


#define ATTS_CHAR_UUID_ENCR_RDWR_NULL(properties, uuid) \
    ATTS_CHARACTERISTIC_DECLARATIONS(properties), ATTS_ATTRIBUTE_INIT_PARAM(ATT_PERMISSIONS_ENCRYPT_RDWR, uuid, ATTS_SET_READ_CALLBACK | ATTS_SET_WRITE_CALLBACK, 0, NULL, NULL)

#define ATTS_CHAR_UUID_NOTIF_ONLY(uuid)                                           ATTS_CHARACTERISTIC_DECLARATIONS(charPropNotify), ATTS_ATTRIBUTE_INIT_PARAM(ATT_PERMISSIONS_NONE, uuid, ATTS_SET_NONE, 0, NULL, NULL)

#define ATTS_CHAR_UUID_INDICATE_ONLY(uuid)                                        ATTS_CHAR_UUID_NOTIF_ONLY(uuid)
#define ATTS_CHAR_UUID_DEFINE(perm, uuid, valueLen, maxValueLen, value, settings) ATTS_ATTRIBUTE_INIT_PARAM(perm, uuid, settings, maxValueLen, &valueLen, value)

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
