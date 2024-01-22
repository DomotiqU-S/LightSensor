#ifndef SPI_CONTROLLER_H
#define SPI_CONTROLLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define HOST SPI1_HOST

typedef struct {
    spi_device_handle_t spi;
    spi_bus_config_t bus_config;
    spi_device_interface_config_t dev_config;
} spi_controller_t;

/**
 * @brief Initialize SPI controller
 * 
 * @param spi_controller The SPI controller
 * @param mosi_pin Master Out Slave In pin
 * @param miso_pin Master In Slave Out pin
 * @param sclk_pin Clock pin
 * @param cs_pin Device select pin
 */
esp_err_t SPIControllerInit(spi_controller_t *spi_controller, int mosi_pin, int miso_pin, int sclk_pin, int cs_pin);

/**
 * @brief Deinitialize SPI controller
 * 
 * @param spi_controller The SPI controller 
 */
esp_err_t SPIControllerDeinit(spi_controller_t *spi_controller);

/**
 * @brief Write data to SPI controller
 * 
 * @param spi_controller The SPI controller
 * @param data buffer to write
 * @param len size of buffer
 */
esp_err_t SPIControllerWrite(spi_controller_t *spi_controller, uint8_t *data, size_t len);

/**
 * @brief Read data from SPI controller
 * 
 * @param spi_controller The SPI controller
 * @param data buffer to read into
 * @param len size of buffer
 */
esp_err_t SPIControllerRead(spi_controller_t *spi_controller, uint8_t *data, size_t len);

/**
 * @brief Write and read data from SPI controller
 * 
 * @param spi_controller The SPI controller
 * @param tx_data buffer to write
 * @param rx_data buffer to read into
 * @param rx_len size of read buffer
 * @param tx_len size of write buffer
 * @return esp_err_t 
 */
esp_err_t SPIControllerWriteRead(spi_controller_t *spi_controller, uint8_t *tx_data, uint8_t *rx_data, size_t rx_len, size_t tx_len);

#endif