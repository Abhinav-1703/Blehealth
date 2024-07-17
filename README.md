# BLE Health Monitoring System with ESP32 and GSM Module

This project involves creating a health monitoring system that uses BLE to collect data from sensors and send the data to Ubidots via a GSM module connected to an ESP32.

## Components Used
- ESP32
- GSM Module (connected via RX/TX using HardwareSerial)
- BLE-enabled Health Monitoring Sensors
- Ubidots for data visualization

## Prerequisites
1. **ESP32 Board Support**: Install the ESP32 board support in the Arduino IDE. Follow the instructions [here](https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md).
2. **BLE Library**: Ensure you have the necessary BLE library for the ESP32. This should be included with the ESP32 board support package.
3. **Ubidots Account**: Sign up for an account at [Ubidots](https://ubidots.com/) to obtain your Ubidots token.

## Wiring
- **GSM Module**:
  - Connect the TX pin of the GSM module to RX pin 18 of the ESP32S3.
  - Connect the RX pin of the GSM module to TX pin 17 of the ESP32S3.
  - Connect the DTR pin of the GSM module to GPIO 10 of the ESP32S3.
  - Connect the RI pin of the GSM module to GPIO 11 of the ESP32S3.
