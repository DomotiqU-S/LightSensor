#include <stdio.h>
#include "veml7700.h"

void ConfigureSensor(VEML7700Conf *conf)
{
    conf->als_gain = ALS_GAIN_1_8;
    conf->als_it = ALS_IT_100MS;
    conf->als_pers = ALS_PERS_1;
    conf->als_int_en = ALS_INT_DISABLE;
    conf->als_sd = ALS_SD_ENABLE;
}

void app_main(void)
{
    // Initialize the sensor
    BusController *bus = malloc(sizeof(BusController));
    // setup the bus controller for i2c on pin 0 (sda) and 1 (scl)
    bus->bus_type = I2C;
    bus->sda_pin = 1;
    bus->scl_pin = 0;
    bus->address = 0x10;
    bus->clk_speed = 100000;
    BusControllerInit(bus);
    VEML7700Conf *conf = malloc(sizeof(VEML7700Conf));
    VEML7700Data *data = malloc(sizeof(VEML7700Data));
    ConfigureSensor(conf);

    VEML7700Init(bus, conf);
    vTaskDelay(3000);
    while(1) {
        VEML7700ReadAlsLux(bus, data);
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}
