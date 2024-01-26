#ifndef I2C_CONTROLLER_H
#define I2C_CONTROLLER_H

#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "driver/i2c.h"

#define I2C_MASTER_SCL_IO           1      /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           0      /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              0                          /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ          400000                     /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000
#define DEBUG_I2C_CONTROLLER        1

static const char *TAG = "I2C_CONTROLLER";

typedef struct {
    uint8_t address;
    uint8_t sda_pin;
    uint8_t scl_pin;
    uint32_t clk_speed;
} i2c_data_t;

/**
 * @brief Construct a new i2ccontroller init object
 * 
 */
esp_err_t I2CControllerInit(i2c_data_t *this, uint8_t address, uint8_t sda_pin, uint8_t scl_pin, uint32_t clk_speed);

/**
 * @brief Read a byte from a register
 * 
 * @param this the i2c_data_t object
 * @param rx_buffer data buffer to store the read data
 * @param reg the register to read from
 * @return esp_err_t 
 */
esp_err_t I2CControllerReadByte(i2c_data_t *this, uint8_t *rx_buffer, uint8_t reg, uint8_t restart);

/**
 * @brief Read a word (4 bytes) from a register
 * 
 * @param this the i2c_data_t object
 * @param rx_buffer data buffer to store the read data
 * @param reg the register to read from
 * @return esp_err_t 
 */
esp_err_t I2CControllerReadWord(i2c_data_t *this, uint8_t *rx_buffer, uint8_t reg, uint8_t restart);

/**
 * @brief read from a register with a given length
 * 
 * @param this the i2c_data_t object
 * @param rx_buffer data buffer to store the read data
 * @param reg the register to read from
 * @param len the length of the data to read
 * @return esp_err_t 
 */
esp_err_t I2CControllerRead(i2c_data_t *this, uint8_t *rx_buffer, uint8_t reg, uint8_t len, uint8_t restart);

/**
 * @brief write a byte to a register
 * 
 * @param data the i2c_data_t object
 * @param tx_buffer data buffer to write
 * @param reg the register to write to
 * @return esp_err_t 
 */
esp_err_t I2CControllerWriteByte(i2c_data_t *this, uint8_t *tx_buffer, uint8_t reg);

/**
 * @brief Write a word (4 bytes) to a register
 * 
 * @param data write a word to a register
 * @param tx_buffer data buffer to write
 * @param reg the register to write to
 * @return esp_err_t 
 */
esp_err_t I2CControllerWriteWord(i2c_data_t *this, uint8_t *tx_buffer, uint8_t reg);

/**
 * @brief Write to a register with a given length
 * 
 * @param data the i2c_data_t object
 * @param tx_buffer data buffer to write
 * @param reg the register to write to
 * @param len the length of the data to write
 * @return esp_err_t 
 */
esp_err_t I2CControllerWrite(i2c_data_t *this, uint8_t *tx_buffer, uint8_t reg, uint8_t len);

/**
 * @brief Deinitialize the I2C controller
 * 
 * @return esp_err_t 
 */
esp_err_t I2CControllerDeinit(void);

#endif