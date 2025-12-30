# iOS ANCS Example

This is an example project that demonstrates how to use Apple Notification Center Service (ANCS) in iOS. It uses CoreBluetooth to connect to a peripheral device and receive notifications from it.

Demo currently only subscribes ANCS notifications, but not deal with the actual notifications.

## Features

Default Device Name: "telink ANCS Client"

```c
#define BLE_PERIPHERAL_DEVICE_NAME              "telink ANCS Client"
```

The Demo provides a method to distinguish between iOS devices and non-ios devices. Code is commented to explain the purpose of each section.

```c
static bool app_discovery_ancs_callback(uint16_t conn_handle, uint16_t cid, uint32_t err, const struct gattc_disc_services *service)
{
    (void) conn_handle;
    (void) err;
    (void) cid;
    if (service->start_handle != 0x0000) {
        tlk_printf("Peer Device ANCS Service found, start handle:0x%x, end handle:0x%x", service->start_handle, service->end_handle);
        tlk_printf("The Device is iOS Device");
    } else {
        tlk_printf("Peer Device ANCS Service not found");
        tlk_printf("The Device is not iOS Device");
    }
    return false;
}

ble_host_gattc_discover_primary_service_by_uuid(s_acl_conn_handle, LE_L2CAP_CID_ATT, &ble_ios_ancs_service_uuid, app_discovery_ancs_callback, NULL);
```

You can call it at any time after the connection is completed to distinguish whether it is an iOS device or not.

## ANCS Service Client

This client base Telink Profile layer code. It provides a method to any client function code.

**Initiate**

It will initiate the ANCS client module, you can register the 'init' callback function to the client module.

```c
static const struct ble_prf_client_param s_ancs_client_params = {
    .used_acl_role = PRF_USED_ACL_ROLE_CONNECT,
    .service_id = SERVICE_ID_ANCS,
    .init = NULL,
    .connect = ble_ancsc_connect,
    .discovery = ble_ancsc_discovery,
};

static struct app_ios_ancs_client_control s_ancs_client_ctrl = {
    .prf_process = {
        .client_param = &s_ancs_client_params,
    },
};

void app_ios_ancs_client_init(void)
{
    ble_prf_register_client_module(&s_ancs_client_ctrl.prf_process, NULL);
}
```

**Connect or Disconnect**

It will connect or disconnect ACL connection, you can register the 'connect' callback function to the client module.

```c
static void ble_ancsc_connect(uint16_t conn_handle, enum prf_acl_state_change state)
{
    memset(&s_ancs_client_ctrl.ancs_client, 0, sizeof(s_ancs_client_ctrl.ancs_client));
    if (state == PRF_ACL_CONNECT) {
        s_ancs_client_ctrl.ancs_client.conn_handle = conn_handle;
    } else if (state == PRF_ACL_DISCONNECT) {
        s_ancs_client_ctrl.ancs_client.conn_handle = BLE_CONN_HANDLE_INVALID;
    }
}
```

You can do any operations after the connection is connected or disconnected.

**Discovery**

It will start the all services discovery, when encrypted this connection by default, you can register the 'discovery' callback function to the client module.Currently, it only supports PRF_DISC_TYPE_SVC.

```c
static void ble_ancsc_discovery(uint16_t conn_handle, enum prf_disc_type type)
{
    (void) type;
    ble_host_gatt_ssdp_start_no_include(conn_handle, &s_disc_ancs);
}
```

## ANCS Client Discovery Struct

This struct is used to define the ANCS service and its characteristics. It will be used by the client module to start the discovery process.

service_uuid is the ANCS service UUID, service_callback is the callback function when the ANCS service is found, found finish or not found.

characteristic_size and characteristic are the characteristics of the ANCS service. 

subscribe_notify mean if the characteristic supports notification, it will subscribe to it auto.

If you want to subscribe to the notification message, you can call 'ble_host_gattc_add_subscribe_ccc_message' to add the characteristic to the subscribe list.

```c
static const struct gatt_ssdp_characteristic s_disc_ancs_char[] = {
    {
        .subscribe_notify = true,
        .characteristic_uuid = &ble_ios_ancs_notification_source,
        .characteristic_callback = app_ios_ancs_client_notification_source_found_callback,
    },
    {
        .characteristic_uuid = &ble_ios_ancs_control_point,
        .characteristic_callback = app_ios_ancs_client_control_point_found_callback,
    },
    {
        .subscribe_notify = true,
        .characteristic_uuid = &ble_ios_ancs_data_source,
        .characteristic_callback = app_ios_ancs_client_data_source_found_callback,
    },
};

static const struct ble_gatt_ssdp_no_include_list s_disc_ancs = {
    .max_service_count = 1,
    .service = {
        .service_uuid = &ble_ios_ancs_service_uuid,
        .service_callback = app_ios_ancs_client_found_service_callback,
    },
    .characteristic_table = {
        .characteristic_size = ARRAY_SIZE(s_disc_ancs_char),
        .characteristic = s_disc_ancs_char,
    }
};
```
