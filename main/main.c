#include <stdio.h>
#include "veml7700.h"

void ConfigureSensor(VEML7700Conf *conf)
{
    conf->als_gain = VEML7700_GAIN_1_8;
    conf->als_it = VEML7700_IT_100MS;
    conf->als_pers = VEML7700_PERS_1;
    conf->als_int_en = VEML7700_INT_EN_1;
    conf->als_sd = VEML7700_SD_DISABLE;
}

void app_main(void)
{
    // Initialize the sensor
    BusController *bus = malloc(sizeof(BusController));
    // setup the bus controller for i2c on pin 0 (sda) and 1 (scl)
    bus->bus_type = I2C;
    bus->sda_pin = 0;
    bus->scl_pin = 1;
    bus->address = 0x10;
    bus->clk_speed = 100000;
    BusControllerInit(bus);
    VEML7700Conf *conf = malloc(sizeof(VEML7700Conf));
    ConfigureSensor(conf);

    VEML7700Init(bus, conf);

    vTaskDelay(2000 / portTICK_PERIOD_MS);
}
