# Create New Demo Example

This example shows how to create a new demo application using the Bluetooth Low Energy (BLE) stack. Like "Hello World" examples, this application simply prints a message.

```c
int INIT(APP_BLE_NEW_DEMO)(void)
{
    tlk_printf("Hello, Telink BLE Demo initialized!");
    return 0;
}

void START(APP_BLE_NEW_DEMO)(void)
{
    tlk_printf("Hello, Telink BLE Demo started!");
}
```

tlk_printf is SDK printf log function, which can be used to print messages to the console.

## How to Create New Demo

- [BLE Example Overview](../README.md)
