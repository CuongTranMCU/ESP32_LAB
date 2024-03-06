#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/task.h"
#include "freertos/FreeRTOS.h"
#define LED_PIN 13
void app_main(void)
{
    gpio_set_direction(LED_PIN,GPIO_MODE_OUTPUT);
    bool ON = 0;
    while (true)
    {
        ON = !ON;
        gpio_set_level(LED_PIN,ON);
        if(ON)
        {
            printf("LED IS ON!!!");
        }
        else 
        {
            printf("LED IS OFF!!!");
        }
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
    
}
