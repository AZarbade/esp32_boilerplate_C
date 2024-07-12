#include <stdio.h>

#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_control.h"
#include "utils.h"

void app_main(void) {
  const char *TAG = "[MAIN]";
  ESP_LOGI(TAG, "Getting Started!");
  get_chip_info();

  uint8_t led_state = 1;
  led_control(2, led_state);
}
