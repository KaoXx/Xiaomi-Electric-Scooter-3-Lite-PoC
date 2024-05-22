#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"

void setup() {
  Serial.begin(115200);
  Serial.println("Starting RF Interference");

  // Initialize NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  // Initialize the WiFi driver
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA)); // Set mode to station to avoid connecting to any network
  ESP_ERROR_CHECK(esp_wifi_start());

  // Set the channel to generate interference (1 to 13 for 2.4 GHz)
  ESP_ERROR_CHECK(esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE));

  // Enable continuous transmission mode
  ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE)); // Disable power saving
  ESP_ERROR_CHECK(esp_wifi_set_bandwidth(WIFI_IF_STA, WIFI_BW_HT40)); // Set bandwidth to 40MHz

  // Transmit arbitrary data to create interference
  const uint8_t wifi_tx_test_packet[] = {
    0x80, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
  };

  // Send the test packet continuously
  esp_wifi_80211_tx(WIFI_IF_STA, wifi_tx_test_packet, sizeof(wifi_tx_test_packet), true);
}

void loop() {
  // Continuously emit signals to create interference
  delay(1000); // Adjust delay as necessary
}
