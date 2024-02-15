#ifndef VEML7700_H
#define VEML7700_H

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
    I2CController m_bus;

    uint8_t m_gain;
    uint8_t m_integration_time;
    uint8_t m_persistence;
    uint8_t m_power_save_mode;
    uint8_t m_als_int_en;
    uint8_t m_als_sd;
    float m_resolution;
    uint32_t m_maximum_lux;

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
    VEML7700(gpio_num_t sda, gpio_num_t scl, uint8_t addr = VEML7700_ADDR) : m_bus(addr, sda, scl) {
        this->m_gain = ALS_GAIN_1;
        this->m_integration_time = ALS_IT_100MS;
        this->m_persistence = ALS_PERS_1;
        this->m_power_save_mode = VEML7700_POWER_SAVE_MODE1;
        this->m_als_int_en = ALS_INT_DISABLE;
        this->m_als_sd = ALS_SD_ENABLE;

        esp_err_t ret = m_bus.begin();
        sendConfiguration();
    }
    ~VEML7700();
    uint8_t getGain();
    uint8_t getIntegrationTime();
    uint8_t getPersistence();
    uint8_t getPowerSaveMode();
    uint8_t getAlsIntEn();
    uint8_t getAlsSd();
    void setGain(uint8_t gain);
    void setIntegrationTime(uint8_t integration_time);
    void setPersistence(uint8_t persistence);
    void setPowerSaveMode(uint8_t power_save_mode);
    void setAlsIntEn(uint8_t als_int_en);
    void setAlsSd(uint8_t als_sd);
    float readAlsLux();
    float readAlsWhite();
};

// /**
//  * @brief Initialize the sensor
//  * 
//  * @param bus the bus controller to use
//  * @return esp_err_t 
//  */
// esp_err_t VEML7700Init(BusController *this, VEML7700Conf *conf);

// /**
//  * @brief Deinitialize the sensor
//  * 
//  * @param this the bus controller to use
//  */
// void VEML7700Deinit(BusController *this);

// /**
//  * @brief set the configuration of the sensor
//  * 
//  * @param this the bus controller to use
//  * @param conf the configuration to set
//  * @return esp_err_t 
//  */
// void VEML7700SetConfig(BusController *this, VEML7700Conf *conf);

// /**
//  * @brief Read the configuration of the sensor
//  * 
//  * @param this 
//  * @param data 
//  * @return esp_err_t 
//  */
// esp_err_t VEML7700ReadAlsLux(BusController *this, float *lux);

// /**
//  * @brief 
//  * 
//  * @param this 
//  * @param data 
//  * @return esp_err_t 
//  */
// esp_err_t VEML7700ReadAlsWhite(BusController *this, float *white);

// /**
//  * @brief 
//  * 
//  * @param this 
//  * @return float 
//  */
// float VEML7700GetResolution(BusController *this, VEML7700Conf *conf);

// /**
//  * @brief 
//  * 
//  * @param this 
//  * @return esp_err_t 
//  */
// esp_err_t VEML7700ReadAlsLuxAuto(BusController *this, float *lux);

// /**
//  * @brief Rean the white value for the current configuration
//  * 
//  * @param this 
//  * @return esp_err_t 
//  */
// esp_err_t VEML7700ReadAlsWhiteAuto(BusController *this);

// /**
//  * @brief Optimize the configuration of the sensor
//  * 
//  * @param this 
//  * @param lux
//  * @return esp_err_t 
//  */
// esp_err_t VEML7700OptimizeConfiguration(BusController *this, float *lux);

// /**
//  * @brief Get the current maximum lux value
//  * 
//  * @param this 
//  * @return uint32_t 
//  */
// uint32_t VEML7700GetCurrentMaximumLux(BusController *this);

// /**
//  * @brief Get the lower value in the lux range
//  * 
//  * @param this BusIO
//  * @return uint32_t 
//  */
// uint32_t VEML7700GetLowerLux(BusController *this);

// /**
//  * @brief Get the lowest maximum lux value
//  * 
//  * @param this 
//  * @return uint32_t 
//  */
// uint32_t VEML7700GetLowestMaxLux(BusController *this);

// /**
//  * @brief Get the maximum lux value from the configuration
//  * 
//  * @param this 
//  * @return uint32_t 
//  */
// uint32_t VEML7700GetMaxLux(BusController *this);

// /**
//  * @brief Get the index of the gain in the gain array
//  * 
//  * @param gain 
//  * @return int 
//  */
// int VEML7700GetGainIndex(uint8_t gain);

// /**
//  * @brief Get the index of the integration time in the integration time array
//  * 
//  * @param integration_time 
//  * @return int 
//  */
// int VEML7700GetItIndex(uint8_t integration_time);

// /**
//  * @brief Get the index of an element in an array
//  * 
//  * @param element 
//  * @param array 
//  * @param array_size 
//  * @return uint8_t 
//  */
// uint8_t indexOf(uint8_t element, const uint8_t *array, uint8_t array_size);

// /**
//  * @brief Decrease the actual resolution of the sensor
//  * 
//  * @param this 
//  */
// void VEML7700DecreaseResolution(BusController *this);

// /**
//  * @brief Increase the actual resolution of the sensor
//  * 
//  * @param this 
//  */
// void VEML7700IncreaseResolution(BusController *this);

// /**
//  * @brief Send the configuration to the sensor
//  * 
//  * @param this 
//  * @param conf 
//  * @return esp_err_t 
//  */
// esp_err_t VEML7700SendConfiguration(BusController *this, VEML7700Conf *conf);

#endif // VEML7700_H