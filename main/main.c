#include "esp_log.h"
#include <stdio.h>
#include "veml7700.h"

void ConfigureSensor(VEML7700Conf *conf)
{
    conf->als_gain = ALS_GAIN_1_8;
    conf->als_it = ALS_IT_100MS;
    conf->als_pers = ALS_PERS_1;
    conf->als_int_en = ALS_INT_DISABLE;
    conf->als_sd = ALS_SD_ENABLE;
    conf->resolution = 0;
    conf->maximum_lux = 32000;
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
    bus->clk_speed = 400000;
    BusControllerInit(bus);
    VEML7700Conf *conf = malloc(sizeof(VEML7700Conf));
    ConfigureSensor(conf);
    float lux = 0;
    float white = 0;

    VEML7700Init(bus, conf);
    while(1) {
        esp_err_t ret = VEML7700ReadAlsLux(bus, &lux);
        if (ret == ESP_OK) {
            ESP_LOGI("main", "Lux: %f", lux);
        } else {
            ESP_LOGE("main", "Error reading lux");
        }

        esp_err_t retw = VEML7700ReadAlsWhite(bus, &white);
        if (ret == ESP_OK) {
            ESP_LOGI("main", "White: %f", white);
        } else {
            ESP_LOGE("main", "Error reading white");
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
