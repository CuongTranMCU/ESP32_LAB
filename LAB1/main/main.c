
#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED_PIN 27

void app_main(void)
{
    esp_rom_gpio_pad_select_gpio(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    int ON = 0;
    while (true)
    {
        if (ON)
        {
            printf("\nLED IS ON");
        }
        else
        {
            printf("\nLED IS OFF");
        }
        ON = !ON;
        gpio_set_level(LED_PIN, ON);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}