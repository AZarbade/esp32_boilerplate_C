// for more info:
// https://github.com/espressif/esp-idf/blob/master/examples/get-started/hello_world/main/hello_world_main.c

#include "utils.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"

void get_chip_info() {
  const char *TAG = "[CHIP]";
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  ESP_LOGI(TAG, "This is %s chip with %d CPU core(s), %s%s%s%s",
           CONFIG_IDF_TARGET, chip_info.cores,
           (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
           (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
           (chip_info.features & CHIP_FEATURE_IEEE802154)
               ? ", 802.15.4 (Zigbee/Thread)"
               : "");

  uint32_t flash_size;
  if (esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
    printf("Get flash size failed");
    return;
  }

  ESP_LOGI(TAG, "%" PRIu32 "MB %s flash", flash_size / (uint32_t)(1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded"
                                                         : "external");

  ESP_LOGI(TAG, "Minimum free heap size: %" PRIu32 " bytes",
           esp_get_minimum_free_heap_size());
}
