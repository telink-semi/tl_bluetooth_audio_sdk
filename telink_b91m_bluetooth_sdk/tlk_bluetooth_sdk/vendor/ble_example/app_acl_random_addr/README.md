# BLE Random Address Example

ACL configuration parameters used BLE ACL example demo, support static random address, non-resolvable private address, and resolvable private address.

non-resolvable private address current not support reconnect after paired.

static random address using default address: D2-76-C3-C3-84-7F

non-resolvable private address using default address: 26-28-66-95-EB-2A

resolvable private address using random address by per power on, only support paired device can reconnect.

```
int ble_host_gap_set_resolvable_private_addr(void)
{
    uint8_t addr[6];
    ble_host_smp_generate_resolvable_private_addr(addr);
    return ble_host_gap_set_random_addr(addr);
}
```