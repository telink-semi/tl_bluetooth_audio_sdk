# BLE ACL Central Example

This is a BLE ACL Central example application for the Telink B91 Bluetooth Module. It demonstrates how to use the Bluetooth Low Energy (BLE) API to connect to a peripheral device and exchange data with it.

## How to use this example

Key 1 click to start the scanning process, and then select the target device to connect.

Key 2 click to disconnect all the current connection.

Default select the target device by RSSI filter. you can change mode to Device Name or MAC address filter by modifying the macro `APP_BLE_ACL_CENTRAL_RSSI_FILTER` or `APP_BLE_ACL_CENTRAL_NAME_FILTER` or `APP_BLE_ACL_CENTRAL_MAC_FILTER`.

Only one of these macros will take effect.

```c
#define APP_BLE_ACL_CENTRAL_RSSI_FILTER         (-35)
// #define APP_BLE_ACL_CENTRAL_NAME_FILTER         "LE_LINK"       /** < Filter for complete name. */
// #define APP_BLE_ACL_CENTRAL_MAC_FILTER           {0xA4, 0xC1, 0x38}  /** < Filter for high MAC address. */
```


It pairing mode default set legacy justworks, you can change it to other pairing mode by modifying the macro `BLE_HOST_SMP_LEGACY_JUST_WORKS_INIT_PARAMS`.

```c
ble_host_smp_initial(BLE_HOST_SMP_LEGACY_JUST_WORKS_INIT_PARAMS);
```

If you want more functions, you can call stack/ble/host_v1 defined functions.


