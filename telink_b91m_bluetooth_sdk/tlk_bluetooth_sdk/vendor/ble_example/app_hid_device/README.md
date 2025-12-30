# BLE HID Device Example

This is an example application that demonstrates how to implement a HID device using Bluetooth Low Energy (BLE). Example default support HID keyboard and mouse devices.

Can change APP_HID_DEVICE_SELECT_MODE marco to select keyboard or mouse mode. Keyboard mode support 3 keys, send cap lock, volume increase and decrease. Mouse mode support 4 keys, send left click, x move 10, x move -10 and wheel 1.

Double click key 1, reconnect first connection device using Directed Advertising.

Double click key 2, reconnect second connection device using Directed Advertising.

## Configuration Pnp(Vendor ID)

1. using marco configuration.

DIS_PNP_ID_VENDOR_ID_SOURCE to configures the source of Vendor ID, 0x01 mean using Bluetooth SIG assigned company ID, 0x02 mean using USB-IF assigned vendor ID.

DIS_PNP_ID_VENDOR_ID to configures the Vendor ID.

DIS_PNP_ID_PRODUCT_ID to configures the Product ID.

DIS_PNP_ID_PRODUCT_VERSION to configures the Product Version.

2. call api to change Vendor ID, Product ID and Product Version.

```c
/**
 * @brief      for user update PNP ID.
 * @param[in]  vid: vendor ID.
 * @param[in]  pid: product ID.
 * @param[in]  ver: version.
 * @return     none.
 * @note Default using Bluetooth SIG assigned Company ID.
 */
void blc_svc_dis_update_pnp_id(uint16_t vid, uint16_t pid, uint16_t ver);
```

## Configuration HID map

HID_INPUT_REPORT_NUM to configures the number of input report.

HID_OUTPUT_REPORT_NUM to configures the number of output report.

HID_FEATURE_REPORT_NUM to configures the number of feature report.

HID_INPUT_REPORT_1_ID to configures the input report ID.

HID_OUTPUT_REPORT_1_ID to configures the output report ID.

call api to register HID map.

```c
/**
 *   @brief Register the Human Interface Device server control.
 *
 *   @param[in] param Pointer to the structure containing the initial values for the service.
 *
 *   @note This function should be called before any other function in the HIDSS module.
 *
 *   @return None.
 */
void ble_hid_register_HID_control_server(const struct ble_hids_register_param *param);
```

## Receive HID output report

```c
/**
 *   @brief Set the event callback for the Human Interface Device server control.
 *
 *   @param[in] callback Pointer to the callback function.
 *
 *   @return None.
 *
 */
void ble_hid_register_HID_server_event_callback(ble_hids_event_callback callback);
```

## Send HID input report

```c
/**
 *   @brief Notify the input report.
 *
 *   @param[in] conn_handle Connection handle.
 *   @param[in] report_id Input Report ID.
 *   @param[in] value Pointer to the report data.
 *   @param[in] value_len Length of the report data.
 *
 *   @return BLE_HOST_ERR_SUCC if notify input report successfully, otherwise return error code.
 */
int ble_hids_notify_input_report(uint16_t conn_handle, uint8_t report_id, const uint8_t *value, uint16_t value_len);
```

## How to Support Boot Keyboard or mouse

If want support Boot Keyboard or mouse, need to call hid_km module api.
