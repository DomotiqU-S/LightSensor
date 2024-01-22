#include "SPIController.h"

esp_err_t SPIControllerInit(spi_controller_t *spi_controller, int mosi_pin, int miso_pin, int sclk_pin, int cs_pin) {
    esp_err_t ret;
    spi_bus_config_t buscfg={
        .miso_io_num = miso_pin,
        .mosi_io_num = mosi_pin,
        .sclk_io_num = sclk_pin,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 32,
    };
    //Initialize the SPI bus
    ret = spi_bus_initialize(HOST, &buscfg, SPI_DMA_CH_AUTO);
    return ret;
}

esp_err_t SPIControllerDeinit(spi_controller_t *spi_controller) {
    esp_err_t ret;
    ret = spi_bus_free(HOST);
    return ret;
}

esp_err_t SPIControllerWrite(spi_controller_t *spi_controller, uint8_t *data, size_t len) {
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));       //Zero out the transaction
    t.length = len * 8;                 //Len is in bytes, transaction length is in bits.
    t.tx_buffer = data;               //Data
    ret = spi_device_transmit(spi_controller->spi, &t);  //Transmit!
    return ret;
}

esp_err_t SPIControllerRead(spi_controller_t *spi_controller, uint8_t *data, size_t len) {
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));       //Zero out the transaction
    t.length = len * 8;                 //Len is in bytes, transaction length is in bits.
    t.rx_buffer = data;               //Data
    ret = spi_device_transmit(spi_controller->spi, &t);  //Transmit!
    return ret;
}

esp_err_t SPIControllerWriteRead(spi_controller_t *spi_controller, uint8_t *tx_data, uint8_t *rx_data, size_t rx_len, size_t tx_len) {
    esp_err_t ret;
    ret = SPIControllerWrite(spi_controller, tx_data, tx_len);
    if (ret != ESP_OK) {
        return ret;
    }
    ret = SPIControllerRead(spi_controller, rx_data, rx_len);
    return ret;
}