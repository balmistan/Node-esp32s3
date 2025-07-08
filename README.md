# ESP-IDF MQTT v5 Authentication Example

This is an ESP-IDF project that demonstrates how to connect to an MQTT v5 broker using username and password.

## Features

- MQTT v5 protocol support
- Wi-Fi STA connection
- Username/password authentication
- Compatible with brokers like Mosquitto or HiveMQ

## Configuration

Edit the `sdkconfig.defaults` or use `idf.py menuconfig` to set:

- Wi-Fi SSID and password
- MQTT broker URI
- MQTT username and password

## Build and Flash

```bash
idf.py set-target esp32s3
idf.py defconfig
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```
