# BLE L2CAP CoC Example

This demo shows how to use the BLE API to implement a simple L2CAP CoC connection between a central and a peripheral device.

## BLE ACL Role

The peripheral role can read [BLE ACL peripheral role](../app_acl/README.md)

## CoC Parameter configuration

Default SPSM is 0x0080, credit is 20, MTU is 1024, MPS is 512, max count is 64.

```c
#define BLE_L2CAP_COC_PARAMETER_SPSM          0x0080
#define BLE_L2CAP_COC_PARAMETER_INITIAL_CREDIT  20
#define BLE_L2CAP_COC_PARAMETER_MTU            1024
#define BLE_L2CAP_COC_PARAMETER_MPS            512
#define BLE_L2CAP_COC_MAX_COUNT                 64
#define BLE_L2CAP_COC_ACL_MAX_NUMBER           8
```

## UI Configuration

The demo provides a simple UI configuration to set the CoC parameter.

| Key ID | Description | Key ID | Description        |
|--------|-------------|--------|--------------------|
| 0      | Starting Connection | 4      | Disconnect all CoC connections |
| 1      | Disconnect all connection| 5      | send data by CoC Channel |
| 2      | create a CoC connection | 6      | None |
| 3      | create 5 CoC connections | 7      | None |

Key ID how to use can [app_key.h](../README_zh.md)
