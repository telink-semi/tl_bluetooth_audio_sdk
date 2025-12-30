# BLE legacy advertising example

This example demonstrates how to use the legacy BLE advertising API in SDK. It supports ADV_IND, ADV_SCAN_IND, and ADV_NONCONN_IND advertising types.

Default Configuration:

ADV Type: ADV_IND, can change marco APP_ADV_TYPE value to change advertising type.

```c
#define ADV_TYPE_ADV_IND            0
#define ADV_TYPE_ADV_SCAN_IND       1
#define ADV_TYPE_ADV_NONCONN_IND    2

#define APP_ADV_TYPE                ADV_TYPE_ADV_IND
```

Default Advertising Name: "app_ble_adv"

```c
#define BLE_DEVICE_NAME             "app_ble_adv"
```

Default Advertising Interval: Min Interval 100ms, Max Interval 200ms

```c
ble_host_gap_adv_set_param(BLE_HOST_GAP_ADV_SET_ADV_IND(100, 200));
```
