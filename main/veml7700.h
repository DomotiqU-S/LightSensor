#ifndef VEML7700_H
#define VEML7700_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "BusController.h"
#include "esp_log.h"
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

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

typedef struct
{
    uint8_t als_gain;
    uint8_t als_it;
    uint8_t als_pers;
    uint8_t als_int_en;
    uint8_t als_sd;
    float resolution;
    uint32_t maximum_lux;
} VEML7700Conf;
/**
 * @brief Initialize the sensor
 * 
 * @param bus the bus controller to use
 * @return esp_err_t 
 */
esp_err_t VEML7700Init(BusController *this, VEML7700Conf *conf);

/**
 * @brief Deinitialize the sensor
 * 
 * @param this the bus controller to use
 */
void VEML7700Deinit(BusController *this);

/**
 * @brief set the configuration of the sensor
 * 
 * @param this the bus controller to use
 * @param conf the configuration to set
 * @return esp_err_t 
 */
void VEML7700SetConfig(BusController *this, VEML7700Conf *conf);

/**
 * @brief Read the configuration of the sensor
 * 
 * @param this 
 * @param data 
 * @return esp_err_t 
 */
esp_err_t VEML7700ReadAlsLux(BusController *this, float *lux);

/**
 * @brief 
 * 
 * @param this 
 * @param data 
 * @return esp_err_t 
 */
esp_err_t VEML7700ReadAlsWhite(BusController *this, float *white);

/**
 * @brief 
 * 
 * @param this 
 * @return float 
 */
float VEML7700GetResolution(BusController *this, VEML7700Conf *conf);

/**
 * @brief 
 * 
 * @param this 
 * @return esp_err_t 
 */
esp_err_t VEML7700ReadAlsLuxAuto(BusController *this, float *lux);

/**
 * @brief 
 * 
 * @param this 
 * @return esp_err_t 
 */
esp_err_t VEML7700ReadAlsWhiteAuto(BusController *this);

esp_err_t VEML7700OptimizeConfiguration(BusController *this, float *lux);
uint32_t VEML7700GetCurrentMaximumLux(BusController *this);
uint32_t VEML7700GetLowerLux(BusController *this);
uint32_t VEML7700GetLowestMaxLux(BusController *this);
uint32_t VEML7700GetMaxLux(BusController *this);
int VEML7700GetGainIndex(uint8_t gain);
int VEML7700GetItIndex(uint8_t integration_time);
uint8_t indexOf(uint8_t element, const uint8_t *array, uint8_t array_size);
void VEML7700DecreaseResolution(BusController *this);
void VEML7700IncreaseResolution(BusController *this);
esp_err_t VEML7700SendConfiguration(BusController *this, VEML7700Conf *conf);

#ifdef __cplusplus
}
#endif // VEML7700_H

#endif // VEML7700_H