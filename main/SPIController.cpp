#include "SPIController.hpp"

SPIController::SPIController(gpio_num_t mosi_pin, gpio_num_t miso_pin, gpio_num_t sclk_pin, gpio_num_t cs_pin, uint32_t clk_speed)
{
    this->mosi_pin = mosi_pin;
    this->miso_pin = miso_pin;
    this->sclk_pin = sclk_pin;
    this->cs_pin = cs_pin;
    this->clk_speed = clk_speed;
}

SPIController::~SPIController()
{
    spi_bus_free(HOST);
}

esp_err_t SPIController::begin() {
    spi_bus_config_t devbus;
    devbus.mosi_io_num = this->mosi_pin;
    devbus.miso_io_num = this->miso_pin;
    devbus.sclk_io_num = this->sclk_pin;
    devbus.quadwp_io_num = -1;
    devbus.quadhd_io_num = -1;
    devbus.max_transfer_sz = 32;

    esp_err_t ret = spi_bus_initialize((spi_host_device_t)HOST, &devbus, SPI_DMA_CH_AUTO);

    return ret;
}

esp_err_t SPIController::readByte(uint8_t *rx_buffer, uint8_t reg, bool restart)
{
    return this->read(rx_buffer, reg, 1, restart);
}

esp_err_t SPIController::readWord(uint8_t *rx_buffer, uint8_t reg, bool restart)
{
    return this->read(rx_buffer, reg, 4, restart);
}

esp_err_t SPIController::read(uint8_t *rx_buffer, uint8_t reg, uint8_t len, bool restart)
{
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));       //Zero out the transaction
    t.length = (len + 1) * 8;                 //Len is in bytes, transaction length is in bits.
    t.flags = SPI_TRANS_USE_RXDATA;
    t.tx_buffer = &reg;               //Data
    t.rx_buffer = rx_buffer;               //Data
    //ret = spi_device_transmit(HOST, &t);  //Transmit!
    return ESP_OK;
}

esp_err_t SPIController::writeByte(uint8_t *tx_buffer, uint8_t reg)
{
    return this->write(tx_buffer, reg, 1);
}

esp_err_t SPIController::writeWord(uint8_t *tx_buffer, uint8_t reg)
{
    return this->write(tx_buffer, reg, 4);
}

esp_err_t SPIController::write(uint8_t *tx_buffer, uint8_t reg, uint8_t len)
{
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));       //Zero out the transaction
    t.length = (len + 1) * 8;                 //Len is in bytes, transaction length is in bits.
    t.flags = 0;
    t.tx_buffer = tx_buffer;               //Data
    //ret = spi_device_transmit(HOST, &t);  //Transmit!
    return ESP_OK;
}