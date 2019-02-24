
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

## GATT Specifications

### Service Name

This service provides WPA Connection Service.

UUID: `6a1c63be-71ac-4d8d-91b1-8b66af6c3193`

### Service Characteristics

#### SSID

- Property: Write
- Type: org.bluetooth.characteristic.string
- UUID: `0a0afd34-f565-416a-a57f-4a652fed52eb`

#### Password

- Property: Write
- Type: org.bluetooth.characteristic.string
- UUID: `40b4cb5a-739b-49d3-b230-6e16e469e93f`

### Connect

- Property: Write
- Type: boolean
  - 0: To disconnect
  - 1: To connect
- UUID: `79995725-db40-40f7-9c1d-a4ff7116c230`

#### IpAddr

- Property: Read
- Type: org.bluetooth.characteristic.string
- UUID: `766DD6E6-E301-4283-AFD9-F5EEC5FA49B2`

#### Status

- Property: Read
- Type: uint8
- UUID: `f51f39f3-8fa2-4f9e-846f-00b36053ac0e`

| Bit | Name | Description  |
|---|---|---|---|---|
| 0 |   |   |   |   |
| 1 |   |   |   |   |
| 2 |   |   |   |   |
| 3 |   |   |   |   |
| 4 |   |   |   |   |
| 5 |   |   |   |   |
| 6 |   |   |   |   |
| 7 |   |   |   |   |

### Flow

#### Connect

1. Write SSID
2. Write Password
3. Write `Connect` to 1

#### Disconnect

1. Write `Connect` to 0

