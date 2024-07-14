// for more info:
// https://github.com/espressif/esp-idf/blob/master/examples/wifi/getting_started/station/main/station_example_main.c

#include <stdbool.h>
#include <string.h>

#include "esp_err.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_netif_types.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"
#include "portmacro.h"
#include "wifi_control.h"

static EventGroupHandle_t wifi_event_group;
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

const char *WIFI_TAG = "[WIFI]";

static int retry_num = 0;
static void event_handler(void *arg, esp_event_base_t event_base,
                          int32_t event_id, void *event_data) {
  if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
    esp_wifi_connect();
  } else if (event_base == WIFI_EVENT &&
             event_id == WIFI_EVENT_STA_DISCONNECTED) {
    if (retry_num < 5) {
      esp_wifi_connect();
      retry_num++;
      ESP_LOGI(WIFI_TAG, "retry to connect to the AP");
    } else {
      xEventGroupSetBits(wifi_event_group, WIFI_FAIL_BIT);
    }
    ESP_LOGI(WIFI_TAG, "connect to the AP fail");
  } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
    ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
    ESP_LOGI(WIFI_TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
    retry_num = 0;
    xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
  }
}

void wifi_station_connect(const char *ssid, const char *password) {
  wifi_event_group = xEventGroupCreate();

  ESP_LOGI(WIFI_TAG, "WiFi init...");
  esp_event_loop_create_default();
  esp_netif_init();
  esp_netif_create_default_wifi_sta();
  ESP_LOGI(WIFI_TAG, "WiFi in station mode");
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  esp_wifi_init(&cfg);

  esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler,
                             NULL);
  esp_event_handler_register(IP_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL);

  wifi_config_t wifi_cfg = {0};
  strncpy((char *)wifi_cfg.sta.ssid, ssid, sizeof(wifi_cfg.sta.ssid) - 1);
  strncpy((char *)wifi_cfg.sta.password, password,
          sizeof(wifi_cfg.sta.password) - 1);

  esp_wifi_set_mode(WIFI_MODE_STA);
  esp_wifi_set_config(WIFI_IF_STA, &wifi_cfg);
  esp_wifi_start();
  ESP_LOGI(WIFI_TAG, "WiFi station has started!");

  EventBits_t bits =
      xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                          pdFALSE, pdFALSE, 10000 / portTICK_PERIOD_MS);

  if (bits & WIFI_CONNECTED_BIT) {
    ESP_LOGI(WIFI_TAG, "Connected to WiFi!");
  } else if (bits & WIFI_FAIL_BIT) {
    ESP_LOGE(WIFI_TAG, "Failed to connect to WiFi!");
  } else {
    ESP_LOGE(WIFI_TAG, "Unexpected Event");
  }
}

//    Post Office (ESP32 System)
//    ┌──────────────────────────────────────┐
//    │                                      │
//    │  ┌─────────────┐    ┌─────────────┐  │
//    │  │ Wi-Fi Events│    │   IP Events │  │
//    │  │ Mailbox     │    │   Mailbox   │  │
//    │  │ ┌─────────┐ │    │ ┌─────────┐ │  │
//    │  │ │ Event 1 │ │    │ │ Event 1 │ │  │
//    │  │ │ Event 2 │ │    │ │ Event 2 │ │  │
//    │  │ │ Event 3 │ │    │ │ Event 3 │ │  │
//    │  │ └─────────┘ │    │ └─────────┘ │  │
//    │  └─────────────┘    └─────────────┘  │
//    │                                      │
//    │         Mail Sorter (Event Loop)     │
//    │                     │                │
//    └─────────────────────┼────────────────┘
//                          │
//                          ▼
//                  Your Event Handler
//              (Person checking mailboxes)
