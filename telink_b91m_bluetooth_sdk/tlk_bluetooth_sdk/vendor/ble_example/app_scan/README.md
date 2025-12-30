# BLE Scan Example

This example demonstrates how to start scan that include legacy scan and extended scan. Example supports legacy scan, extended scan, active scan mode and passive scan mode.

```c
#define APP_BLE_SCAN_MODE_LEGACY        1
#define APP_BLE_SCAN_MODE_EXTENDED      2

#define APP_BLE_SCAN_FUNC_ACTIVE_SCAN   1
#define APP_BLE_SCAN_FUNC_PASSIVE_SCAN  2


#define APP_BLE_SCAN_MODE_DEFAULT       APP_BLE_SCAN_MODE_LEGACY
#define APP_BLE_SCAN_FUNC_DEFAULT       APP_BLE_SCAN_FUNC_PASSIVE_SCAN
```

## How to subscribe to scan results

```c
static void app_scan_report_evt_handler(uint32_t event_id, const void *data, void *user_data);

static const struct ble_host_gap_evt_subscribe_param s_scan_evt_param = {
    .gap_le_scan_mask = 1,
    .handler = app_scan_report_evt_handler,
};

static struct ble_host_gap_evt_subscribe s_scan_evt = {
    .next_handler = NULL,
    .param = &s_scan_evt_param,
};

static void app_scan_report_evt_handler(uint32_t event_id, const void *data, void *user_data)
{
    (void) user_data;
    if (event_id == BLE_HOST_GAP_EVT_LE_SCAN_REPORT_UNDIRECTED) {
        const struct ble_host_gap_evt_scan_adv_report_undirected *report = data;
        tlk_printf("[APP] scan report (undirected), MAC is %s", addr_to_str(report->addr));
    } else if (event_id == BLE_HOST_GAP_EVT_LE_SCAN_REPORT_DIRECTED) {

    }
}
```

any scan report will deal with new struct value.

```c
struct ble_host_gap_evt_scan_adv_report_undirected {
    bool legacy;        /** < If true, controller received ADV_IND, otherwise */
    bool connectable;   /** < The device is connectable. */
    uint8_t addr_type;
    uint8_t addr[6];
    uint8_t sid;        /** < 0xFF, No ADI field provided. */
    int8_t tx_power;    /** < 0x7F, TX power information not available. */
    int8_t rssi;        /** < 0x7F, RSSI information not available. */
    uint16_t pa_interval;   /** < 0x0000, No periodic advertising. */
    uint16_t data_len;
    const uint8_t *data;
};

struct ble_host_gap_evt_scan_adv_report_directed {
    bool legacy;        /** < If true, controller received ADV_IND, otherwise */
    bool connectable;   /** < The device is connectable. */
    uint8_t addr_type;
    uint8_t addr[6];
    uint8_t direct_addr_type;
    uint8_t direct_addr[6];
    uint8_t sid;        /** < 0xFF, No ADI field provided. */
    int8_t tx_power;    /** < 0x7F, TX power information not available. */
    int8_t rssi;        /** < 0x7F, RSSI information not available. */
    uint16_t pa_interval;   /** < 0x0000, No periodic advertising. */
    uint16_t data_len;
    const uint8_t *data;
};
```
