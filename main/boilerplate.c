#include <stdbool.h>

#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_control.h"
#include "nvs_flash.h"
#include "utils.h"
#include "wifi_control.h"

void app_main(void) {
  const char *TAG = "[MAIN]";
  ESP_LOGI(TAG, "Getting Started!");
  get_chip_info();

  // LED CONTROL
  led_control(2, 1);

  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    nvs_flash_erase();
    ret = nvs_flash_init();
  }

  // WIFI CONTROL
  char *ssid = "ssid here";
  char *password = "pass here";
  wifi_station_connect(ssid, password);

  led_control(2, 0);
}
