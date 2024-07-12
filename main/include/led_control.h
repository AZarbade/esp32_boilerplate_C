
#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include "freertos/FreeRTOS.h"

void led_control(int gpio_pin, uint8_t led_state);

#endif
