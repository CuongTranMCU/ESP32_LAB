
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "nvs_flash.h"
#include "wifi.h"
#include "esp_log.h"
// dht11
#include "DHT11.h"
static const char *TAG = "DEVICE";
uint32_t MQTT_CONNEECTED = 0;

void Publisher_Task(void *params)
{
  while (true)
  {
    // DHT11
    int temp = DHT11_read().temperature;
    int hum = DHT11_read().humidity;
    if (MQTT_CONNEECTED)
    {
      esp_mqtt_client_handle_t client = get_mqtt_client_handle();
      char *json_data = convert_model_sensor_to_json(temp, hum);
      mqtt_data_publish_callback(json_data);
      free(json_data);
    }
    vTaskDelay(10000 / portTICK_PERIOD_MS);
  }
}

void DisplayOled()
{
  ssd1306_init();
  task_ssd1306_display_text();
}

void app_main(void)
{
  esp_err_t err;

  ESP_LOGI(TAG, "Initializing...");
  err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES)
  {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);
  wifi_init_sta();
  DHT11_init(GPIO_NUM_5);
  xTaskCreate(Publisher_Task, "Publisher_Task", 1024 * 5, NULL, 5, NULL);
}
