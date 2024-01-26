#include "I2CController.h"

esp_err_t I2CControllerInit(i2c_data_t *this, uint8_t address, uint8_t sda_pin, uint8_t scl_pin, uint32_t clk_speed)
{
    int i2c_master_port = I2C_MASTER_NUM;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = sda_pin,
        .scl_io_num = scl_pin,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = clk_speed,
    };

    i2c_param_config(i2c_master_port, &conf);

    this->address = address;
    this->scl_pin = scl_pin;
    this->sda_pin = sda_pin;

    #if DEBUG_I2C_CONTROLLER
        ESP_LOGI(TAG, "Initializing I2C controller");
    #endif

    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

esp_err_t I2CControllerDeinit(void)
{
    #if DEBUG_I2C_CONTROLLER
        ESP_LOGI(TAG, "Deinitializing I2C controller");
    #endif
    return i2c_driver_delete(I2C_MASTER_NUM);
}

esp_err_t I2CControllerWrite(i2c_data_t *this, uint8_t *tx_buffer, uint8_t reg, uint8_t len)
{
    int ret;
    uint8_t write_buffer[len + 1];
    
    // Fill the write buffer with the register address and data
    write_buffer[0] = reg;
    memcpy(write_buffer + 1, tx_buffer, len);

    #if DEBUG_I2C_CONTROLLER
        ESP_LOGI(TAG, "Writing %d bytes to register 0x%02x", len, reg);
    #endif
    ret = i2c_master_write_to_device(I2C_MASTER_NUM, this->address, write_buffer, len + 1, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    
    return ret;
}

esp_err_t I2CControllerRead(i2c_data_t *this, uint8_t *rx_buffer, uint8_t reg, uint8_t len, uint8_t restart)
{
    int ret;
    uint8_t write_buffer[1] = {reg};

    #if DEBUG_I2C_CONTROLLER
        ESP_LOGI(TAG, "Reading %d bytes from register 0x%02x", len, reg);
    #endif
    if (restart == 1) {
        ret = i2c_master_write_read_device(I2C_MASTER_NUM, this->address, write_buffer, 1, rx_buffer, len, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    }
    else {
        ret = i2c_master_write_to_device(I2C_MASTER_NUM, this->address, write_buffer, 1, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
        ret = i2c_master_read_from_device(I2C_MASTER_NUM, this->address, rx_buffer, len, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    }
    return ret;
}

esp_err_t I2CControllerReadByte(i2c_data_t *this, uint8_t *rx_buffer, uint8_t reg, uint8_t restart)
{
    return I2CControllerRead(this, rx_buffer, reg, 1, restart);
}

esp_err_t I2CControllerReadWord(i2c_data_t *this, uint8_t *rx_buffer, uint8_t reg, uint8_t restart)
{
    return I2CControllerRead(this, rx_buffer, reg, 4, restart);
}

esp_err_t I2CControllerWriteByte(i2c_data_t *this, uint8_t *tx_buffer, uint8_t reg)
{
    return I2CControllerWrite(this, tx_buffer, reg, 1);
}

esp_err_t I2CControllerWriteWord(i2c_data_t *this, uint8_t *tx_buffer, uint8_t reg)
{
    return I2CControllerWrite(this, tx_buffer, reg, 4);
}