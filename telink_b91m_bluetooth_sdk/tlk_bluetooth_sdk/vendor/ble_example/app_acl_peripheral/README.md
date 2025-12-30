# BLE ACL Peripheral Example

This is a simple example of a BLE peripheral device that supports the Bluetooth Low Energy (BLE) Access Control Profile (ACL). 

The example default Device Name is 'tlk_bluetooth_ble_per', support max connection number is 4. It can be changed by modifying the macro `APP_BLE_ACL_PERIPHERAL_MAX_COUNT` and `BLE_PERIPHERAL_DEVICE_NAME`.

```c
#define APP_BLE_ACL_PERIPHERAL_MAX_COUNT        4
#define BLE_PERIPHERAL_DEVICE_NAME              "tlk_bluetooth_ble_per"
```

It pairing mode default set legacy justworks, you can change it to other pairing mode by modifying the macro `BLE_HOST_SMP_LEGACY_JUST_WORKS_INIT_PARAMS`.

```c
ble_host_smp_initial(BLE_HOST_SMP_LEGACY_JUST_WORKS_INIT_PARAMS);
```

For additional features, you can utilize the functions provided in stack/ble/host_v1.


