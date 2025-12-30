# BLE SSDP Example

SSDP (Simple Service Discovery Protocol) is a protocol for discovering services on a local network. This example demonstrates how to use the BLE API to implement an SSDP server.

ACL configuration parameters used BLE ACL example demo, Server for any ACL role can be used.

Default SDP Information:

GAP(Generic Access Profile) Service: Support Characteristic: Device Name, Appearance, Peripheral Preferred Connection Parameters, Central Address Resolution.

GATT(Generic Attribute Profile) Service: Support Characteristic: Service Changed, Client Supported Features, Database Hash, Server Supported Features.

When ACL connected, device will discovery GAP and GATT services and their characteristics.
