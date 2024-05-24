#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <esp_bt_device.h>
#include <BLE2902.h> 

// Dirección MAC del Xiaomi electric scooter Lite 3
uint8_t scooterMAC[] = {0x68, 0xAB, 0xBC, 0x4E, 0x6C, 0x4E};

// Nombre del scooter
const char* deviceName = "dreame.scooter.epro";

// UUIDs de los servicios y características obtenidos en los logs
#define SERVICE_UUID_FE95 "0000fe95-0000-1000-8000-00805f9b34fb" //95
#define SERVICE_UUID_0100 "00000100-0065-6c62-2e74-6f696d2e696d" //TX/RX
#define SERVICE_UUID_1000 "00001000-0065-6c62-2e74-6f696d2e696d"

#define CHARACTERISTIC_UUID_04 "00000004-0000-1000-8000-00805f9b34fb"
#define CHARACTERISTIC_UUID_10 "00000010-0000-1000-8000-00805f9b34fb"
#define CHARACTERISTIC_UUID_16 "00000016-0000-1000-8000-00805f9b34fb"
#define CHARACTERISTIC_UUID_17 "00000017-0000-1000-8000-00805f9b34fb"
#define CHARACTERISTIC_UUID_18 "00000018-0000-1000-8000-00805f9b34fb"
#define CHARACTERISTIC_UUID_1A "0000001a-0000-1000-8000-00805f9b34fb"
#define CHARACTERISTIC_UUID_1B "0000001b-0000-1000-8000-00805f9b34fb"
#define CHARACTERISTIC_UUID_1C "0000001c-0000-1000-8000-00805f9b34fb"
#define CHARACTERISTIC_UUID_0101 "00000101-0065-6c62-2e74-6f696d2e696d"
#define CHARACTERISTIC_UUID_0102 "00000102-0065-6c62-2e74-6f696d2e696d"
#define CHARACTERISTIC_UUID_1001 "00001001-0065-6c62-2e74-6f696d2e696d"
#define CHARACTERISTIC_UUID_1002 "00001002-0065-6c62-2e74-6f696d2e696d"
#define CHARACTERISTIC_UUID_1003 "00001003-0065-6c62-2e74-6f696d2e696d"

BLEServer *pServer = NULL;

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    Serial.println("Device connected");
  }

  void onDisconnect(BLEServer* pServer) {
    Serial.println("Device disconnected");
    BLEDevice::startAdvertising();
  }
};

class MyCharacteristicCallbacks : public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic* pCharacteristic) {
    Serial.print("Characteristic read: ");
    Serial.println(pCharacteristic->getUUID().toString().c_str());
  }

  void onWrite(BLECharacteristic* pCharacteristic) {
    Serial.print("Characteristic written: ");
    Serial.println(pCharacteristic->getUUID().toString().c_str());
    std::string value = pCharacteristic->getValue();
    Serial.print("Value: ");
    for (int i = 0; i < value.length(); i++) {
      Serial.printf("%02x ", (unsigned char)value[i]);
    }
    Serial.println();
  }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  // Spoofing de la MAC
  esp_base_mac_addr_set(scooterMAC);
  
  // Inicializar el dispositivo BLE
  BLEDevice::init(deviceName);
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Crear servicio FE95 y sus características
  BLEService *pServiceFE95 = pServer->createService(SERVICE_UUID_FE95);
  BLECharacteristic *pCharacteristic04 = pServiceFE95->createCharacteristic(
    CHARACTERISTIC_UUID_04,
    BLECharacteristic::PROPERTY_READ
  );
  pCharacteristic04->setCallbacks(new MyCharacteristicCallbacks());

  BLECharacteristic *pCharacteristic10 = pServiceFE95->createCharacteristic(
    CHARACTERISTIC_UUID_10,
    BLECharacteristic::PROPERTY_NOTIFY
  );
  pCharacteristic10->addDescriptor(new BLE2902());
  pCharacteristic10->setCallbacks(new MyCharacteristicCallbacks());

  BLECharacteristic *pCharacteristic16 = pServiceFE95->createCharacteristic(
    CHARACTERISTIC_UUID_16,
    BLECharacteristic::PROPERTY_NOTIFY
  );
  pCharacteristic16->addDescriptor(new BLE2902());
  pCharacteristic16->setCallbacks(new MyCharacteristicCallbacks());

  BLECharacteristic *pCharacteristic17 = pServiceFE95->createCharacteristic(
    CHARACTERISTIC_UUID_17,
    BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY
  );
  pCharacteristic17->addDescriptor(new BLE2902());
  pCharacteristic17->setCallbacks(new MyCharacteristicCallbacks());

  BLECharacteristic *pCharacteristic18 = pServiceFE95->createCharacteristic(
    CHARACTERISTIC_UUID_18,
    BLECharacteristic::PROPERTY_NOTIFY
  );
  pCharacteristic18->addDescriptor(new BLE2902());
  pCharacteristic18->setCallbacks(new MyCharacteristicCallbacks());

  BLECharacteristic *pCharacteristic1A = pServiceFE95->createCharacteristic(
    CHARACTERISTIC_UUID_1A,
    BLECharacteristic::PROPERTY_NOTIFY
  );
  pCharacteristic1A->addDescriptor(new BLE2902());
  pCharacteristic1A->setCallbacks(new MyCharacteristicCallbacks());

  BLECharacteristic *pCharacteristic1B = pServiceFE95->createCharacteristic(
    CHARACTERISTIC_UUID_1B,
    BLECharacteristic::PROPERTY_NOTIFY
  );
  pCharacteristic1B->addDescriptor(new BLE2902());
  pCharacteristic1B->setCallbacks(new MyCharacteristicCallbacks());

  BLECharacteristic *pCharacteristic1C = pServiceFE95->createCharacteristic(
    CHARACTERISTIC_UUID_1C,
    BLECharacteristic::PROPERTY_NOTIFY
  );
  pCharacteristic1C->addDescriptor(new BLE2902());
  pCharacteristic1C->setCallbacks(new MyCharacteristicCallbacks());

  pServiceFE95->start();

  // Crear servicio 0100 y sus características
  BLEService *pService0100 = pServer->createService(SERVICE_UUID_0100);
  BLECharacteristic *pCharacteristic0101 = pService0100->createCharacteristic(
    CHARACTERISTIC_UUID_0101,
    BLECharacteristic::PROPERTY_WRITE
  );
  pCharacteristic0101->setCallbacks(new MyCharacteristicCallbacks());

  BLECharacteristic *pCharacteristic0102 = pService0100->createCharacteristic(
    CHARACTERISTIC_UUID_0102,
    BLECharacteristic::PROPERTY_NOTIFY
  );
  pCharacteristic0102->addDescriptor(new BLE2902());
  pCharacteristic0102->setCallbacks(new MyCharacteristicCallbacks());

  pService0100->start();

  // Crear servicio 1000 y sus características
  BLEService *pService1000 = pServer->createService(SERVICE_UUID_1000);
  BLECharacteristic *pCharacteristic1001 = pService1000->createCharacteristic(
    CHARACTERISTIC_UUID_1001,
    BLECharacteristic::PROPERTY_WRITE
  );
  pCharacteristic1001->setCallbacks(new MyCharacteristicCallbacks());

  BLECharacteristic *pCharacteristic1002 = pService1000->createCharacteristic(
    CHARACTERISTIC_UUID_1002,
    BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_READ
  );
  pCharacteristic1002->addDescriptor(new BLE2902());
  pCharacteristic1002->setCallbacks(new MyCharacteristicCallbacks());

  BLECharacteristic *pCharacteristic1003 = pService1000->createCharacteristic(
    CHARACTERISTIC_UUID_1003,
    BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_READ
  );
  pCharacteristic1003->addDescriptor(new BLE2902());
  pCharacteristic1003->setCallbacks(new MyCharacteristicCallbacks());

  pService1000->start();

  // Advertising BLE
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID_FE95);
  pAdvertising->addServiceUUID(SERVICE_UUID_0100);
  pAdvertising->addServiceUUID(SERVICE_UUID_1000);
  pAdvertising->setScanResponse(true);
  pAdvertising->start();
  
  Serial.println("Advertising started");
}

void loop() {
  delay(1000);
}
