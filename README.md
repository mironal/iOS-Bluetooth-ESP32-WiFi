
# iOS -> Bluetooth -> ESP32 -> Wi-Fi

> Securely send WiFi password to ESP 32 using iOS and Bluetooth.

You don't need following code anymore.

```c
// Very sensitive data.
const char* ssid     = "your ssid";
const char* password = "your password";
```

You can easily configure WiFi of ESP32 using Bluetooth from iOS device.

```
|Your iOS Device |
        |
        | Bluetooth
        |
    |ESP 32|
        |
        | Wi-Fi
        |
|Wi-Fi Access Point|
```