# file description: HCI Command Interface

## hci_cmd_vendor.h

### Description
This file contains the definitions of the HCI commands for the vendor specific commands.

LE Vendor Specific HCI Commands only use the OCF (0x50),and use sub command to distinguish different commands.

### Format
OGF: 0x3F + OCF: 0x50 + Vendor Sub Command(0x01~0xff) + data[]


