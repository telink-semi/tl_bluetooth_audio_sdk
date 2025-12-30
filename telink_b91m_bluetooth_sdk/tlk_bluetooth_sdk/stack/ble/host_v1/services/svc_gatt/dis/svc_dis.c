/********************************************************************************************************
 * @file    svc_dis.c
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

#include "svc_dis.h"

#include "vendor/common/user_config.h"

#define DIS_START_HDL SERVICE_DEVICE_INFORMATION_HDL

#ifdef DIS_MANUFACTURER_NAME_STRING
static const char     manufacturerName[]  = DIS_MANUFACTURER_NAME_STRING;
static const uint16_t manufacturerNameLen = sizeof(manufacturerName) - 1;
#endif

#ifdef DIS_MODEL_NUMBER_STRING
static const char     modelNumber[]  = DIS_MODEL_NUMBER_STRING;
static const uint16_t modelNumberLen = sizeof(modelNumber) - 1;
#endif

#ifdef DIS_SERIAL_NUMBER_STRING
static const char     serialNumber[]  = DIS_SERIAL_NUMBER_STRING;
static const uint16_t serialNumberLen = sizeof(serialNumber) - 1;
#endif

#ifdef DIS_HARDWARE_REVISION_STRING
static const char     hardwareRevision[]  = DIS_HARDWARE_REVISION_STRING;
static const uint16_t hardwareRevisionLen = sizeof(hardwareRevision) - 1;
#endif

#ifdef DIS_FIRMWARE_REVISION_STRING
static const char     firmwareRevision[]  = DIS_FIRMWARE_REVISION_STRING;
static const uint16_t firmwareRevisionLen = sizeof(firmwareRevision) - 1;
#endif

#ifdef DIS_SOFTWARE_REVISION_STRING
static const char     softwareRevision[]  = DIS_SOFTWARE_REVISION_STRING;
static const uint16_t softwareRevisionLen = sizeof(softwareRevision) - 1;
#endif

#if defined(DIS_SYSTEM_ID_MANUFACTURER) && defined(DIS_SYSTEM_ID_OUI)
// Organizationally Unique Identifier(OUI)
// uint8_t manufacturer[5]; // manufacturer-defined identifier
// uint8_t oui[3];          // Organizationally Unique Identifier(OUI)
static const uint64_t systemId    = DIS_SYSTEM_ID_MANUFACTURER | ((uint64_t)DIS_SYSTEM_ID_OUI << 40);
static const uint16_t systemIdLen = sizeof(systemId);
#endif

#ifdef DIS_IEEE_11073_20601_REGULATORY_CERTIFICATION_DATA_LIST
static const uint8_t  IEEE_DataList[]  = {DIS_IEEE_11073_20601_REGULATORY_CERTIFICATION_DATA_LIST};
static const uint16_t IEEE_DataListLen = sizeof(IEEE_DataList);
#endif

#if defined(DIS_PNP_ID_VENDOR_ID_SOURCE) && defined(DIS_PNP_ID_VENDOR_ID) && defined(DIS_PNP_ID_PRODUCT_ID) && defined(DIS_PNP_ID_PRODUCT_VERSION)
static const struct dis_pnp PnPID = {
    .vidSrc = DIS_PNP_ID_VENDOR_ID_SOURCE,
    .vid    = DIS_PNP_ID_VENDOR_ID,
    .pid    = DIS_PNP_ID_PRODUCT_ID,
    .ver    = DIS_PNP_ID_PRODUCT_VERSION,
};
#else
static struct dis_pnp PnPID = {
    .vidSrc = 0x01,
    .vid    = 0x0211,
    .pid    = 0x0002,
    .ver    = 0x0003,
};
#endif
static const uint16_t PnPIDLen = sizeof(struct dis_pnp);

#ifdef DIS_UDI_FOR_MEDICAL_DEVICES
// Unique Device Identifier(UDI) for Medical Devices,
static const uint8_t  udiForMedicalDevices[]  = {DIS_UDI_FOR_MEDICAL_DEVICES};
static const uint16_t udiForMedicalDevicesLen = sizeof(udiForMedicalDevices);
#endif

/*
 * @brief the structure for default DIS service List.
 */
static const struct atts_attribute disList[] = {
    ATTS_PRIMARY_SERVICE(serviceDeviceInformationUuid),

#ifdef DIS_MANUFACTURER_NAME_STRING
    // Manufacturer Name String
    ATTS_CHAR_UUID_READ_POINT_NOCB(charPropRead, characteristicManufacturerNameStringUuid, manufacturerName),
#endif

#ifdef DIS_MODEL_NUMBER_STRING
    // Model Number String
    ATTS_CHAR_UUID_READ_POINT_NOCB(charPropRead, characteristicModelNumberStringUuid, modelNumber),
#endif

#ifdef DIS_SERIAL_NUMBER_STRING
    // Serial Number String
    ATTS_CHAR_UUID_READ_POINT_NOCB(charPropRead, characteristicSerialNumberStringUuid, serialNumber),
#endif

#ifdef DIS_HARDWARE_REVISION_STRING
    // Hardware Revision String
    ATTS_CHAR_UUID_READ_POINT_NOCB(charPropRead, characteristicHardwareRevisionStringUuid, hardwareRevision),
#endif

#ifdef DIS_FIRMWARE_REVISION_STRING
    // Firmware Revision String
    ATTS_CHAR_UUID_READ_POINT_NOCB(charPropRead, characteristicFirmwareRevisionStringUuid, firmwareRevision),
#endif

#ifdef DIS_SOFTWARE_REVISION_STRING
    // Software Revision String
    ATTS_CHAR_UUID_READ_POINT_NOCB(charPropRead, characteristicSoftwareRevisionStringUuid, softwareRevision),
#endif

#if defined(DIS_SYSTEM_ID_MANUFACTURER) && defined(DIS_SYSTEM_ID_OUI)
    // System ID
    ATTS_CHAR_UUID_READ_ENTITY_NOCB(charPropRead, characteristicSystemIdUuid, systemId),
#endif

#ifdef DIS_IEEE_11073_20601_REGULATORY_CERTIFICATION_DATA_LIST
    // IEEE 11073-20601 Regulatory Certification Data List
    ATTS_CHAR_UUID_READ_POINT_NOCB(charPropRead, characteristicIEEE_11073_20601DataListUuid, IEEE_DataList),
#endif

    // PNP ID
    ATTS_CHAR_UUID_READ_ENTITY_NOCB(charPropRead, characteristicPnpIdUuid, PnPID),

#ifdef DIS_UDI_FOR_MEDICAL_DEVICES
    // UDI for Medical Devices
    ATTS_CHAR_UUID_READ_POINT_NOCB(charPropRead, characteristicUdiForMedicalDevicesUuid, udiForMedicalDevices),
#endif
};

/*
 * @brief the structure for default DIS service group.
 */
_attribute_ble_data_retention_ static struct atts_group svcDisGroup = {
    NULL, disList, NULL, NULL, DIS_START_HDL, 0,
};

/**
 * @brief      for user add default DIS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_addDisGroup(void)
{
    svcDisGroup.endHandle = svcDisGroup.startHandle + ARRAY_SIZE(disList) - 1;
    blc_gatts_addAttributeServiceGroup(&svcDisGroup);
}

/**
 * @brief      for user remove default DIS service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_removeDisGroup(void)
{
    blc_gatts_removeAttributeServiceGroup(DIS_START_HDL);
}

void blc_svc_dis_update_pnp_id(uint16_t vid, uint16_t pid, uint16_t ver)
{
#if defined(DIS_PNP_ID_VENDOR_ID_SOURCE) && defined(DIS_PNP_ID_VENDOR_ID) && defined(DIS_PNP_ID_PRODUCT_ID) && defined(DIS_PNP_ID_PRODUCT_VERSION)
    (void)vid;
    (void)pid;
    (void)ver;
#else
    PnPID.vid = vid;
    PnPID.pid = pid;
    PnPID.ver = ver;
#endif
}
