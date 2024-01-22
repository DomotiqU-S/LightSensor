#ifndef BUSCONTROLLER_H
#define BUSCONTROLLER_H

#include "esp_log.h"
#include "I2CController.h"
#include "SPIController.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Define the bus type
 * 
 */
enum BusType {
    I2C,
    SPI
};

/**
 * @brief Define the bus controller struct
 * 
 */
struct BusController {
    i2c_data_t i2c_controller;
    spi_controller_t spi_controller;
    uint8_t address;
    enum BusType bus_type;
    uint8_t sda_pin;
    uint8_t scl_pin;
    uint32_t clk_speed;
    uint8_t mosi_pin;
    uint8_t miso_pin;
    uint8_t sclk_pin;
    uint8_t cs_pin;
};

/**
 * @brief Initialize the bus controller
 * 
 * @param bus_controller 
 * @return esp_err_t 
 */
esp_err_t BusControllerInit(struct BusController *bus_controller);

/**
 * @brief Deinitialize the bus controller
 * 
 * @param bus_controller 
 * @return esp_err_t 
 */
esp_err_t BusControllerDeinit(struct BusController *bus_controller);

/**
 * @brief Write data to the bus controller
 * 
 * @param bus_controller the bus controller self
 * @param cmd the command to write for I2C is register address
 * @param data buffer to write
 * @param data_size size of buffer
 * @return esp_err_t 
 */
esp_err_t BusControllerWrite(struct BusController *bus_controller, uint8_t cmd, uint8_t *data, size_t data_size);

/**
 * @brief read data from the bus controller
 * 
 * @param bus_controller the bus controller self
 * @param cmd the command to write for I2C is register address
 * @param data buffer to read into
 * @param data_size size of buffer
 * @return esp_err_t 
 */
esp_err_t BusControllerRead(struct BusController *bus_controller, uint8_t cmd, uint8_t *data, size_t data_size);

#endif