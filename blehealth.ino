#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <HardwareSerial.h>

HardwareSerial simSerial(1);  

const int dtrPin = 10;
const int riPin = 11;

// BLE Health Monitoring System Service and Characteristic UUIDs
static BLEUUID serviceUUID("XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"); // Replace with your service UUID
static BLEUUID charUUID("XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX"); // Replace with your characteristic UUID

bool deviceConnected = false;
BLEAdvertisedDevice* myDevice;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {
        BLEDevice::getScan()->stop();
        myDevice = new BLEAdvertisedDevice(advertisedDevice);
        deviceConnected = true;
      }
    }
};

void setup() {
  Serial.begin(115200);
  simSerial.begin(115200, SERIAL_8N1, 18, 17);

  pinMode(dtrPin, OUTPUT);
  digitalWrite(dtrPin, HIGH);
  pinMode(riPin, OUTPUT);
  digitalWrite(riPin, HIGH);

  delay(1000);
  Serial.println("AT command program ready");

  // Initialize Bluetooth LE
  BLEDevice::init("");
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(30);
}

void loop() {
  if (deviceConnected) {
    BLEClient* pClient = BLEDevice::createClient();
    pClient->connect(myDevice);
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    
    if (pRemoteService != nullptr) {
      BLERemoteCharacteristic* pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
      if (pRemoteCharacteristic != nullptr) {
        String value = pRemoteCharacteristic->readValue().c_str();
        Serial.println("Received value: " + value);
        sendToUbidots(value);
      }
    }
    deviceConnected = false;
  }
  delay(10000); // Wait before next scan
}

void sendToUbidots(const String &value) {
  // Send AT commands to module to send data to Ubidots
  simSerial.println("AT+CSQ");
  delay(1000);
  printResponse();

  // Replace with your Ubidots token and variable label
  String token = "YOUR_UBIDOTS_TOKEN";
  String variable = "YOUR_VARIABLE_LABEL";
  String data = "{\"" + variable + "\": " + value + "}";

  simSerial.print("AT+HTTPPARA=\"URL\",\"http://industrial.api.ubidots.com/api/v1.6/devices/esp32?token=" + token + "\"");
  delay(1000);
  printResponse();

  simSerial.print("AT+HTTPPARA=\"CONTENT\",\"application/json\"");
  delay(1000);
  printResponse();

  simSerial.print("AT+HTTPDATA=" + String(data.length()) + ",10000");
  delay(1000);
  simSerial.print(data);
  delay(1000);
  printResponse();

  simSerial.println("AT+HTTPACTION=1");
  delay(1000);
  printResponse();
}

void printResponse() {
  digitalWrite(dtrPin, LOW);
  while (simSerial.available()) {
    Serial.write(simSerial.read());
  }
  digitalWrite(riPin, HIGH);
}