# BLE SMP Example

This example current support legacy pairing and secure connection. 
Secure connection OOB and keypress notify messages are not supported.

ACL configuration parameters used BLE ACL example demo, SMP support any ACL role, including central and peripheral. 
If remote device supports ACL peripheral role and central role, 
you can pair with it using ACL peripheral role then reconnect with ACL central role.

## How to use example

app_example.h select demo APP_DEMO_SELECT to APP_BLE_ACL_SMP, and app_acl_smp.c 
select smp pairing APP_ACL_SMP_SELECT_MODE than you want to test it. then build it and download to your device.

If smp mode support input or output interface, output interface callback function is app_acl_smp_pairing_output_callback, 
default printf output value into log.

input interface callback function is app_acl_smp_pairing_input_callback, default input value from RISC-V TDB. 
Input format is "11 xx xx xx" for any pairing mode.

For example, if your smartphone displays the pairing PIN code as 123456, simply enter "11 12 34 56" in the RISC-V TDB to input the PIN code. The input is handled by the `tlkusb_debug_shell_hook` function; you can modify this function if you wish to support PIN code input directly from the smartphone or other interfaces.

## Note

When operating as an ACL peripheral, the device does not automatically initiate a secure request to the remote side; instead, pairing must be initiated from the remote device. If you prefer the peripheral to actively start the secure request, you can invoke the ble_host_smp_start_pairing_peripheral() function.

When functioning as an ACL central, the device by default sends a pairing request if there is no existing bond, or starts an encryption request if already bonded. If you wish to force a pairing request every time a connection is made, you may use the ble_host_smp_start_new_pairing_central() function.
