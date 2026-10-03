/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "Arduino.h"

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CMD_CHAR_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#define CMD_GO 1
#define CMD_SONAR 2

class CmdCallback : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pChar) override {
    std::string value = pChar->getValue();

    Serial.printf("BLE WRITE! len=%d: ", value.length());

    for (unsigned char c : value) {
      Serial.printf("%02X ", c);
    }

    Serial.println();

    if (value.length() >= 3) {
      uint8_t cmd = (uint8_t)value[0];
      int8_t vL = (int8_t)value[1];
      int8_t vR = (int8_t)value[2];

      Serial.printf("CMD=%d VL=%d VR=%d\n", cmd, vL, vR);
    }
    else if (value.length() > 0) {
      uint8_t cmd = (uint8_t)value[0];
      Serial.printf("CMD=%d\n", cmd);
    }
  }
};

void setup() {
  Serial.begin(115200);

  Serial.write("init");

  BLEDevice::init("Tymek rover");
  BLEServer *server = BLEDevice::createServer();

  BLEService *service = server->createService(SERVICE_UUID);

  BLECharacteristic *cmdChar = service->createCharacteristic(
    CMD_CHAR_UUID,
    BLECharacteristic::PROPERTY_WRITE
  );

  cmdChar->setCallbacks(new CmdCallback());

  service->start();
  BLEAdvertising *adv = BLEDevice::getAdvertising();
  adv->addServiceUUID(SERVICE_UUID);
  adv->start();

  // Set the RGB LED pins as outputs
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
}

void loop() {
  // Turn on Red (LOW is ON)
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, HIGH);
  delay(1000);

  // Turn on Green
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, HIGH);
  delay(1000);

  // Turn on Blue
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, LOW);
  delay(1000);

  Serial.println("loop end");
}
