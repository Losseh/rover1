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
    if (value.length() >= 3) {
      uint8_t cmd = value[0];
      int8_t vL = (int8_t)value[1];
      int8_t vR = (int8_t)value[2];

      Serial.printf("CMD=%d VL=%d VR=%d\n", cmd, vL, vR);

      // TU wywołujesz sterowanie silnikami
      // drive(vL, vR);
    } else {
      uint8_t cmd = value[0];
      Serial.printf("CMD=%d", cmd);
    }
  }
};

void setup() {
  Serial.begin(115200);

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

  Serial.println("Robot BLE gotowy");
}

void loop() {
}
