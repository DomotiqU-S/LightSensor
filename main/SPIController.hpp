#ifndef SPI_CONTROLLER_H
#define SPI_CONTROLLER_H

#include "BusController.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define HOST SPI1_HOST

class SPIController : public BusController
{
private:
    gpio_num_t mosi_pin;
    gpio_num_t miso_pin;
    gpio_num_t sclk_pin;
    gpio_num_t cs_pin;
    uint32_t clk_speed;
    spi_device_handle_t spi;
public:
    SPIController(gpio_num_t mosi_pin, gpio_num_t miso_pin, gpio_num_t sclk_pin, gpio_num_t cs_pin, uint32_t clk_speed = 1000000);
    ~SPIController();
    esp_err_t begin();
    esp_err_t readByte(uint8_t *rx_buffer, uint8_t reg, bool restart = false);
    esp_err_t readWord(uint8_t *rx_buffer, uint8_t reg, bool restart = false);
    esp_err_t read(uint8_t *rx_buffer, uint8_t reg, uint8_t len, bool restart = false);
    esp_err_t writeByte(uint8_t *tx_buffer, uint8_t reg);
    esp_err_t writeWord(uint8_t *tx_buffer, uint8_t reg);
    esp_err_t write(uint8_t *tx_buffer, uint8_t reg, uint8_t len);
};

#endif