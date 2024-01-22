#include <stdio.h>
#include "veml7700.h"

void app_main(void)
{
    // Initialize the sensor
    BusController *bus = NULL;
    while(1) {
        VEML7700Init(bus);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    // Read the ALS and white values from the sensor
}
