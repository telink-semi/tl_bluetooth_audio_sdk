/********************************************************************************************************
 * @file    svc_hid_km.c
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

#include "../svc_hid.h"

#include "hid_def.h"

#include "vendor/common/user_config.h"

#define HID_START_HDL SERVICE_HUMAN_INTERFACE_DEVICE_HDL

#ifdef HID_KM_SUPP_BOOT_PROTOCOL_MODE
//Protocol Mode
static const uint8_t  hidProtocolModeValue    = BLE_HOST_HID_PROTOCOL_MODE_REPORT;
static const uint16_t hidProtocolModeValueLen = sizeof(hidProtocolModeValue);
#endif

#ifdef HID_KM_SUPP_BOOT_KEYBOARD_INPUT
// Boot Keyboard Input Report
static const uint8_t  hidBootKeyInReportValue[]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const uint16_t hidBootKeyInReportValueLen = sizeof(hidBootKeyInReportValue);
#endif

#ifdef HID_KM_SUPP_BOOT_KEYBOARD_OUTPUT
// Boot Keyboard Output Report
static const uint16_t hidBootKeyOutReportValue    = 0x0003;
static const uint16_t hidBootKeyOutReportValueLen = sizeof(hidBootKeyOutReportValue);
#endif

#ifdef HID_KM_SUPP_BOOT_MOUSE_INPUT
// Boot Mouse Input Report
static const uint8_t  hidBootMouseInReportValue[]  = {0x00, 0x00, 0x00};
static const uint16_t hidBootMouseInReportValueLen = sizeof(hidBootMouseInReportValue);
#endif

static const uint8_t  hidReportMap[] = {0x00};
static const uint16_t hidReportMapLen;
static const uint8_t  hidReportMapVale[]  = {0x00, 0x00};
static const uint16_t hidReportMapValeLen = sizeof(hidReportMapVale);

// default HID information, bcd:0x0111, country code:0x00, flags: remote wake(true), normally connectable(false)
static const uint8_t  hidInformationValue[]  = {0x11, 0x01, 0x00, 0x01};
static const uint16_t hidInformationValueLen = sizeof(hidInformationValue);

#if HID_KM_INPUT_REPORT_NUM > 0
static const uint8_t  hidReportInput1Value[]     = {0x00};
static const uint16_t hidReportInput1ValueLen    = sizeof(hidReportInput1Value);
static const uint8_t  hidReportInput1RefValue[2] = {HID_KM_INPUT_REPORT_1_ID, BLE_HOST_HID_REPORT_TYPE_INPUT};
static const uint16_t hidReportInput1RefValueLen = sizeof(hidReportInput1RefValue);
#endif

#if HID_KM_INPUT_REPORT_NUM > 1
static const uint8_t  hidReportInput2Value[]     = {0x00};
static const uint16_t hidReportInput2ValueLen    = sizeof(hidReportInput2Value);
static const uint8_t  hidReportInput2RefValue[2] = {HID_KM_INPUT_REPORT_2_ID, BLE_HOST_HID_REPORT_TYPE_INPUT};
static const uint16_t hidReportInput2RefValueLen = sizeof(hidReportInput2RefValue);
#endif

#if HID_KM_INPUT_REPORT_NUM > 2
static const uint8_t  hidReportInput3Value[]     = {0x00};
static const uint16_t hidReportInput3ValueLen    = sizeof(hidReportInput3Value);
static const uint8_t  hidReportInput3RefValue[2] = {HID_KM_INPUT_REPORT_3_ID, BLE_HOST_HID_REPORT_TYPE_INPUT};
static const uint16_t hidReportInput3RefValueLen = sizeof(hidReportInput3RefValue);
#endif

#if HID_KM_OUTPUT_REPORT_NUM > 0
static const uint8_t  hidReportOutput1Value[]     = {0x00};
static const uint16_t hidReportOutput1ValueLen    = sizeof(hidReportOutput1Value);
static const uint8_t  hidReportOutput1RefValue[2] = {HID_KM_OUTPUT_REPORT_1_ID, BLE_HOST_HID_REPORT_TYPE_OUTPUT};
static const uint16_t hidReportOutput1RefValueLen = sizeof(hidReportOutput1RefValue);
#endif

#if HID_KM_OUTPUT_REPORT_NUM > 1
static const uint8_t  hidReportOutput2Value[]     = {0x00};
static const uint16_t hidReportOutput2ValueLen    = sizeof(hidReportOutput2Value);
static const uint8_t  hidReportOutput2RefValue[2] = {HID_KM_OUTPUT_REPORT_2_ID, BLE_HOST_HID_REPORT_TYPE_OUTPUT};
static const uint16_t hidReportOutput2RefValueLen = sizeof(hidReportOutput2RefValue);
#endif

#if HID_KM_FEATURE_REPORT_NUM
static const uint8_t  hidReportFeatureValue[]     = {0x00};
static const uint16_t hidReportFeatureValueLen    = sizeof(hidReportFeatureValue);
static const uint8_t  hidReportFeatureRefValue[2] = {HID_KM_FEATURE_REPORT_1_ID, BLE_HOST_HID_REPORT_TYPE_FEATURE};
static const uint16_t hidReportFeatureRefValueLen = sizeof(hidReportFeatureRefValue);
#endif

#define HID_MAP_DESCRIPTOR_REFERENCE(value) \
    ATTS_ATTRIBUTE_INIT_PARAM(ATT_PERMISSIONS_READ, descriptorExternalReportReferenceAttUuid, ATTS_SET_NONE, sizeof(value), &value##Len, value)

#define HID_DESCRIPTOR_REFERENCE(value) ATTS_ATTRIBUTE_INIT_PARAM(ATT_PERMISSIONS_READ, descriptorReportReferenceAttUuid, ATTS_SET_NONE, sizeof(value), &value##Len, value)

extern const uint16_t basIncludeVal[3];

/*
 * @brief the structure for default HID service List.
 */
static const struct atts_attribute hidList[] = {
    ATTS_PRIMARY_SERVICE(serviceHumanInterfaceDeviceAttUuid),

    //include BAS
    ATTS_INCLUDE_DEFINE(&basIncludeVal[0]),

#ifdef HID_KM_SUPP_BOOT_PROTOCOL_MODE
    //protocol mode
    ATTS_CHAR_UUID_RDWR_ENTITY_RWCB(charPropReadWriteWithout, characteristicProtocolModeAttUuid, hidProtocolModeValue),
#endif

#ifdef HID_KM_SUPP_BOOT_KEYBOARD_INPUT
    //boot keyboard input report
    ATTS_CHAR_UUID_RDWR_ENTITY_RWCB(charPropReadWriteNotify, characteristicBootKeyboardInputReportAttUuid, hidBootKeyInReportValue),
    ATTS_COMMON_CCC_DEFINE,
#endif

#ifdef HID_KM_SUPP_BOOT_KEYBOARD_OUTPUT
    //boot keyboard output report
    ATTS_CHAR_UUID_RDWR_ENTITY_RWCB(charPropReadWriteWriteWithout, characteristicBootKeyboardOutputReportAttUuid, hidBootKeyOutReportValue),
#endif

#ifdef HID_KM_SUPP_BOOT_MOUSE_INPUT
    //boot mouse input report
    ATTS_CHAR_UUID_RDWR_ENTITY_RWCB(charPropReadWriteNotify, characteristicBootMouseInputReportAttUuid, hidBootMouseInReportValue),
    ATTS_COMMON_CCC_DEFINE,
#endif

    //hid map
    ATTS_CHAR_UUID_ENCR_READ_POINT_CB(charPropRead, characteristicReportMapAttUuid, hidReportMap),
    HID_MAP_DESCRIPTOR_REFERENCE(hidReportMapVale),

    //hid information
    ATTS_CHAR_UUID_READ_ENTITY_NOCB(charPropRead, characteristicHidInformationAttUuid, hidInformationValue),

    //hid control point
    ATTS_CHAR_UUID_WRITE_NULL(charPropWriteWithout, characteristicHidControlPointAttUuid),

#if HID_KM_INPUT_REPORT_NUM > 0
    //report(input)
    ATTS_CHAR_UUID_RDWR_ENTITY_WCB(charPropReadWriteNotify, characteristicReportAttUuid, hidReportInput1Value),
    ATTS_COMMON_CCC_DEFINE,
    HID_DESCRIPTOR_REFERENCE(hidReportInput1RefValue),
#endif

#if HID_KM_INPUT_REPORT_NUM > 1
    //report(input)
    ATTS_CHAR_UUID_RDWR_ENTITY_WCB(charPropReadWriteNotify, characteristicReportAttUuid, hidReportInput2Value),
    ATTS_COMMON_CCC_DEFINE,
    HID_DESCRIPTOR_REFERENCE(hidReportInput2RefValue),
#endif

#if HID_KM_INPUT_REPORT_NUM > 2
    //report(input)
    ATTS_CHAR_UUID_RDWR_ENTITY_WCB(charPropReadWriteNotify, characteristicReportAttUuid, hidReportInput3Value),
    ATTS_COMMON_CCC_DEFINE,
    HID_DESCRIPTOR_REFERENCE(hidReportInput3RefValue),
#endif

#if HID_KM_OUTPUT_REPORT_NUM > 0
    //report(output)
    ATTS_CHAR_UUID_RDWR_ENTITY_WCB(charPropReadWriteWriteWithout, characteristicReportAttUuid, hidReportOutput1Value),
    HID_DESCRIPTOR_REFERENCE(hidReportOutput1RefValue),
#endif

#if HID_KM_OUTPUT_REPORT_NUM > 1
    //report(output)
    ATTS_CHAR_UUID_RDWR_ENTITY_WCB(charPropReadWriteWriteWithout, characteristicReportAttUuid, hidReportOutput2Value),
    HID_DESCRIPTOR_REFERENCE(hidReportOutput2RefValue),
#endif

#if HID_KM_FEATURE_REPORT_NUM
    //report(feature)
    ATTS_CHAR_UUID_RDWR_ENTITY_WCB(charPropReadWrite, characteristicReportAttUuid, hidReportFeatureValue),
    HID_DESCRIPTOR_REFERENCE(hidReportFeatureRefValue),
#endif

};

/*
 * @brief the structure for default HID service group.
 */
_attribute_ble_data_retention_ static struct atts_group svcHidGroup = {
    NULL, hidList, NULL, NULL, HID_START_HDL, 0,
};

/**
 * @brief      for user add default HID keyboard or mouse service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_addHidKmGroup(void)
{
    svcHidGroup.endHandle = svcHidGroup.startHandle + ARRAY_SIZE(hidList) - 1;
    blc_gatts_addAttributeServiceGroup(&svcHidGroup);
}

/**
 * @brief      for user remove default HID keyboard or mouse service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_removeHidKmGroup(void)
{
    blc_gatts_removeAttributeServiceGroup(HID_START_HDL);
}

/**
 * @brief      for user register read or write attribute value callback function in HID keyboard or mouse service.
 * @param[in]  readCback: read attribute value callback function pointer.
 * @param[in]  writeCback: write attribute value callback function pointer.
 * @return     none.
 */
void blc_svc_hidKmCbackRegister(atts_r_cb_t readCback, atts_w_cb_t writeCback)
{
    svcHidGroup.readCallback  = readCback;
    svcHidGroup.writeCallback = writeCback;
}

#if HID_KM_INPUT_REPORT_NUM > 3
#error "ERR: Maximum Input Report Type Count is 3"
#endif

#if HID_KM_OUTPUT_REPORT_NUM > 2
#error "ERR: Maximum Output Report Type Count is 2"
#endif

#if HID_KM_FEATURE_REPORT_NUM > 1
#error "ERR: Maximum Feature Report Type Count is 1"
#endif

#if (defined(HID_KM_SUPP_BOOT_PROTOCOL_MODE) == 0) && \
    (defined(HID_KM_SUPP_BOOT_KEYBOARD_INPUT) || defined(HID_KM_SUPP_BOOT_KEYBOARD_OUTPUT) || defined(HID_KM_SUPP_BOOT_MOUSE_INPUT))
#error "ERR: if want support Boot Mode, must enable Boot Protocol Mode"
#endif
