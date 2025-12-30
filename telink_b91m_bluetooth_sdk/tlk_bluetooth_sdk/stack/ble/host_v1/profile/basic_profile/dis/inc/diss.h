/********************************************************************************************************
 * @file    diss.h
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
// DIS: Device Information Service

/**
 *   @brief Register the device information service server control.
 *
 *   @return None.
 *
 *   @note  Set marco to modify the device information values.
 *          DIS_MANUFACTURER_NAME_STRING: Manufacturer name string.
 *          DIS_MODEL_NUMBER_STRING: Model number string.
 *          DIS_SERIAL_NUMBER_STRING: Serial number string.
 *          DIS_HARDWARE_REVISION_STRING: Hardware revision string.
 *          DIS_FIRMWARE_REVISION_STRING: Firmware revision string.
 *          DIS_SOFTWARE_REVISION_STRING: Software revision string.
 *          DIS_SYSTEM_ID_MANUFACTURER: System ID manufacturer.
 *          DIS_SYSTEM_ID_OUI: System ID oui.
 *          DIS_PNP_ID_VENDOR_ID_SOURCE: PnP ID vendor ID source.
 *          DIS_PNP_ID_VENDOR_ID: PnP ID vendor.
 *          DIS_PNP_ID_PRODUCT_ID: PnP ID product.
 *          DIS_PNP_ID_PRODUCT_VERSION: PnP ID product version.
 *          DIS_IEEE_11073_20601_REGULATORY_CERTIFICATION_DATA_LIST: IEEE 11073-20601 regulatory certification data list.
 *          DIS_UDI_FOR_MEDICAL_DEVICES: UDI for medical devices.
 *    void blc_svc_dis_update_pnp_id(uint16_t vid, uint16_t pid, uint16_t ver); can be used to update the PnP ID values.
 */
void ble_basic_register_DIS_control_server(void);
