#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SCOOTER_NAME "dreame.scooter.epro"
#define SCOOTER_MAC {0x68, 0xAB, 0xBC, 0x4E, 0x6C, 0x4E}

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;
BLECharacteristic *pCharacteristicNotification = NULL;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  BLEDevice::init(SCOOTER_NAME);
  pServer = BLEDevice::createServer();

  // Create the first service and characteristics
  BLEService *pService = pServer->createService(BLEUUID("0000fe95-0000-1000-8000-00805f9b34fb"));
  pCharacteristic = pService->createCharacteristic(
    BLEUUID("00000004-0000-1000-8000-00805f9b34fb"),
    BLECharacteristic::PROPERTY_READ
  );

  pCharacteristicNotification = pService->createCharacteristic(
    BLEUUID("00000010-0000-1000-8000-00805f9b34fb"),
    BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_WRITE_NR
  );
  pCharacteristicNotification->addDescriptor(new BLE2902());

  pService->start();

  // Create the second service and characteristics
  BLEService *pService2 = pServer->createService(BLEUUID("00000100-0065-6c62-2e74-6f696d2e696d"));
  pCharacteristic = pService2->createCharacteristic(
    BLEUUID("00000101-0065-6c62-2e74-6f696d2e696d"),
    BLECharacteristic::PROPERTY_WRITE_NR
  );

  BLECharacteristic *pCharacteristicNotify = pService2->createCharacteristic(
    BLEUUID("00000102-0065-6c62-2e74-6f696d2e696d"),
    BLECharacteristic::PROPERTY_NOTIFY
  );
  pCharacteristicNotify->addDescriptor(new BLE2902());

  pService2->start();

  // Create the third service and characteristics
  BLEService *pService3 = pServer->createService(BLEUUID("00001000-0065-6c62-2e74-6f696d2e696d"));
  BLECharacteristic *pCharacteristicWrite = pService3->createCharacteristic(
    BLEUUID("00001001-0065-6c62-2e74-6f696d2e696d"),
    BLECharacteristic::PROPERTY_WRITE
  );

  BLECharacteristic *pCharacteristicNotifyRead = pService3->createCharacteristic(
    BLEUUID("00001002-0065-6c62-2e74-6f696d2e696d"),
    BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_READ
  );
  pCharacteristicNotifyRead->addDescriptor(new BLE2902());

  pCharacteristicNotifyRead = pService3->createCharacteristic(
    BLEUUID("00001003-0065-6c62-2e74-6f696d2e696d"),
    BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_READ
  );
  pCharacteristicNotifyRead->addDescriptor(new BLE2902());

  pService3->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
  // Here we could simulate notifications, reads, and writes as per the log
  delay(1000);
}
