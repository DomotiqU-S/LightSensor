#include "esp_log.h"
#include <stdio.h>
#include <VEML7700.hpp> 

extern "C" void app_main(void)
{
    VEML7700 light_sensor(GPIO_NUM_1, GPIO_NUM_0);

    while(1) {
        float lux = light_sensor.readAlsLux();
        ESP_LOGI("LUX", "Lux: %f", lux);
        float white = light_sensor.readAlsWhite();
        ESP_LOGI("WHITE", "White: %f", white);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}