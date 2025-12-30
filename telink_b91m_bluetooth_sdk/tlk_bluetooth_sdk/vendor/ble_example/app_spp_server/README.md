# BLE SPP Server Example

ACL configuration parameters used BLE ACL example demo, Server for any ACL role can be used.

SPP server default configuration:

- Start handle: SERVICE_USER_128UUID_START_HDL(0x9000)

Sevice UUID: 00010203-0405-0607-0809-0a0b0c0d1910

Server to Client UUID: 00010203-0405-0607-0809-0a0b0c0d2b10
   - Properties: Read and Notify.
   - Value Handle: 0x9002
   - Encrypted: Yes.

Client to Server UUID: 00010203-0405-0607-0809-0a0b0c0d2b11
    - Properties: Read, Notify, Write, and Write without Response.
    - Value Handle: 0x9005
    - Encrypted: Yes.

## Function

**Read Characteristic Value**

GATT Client Read **Server to Client Characteristic** Value, Device response string "Telink SPP Service Server to Client Characteristic".

GATT Client Read **Client to Server Characteristic** Value, Device response string "Telink SPP Service Client to Server Characteristic", and notify string "Notify Server to Client Telink SPP Service" by **Server to Client Characteristic** UUID.

**Write and Notify Characteristic Value**

GATT Client Write **Client to Server Characteristic** Value, Device will notify string "Notify Client to Server Telink SPP Service" and "Notify2 Client to Server Telink SPP Service". by **Client to Server Characteristic** Handle.

## Note

Demo do not exchange MTU value by default, if you want to exchange MTU value, you can call ble_host_gattc_send_exchange_mtu_req() function after connection established, Or exchange MTU value by remote device.

If MTU size less than string length, the notify string will be cut off.
