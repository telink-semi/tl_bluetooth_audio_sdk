# ble service discovery protocol (SDP)

ble_sdp.h is similar to Android Bluetooth Service Discovery Protocol (SDP) API. It provides a way for a client to discover services and characteristics on a remote device.

ble_ssdp.h provides a single service discovery function, if user only wants to discover a single service. User need know the UUID of the service and characteristic to be discovered.

There are two methods, and the user can only choose one.
