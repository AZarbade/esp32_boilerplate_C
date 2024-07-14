// for more info:
// https://github.com/espressif/esp-idf/blob/master/examples/get-started/blink/main/blink_example_main.c

#include "led_control.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

const char *LED_TAG = "[LED]";

void led_control(int gpio_pin, uint8_t led_state) {
  gpio_reset_pin(gpio_pin);
  gpio_set_direction(gpio_pin, GPIO_MODE_OUTPUT);
  ESP_LOGI(LED_TAG, "Turning the LED %s!", led_state == true ? "ON" : "OFF");
  gpio_set_level(gpio_pin, led_state);
}
