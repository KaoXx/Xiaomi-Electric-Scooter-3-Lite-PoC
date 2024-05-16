#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer *pServer;
BLEService *pService;
BLECharacteristic *pTXCharacteristic;
BLECharacteristic *pButtonCharacteristic;

#define SERVICE_UUID        "00000101-0065-6C62-2E74-6F696D2E696D"
#define TX_CHARACTERISTIC_UUID "00000100-0065-6C62-2E74-6F696D2E696D"
#define BUTTON_CHARACTERISTIC_UUID "00000100-0065-6C62-2E74-6F696D2E696D"

#define BUTTON_PIN 2 // Cambia el pin según corresponda

// Definimos la MAC personalizada (La misma que el Xioami Scooter Lite 3)
uint8_t newMAC[] = {0x68, 0xAB, 0xBC, 0x4E, 0x6C, 0x4E};

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      Serial.println("C");
    };

    void onDisconnect(BLEServer* pServer) {
      Serial.println("D");
    }
};

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  // Cambiamos la MAC personalizada
  if (esp_base_mac_addr_set(newMAC) == ESP_OK) {
    Serial.println("S");
  } else {
    Serial.println("F");
  }
  BLEDevice::init("dreame.scooter.epro");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  
  pService = pServer->createService(SERVICE_UUID);
  
  pTXCharacteristic = pService->createCharacteristic(
                                         TX_CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_NOTIFY
                                       );
  
  pButtonCharacteristic = pService->createCharacteristic(
                                         BUTTON_CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  
  pService->start();
  
  pServer->getAdvertising()->start();
  Serial.println("W8");
}

void loop() {
  // Lee el estado del botón y actualiza el valor de la característica correspondiente
  bool buttonState = digitalRead(BUTTON_PIN);
  pButtonCharacteristic->setValue((uint8_t*)&buttonState, 1);
  pButtonCharacteristic->notify();

  delay(1000); // Actualiza cada segundo
}
