#include "BusController.h"

esp_err_t BusControllerInit(BusController *bus_controller) {
    esp_err_t ret;
    switch (bus_controller->bus_type) {
        case I2C:
            ret = I2CControllerInit(&bus_controller->i2c_controller, bus_controller->address, bus_controller->sda_pin, bus_controller->scl_pin, bus_controller->clk_speed);
            break;
        case SPI:
            ret = SPIControllerInit(&bus_controller->spi_controller, bus_controller->mosi_pin, bus_controller->miso_pin, bus_controller->sclk_pin, bus_controller->cs_pin);
            break;
        default:
            ESP_LOGE("BusController", "Invalid bus type");
            ret = ESP_FAIL;
            break;
    }
    return ret;
}

esp_err_t BusControllerDeinit(BusController *bus_controller) {
    esp_err_t ret;
    switch (bus_controller->bus_type) {
        case I2C:
            ret = I2CControllerDeinit();
            break;
        case SPI:
            ret = SPIControllerDeinit(&bus_controller->spi_controller);
            break;
        default:
            ESP_LOGE("BusController", "Invalid bus type");
            ret = ESP_FAIL;
            break;
    }
    return ret;
}

esp_err_t BusControllerWrite(BusController *bus_controller, uint8_t cmd, uint8_t *data, size_t data_size) {
    esp_err_t ret;
    switch (bus_controller->bus_type) {
        case I2C:
            ret = I2CControllerWrite(&bus_controller->i2c_controller, data, cmd, data_size);
            break;
        case SPI:
            ret = SPIControllerWrite(&bus_controller->spi_controller, data, data_size);
            break;
        default:
            ESP_LOGE("BusController", "Invalid bus type");
            ret = ESP_FAIL;
            break;
    }
    return ret;
}

esp_err_t BusControllerRead(BusController *bus_controller, uint8_t cmd, uint8_t *data, size_t data_size, uint8_t restart) {
    esp_err_t ret;
    switch (bus_controller->bus_type) {
        case I2C:
            ret = I2CControllerRead(&bus_controller->i2c_controller, data, cmd, data_size, 1);
            break;
        case SPI:
            ret = SPIControllerRead(&bus_controller->spi_controller, data, data_size);
            break;
        default:
            ESP_LOGE("BusController", "Invalid bus type");
            ret = ESP_FAIL;
            break;
    }
    return ret;
}