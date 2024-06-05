
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "nvs_flash.h"
#include "wifi.h"
#include "esp_log.h"
// dht11
#include "DHT11.h"
#include "ssd1306.h"
SSD1306_t dev;
int page = 0;
static const char *TAG = "DEVICE";
uint32_t MQTT_CONNEECTED = 0;
void initOLED()
{
	i2c_master_init(&dev,CONFIG_SDA_GPIO,CONFIG_SCL_GPIO,CONFIG_RESET_GPIO);
	ssd1306_init(&dev,128,64);
	ssd1306_clear_screen(&dev,false);
	ssd1306_contrast(&dev,0xff);
    page +=2;
}

void Publisher_Task(void *params)
{
  while (true)
  {
    // DHT11
    ssd1306_clear_screen(&dev,false);
    int temp = DHT11_read().temperature;
    int hum = DHT11_read().humidity;
    char humidity[20];
    sprintf(humidity, "hum: %d percent", hum);
    char temperature[20];
    sprintf(temperature, "temp: %d degC", temp);
    if (MQTT_CONNEECTED)
    {
      esp_mqtt_client_handle_t client = get_mqtt_client_handle();
      char *json_data = convert_model_sensor_to_json(temp, hum);
      ssd1306_display_text(&dev, page, (char *)temperature, 20, false);
      page += 2;
      ssd1306_display_text(&dev, page, (char *)humidity, 20, false);
      mqtt_data_publish_callback(json_data);
      free(json_data);
    }
    page = 0;
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
  initOLED();
  xTaskCreate(Publisher_Task, "Publisher_Task", 1024 * 5, NULL, 5, NULL);
}
