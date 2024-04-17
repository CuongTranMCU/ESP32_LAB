#include "ssd1306.h"
#include "freertos/task.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "sdkconfig.h"
static const char *TAG = "i2c-example";
#define _I2C_NUMBER(num) I2C_NUM_0
#define I2C_NUMBER(num) _I2C_NUMBER(num)
#define DATA_LENGTH 512                  /*!< Data buffer length of test buffer */
#define RW_TEST_LENGTH 128               /*!< Data length for r/w test, [0,DATA_LENGTH] */
#define DELAY_TIME_BETWEEN_ITEMS_MS 1000 /*!< delay time between different test items */

#define I2C_MASTTER_NUM 0

#define I2C_MASTER_SCL_IO 22                                /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO 21                                /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUMBER(CONFIG_I2C_MASTER_PORT_NUM) /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ CONFIG_I2C_MASTER_FREQUENCY        /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0                           /*!< I2C master doesn't need buffer */


static esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;                                           
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
        // .clk_flags = 0,          /*!< Optional, you can use I2C_SCLK_SRC_FLAG_* flags to choose i2c source clock here. */
    };
    esp_err_t err = i2c_param_config(i2c_master_port, &conf);
    if (err != ESP_OK) {
        return err;
    }
    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}
void app_main(void)
{
    ESP_LOGI(TAG,"Initialize I2C Master");
    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG,"Initialize successful");
    ESP_LOGI(TAG,"Intitialize OLED");
    ssd1306_init();
    xTaskCreate(&task_ssd1306_display_clear, "ssd1306_display_clear",  2048, NULL, 6, NULL);
	vTaskDelay(100/portTICK_PERIOD_MS);
	xTaskCreate(&task_ssd1306_display_text, "ssd1306_display_text",  2048,
		(void *)"    21521909\n\n    21521910", 6, NULL);
    ESP_LOGI(TAG,"FINISH");
}
