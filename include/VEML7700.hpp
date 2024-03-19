#ifndef VEML7700_HPP
#define VEML7700_HPP

#include <driver/gpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "I2CController.hpp"
#include "esp_log.h"
#include "esp_err.h"

#define VEML7700_ADDR UINT8_C(0x10)
#define LUX_FC_COEF 0.092903
#define FC_LUX_COEF 10.76391

// Register addresses
#define ALS_CONF 0x00
#define ALS_THDH 0x01
#define ALS_THDL 0x02
#define ALS_PERS 0x03
#define ALS 0x04
#define WHITE 0x05
#define POWER_SAVE 0x06

// ALS_CONF bits
#define ALS_IT_25MS 0x0C
#define ALS_IT_50MS 0x08
#define ALS_IT_100MS 0x00
#define ALS_IT_200MS 0x01
#define ALS_IT_400MS 0x02
#define ALS_IT_800MS 0x03

// ALS gain
#define ALS_GAIN_1 0x00
#define ALS_GAIN_2 0x01
#define ALS_GAIN_1_8 0x02
#define ALS_GAIN_1_4 0x03

// ALS persistence
#define ALS_PERS_1 0x00
#define ALS_PERS_2 0x01
#define ALS_PERS_4 0x02
#define ALS_PERS_8 0x03

// ALS_INT_EN
#define ALS_INT_DISABLE 0x00
#define ALS_INT_ENABLE 0x01

// ALS SD
#define ALS_SD_ENABLE 0x00
#define ALS_SD_DISABLE 0x01

// Power save mode
#define VEML7700_POWER_SAVE_MODE1 0x00
#define VEML7700_POWER_SAVE_MODE2 0x01
#define VEML7700_POWER_SAVE_MODE3 0x02
#define VEML7700_POWER_SAVE_MODE4 0x03

#define VEML7700_GAIN_OPTIONS_COUNT 4	/*!< Possible gain values count */
#define VEML7700_IT_OPTIONS_COUNT 6		/*!< Possible integration time values count */

class VEML7700
{
private:
    I2CController* m_bus;

    uint8_t m_gain;
    uint8_t m_integration_time;
    uint8_t m_persistence;
    uint8_t m_power_save_mode;
    uint8_t m_als_int_en;
    uint8_t m_als_sd;
    float m_resolution;
    uint32_t m_maximum_lux;
    
    gpio_num_t sda_pin;
    gpio_num_t scl_pin;

    /**
     * @brief List of all possible values for configuring sensor gain.
     * 
     */
    const uint8_t gain_values[VEML7700_GAIN_OPTIONS_COUNT] = {
        ALS_GAIN_2,
        ALS_GAIN_1,
        ALS_GAIN_1_4,
        ALS_GAIN_1_8
    };
    /**
     * @brief List of all possible values for configuring sensor integration time.
     * 
     */
    const uint8_t integration_time_values[VEML7700_IT_OPTIONS_COUNT] = {
        ALS_IT_800MS,
        ALS_IT_400MS,
        ALS_IT_200MS,
        ALS_IT_100MS,
        ALS_IT_50MS,
        ALS_IT_25MS
    };

    const float resolution_map[VEML7700_IT_OPTIONS_COUNT][VEML7700_GAIN_OPTIONS_COUNT] = {
        { 0.0036, 0.0072, 0.0288, 0.0576 },
        { 0.0072, 0.0144, 0.0576, 0.1152 },
        { 0.0288, 0.0576, 0.2304, 0.4608 },
        { 0.0576, 0.1152, 0.4608, 0.9216 },
        { 0.1152, 0.2304, 0.9216, 1.8432 },
        { 0.2304, 0.4608, 1.8432, 3.6864 }
    };

    const uint32_t maximums_map[VEML7700_IT_OPTIONS_COUNT][VEML7700_GAIN_OPTIONS_COUNT] = {
        {236, 472, 1887, 3775},
        {472, 944, 3775, 7550},
        {944, 1887, 7550, 15099},
        {1887, 3775, 15099, 30199},
        {3775, 7550, 30199, 60398},
        {7550, 15099, 60398, 120796}
    };

    void sendConfiguration();
    float getResolution();
    uint32_t getCurrentMaximumLux();
    uint8_t getGainIndex(uint8_t gain);
    uint8_t getItIndex(uint8_t integration_time);
    uint8_t indexOf(uint8_t element, const uint8_t *array, uint8_t array_size);

public:

    /**
     * @brief Construct a new VEML7700 object. Create a configuration with default values.
     * 
     * @param sda 
     * @param scl 
     * @param addr 
     */
    VEML7700(gpio_num_t sda, gpio_num_t scl, uint8_t addr = VEML7700_ADDR) {
        this->m_gain = ALS_GAIN_1;
        this->m_integration_time = ALS_IT_100MS;
        this->m_persistence = ALS_PERS_1;
        this->m_power_save_mode = VEML7700_POWER_SAVE_MODE1;
        this->m_als_int_en = ALS_INT_DISABLE;
        this->m_als_sd = ALS_SD_ENABLE;

        this->sda_pin = sda;
        this->scl_pin = scl;

        this->m_bus = I2CController::getInstance();
    }

    /**
     * @brief Destroy the VEML7700 object
     * 
     */
    ~VEML7700();

    /**
     * @brief Send the configuration to the sensor and start it
     * 
     * @return true 
     * @return false 
     */
    bool begin();

    /**
     * @brief Get the Gain object
     * 
     * @return uint8_t 
     */
    uint8_t getGain();

    /**
     * @brief Get the Integration Time object
     * 
     * @return uint8_t 
     */
    uint8_t getIntegrationTime();

    /**
     * @brief Get the Persistence object
     * 
     * @return uint8_t 
     */
    uint8_t getPersistence();

    /**
     * @brief Get the Power Save Mode object
     * 
     * @return uint8_t 
     */
    uint8_t getPowerSaveMode();

    /**
     * @brief Get the Als Int En object
     * 
     * @return uint8_t 
     */
    uint8_t getAlsIntEn();

    /**
     * @brief Get the Als Sd object
     * 
     * @return uint8_t 
     */
    uint8_t getAlsSd();

    /**
     * @brief Set the Gain object
     * 
     * @param gain 
     */
    void setGain(uint8_t gain);

    /**
     * @brief Set the Integration Time object
     * 
     * @param integration_time 
     */
    void setIntegrationTime(uint8_t integration_time);

    /**
     * @brief Set the Persistence object
     * 
     * @param persistence 
     */
    void setPersistence(uint8_t persistence);

    /**
     * @brief Set the Power Save Mode object
     * 
     * @param power_save_mode 
     */
    void setPowerSaveMode(uint8_t power_save_mode);

    /**
     * @brief Set the Als Int En object
     * 
     * @param als_int_en 
     */
    void setAlsIntEn(uint8_t als_int_en);

    /**
     * @brief Set the Als Sd object
     * 
     * @param als_sd 
     */
    void setAlsSd(uint8_t als_sd);

    /**
     * @brief Read the ALS value from the sensor
     * 
     * @return float if data is valid, -1 otherwise
     */
    float readAlsLux();

    /**
     * @brief Read the white light value from the sensor
     * 
     * @return float if data is valid, -1 otherwise
     */
    float readAlsWhite();
};

#endif // VEML7700_H
