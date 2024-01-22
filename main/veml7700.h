#ifndef VEML7700_H
#define VEML7700_H

#include <stdint.h>
#include "BusController.h"
#include "esp_log.h"

#define VEML7700_ADDR 0x10

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

const float MAX_RES = 0.0036;
const float GAIN_MAX = 2;
const float IT_MAX = 800;

struct veml7700_conf
{
    uint8_t als_gain;
    uint8_t als_it;
    uint8_t als_pers;
    uint8_t als_int_en;
    uint8_t als_sd;
};

struct veml7700_data
{
    uint16_t als;
    uint16_t white;
    float als_white;
    float lux;
};

typedef enum {
  VEML_LUX_NORMAL,
  VEML_LUX_CORRECTED,
  VEML_LUX_AUTO,
  VEML_LUX_NORMAL_NOWAIT,
  VEML_LUX_CORRECTED_NOWAIT
} LuxMethod;

/**
 * @brief Initialize the sensor
 * 
 * @param bus the bus controller to use
 * @return esp_err_t 
 */
uint8_t VEML7700Init(BusController *bus);

/**
 * @brief set the configuration of the sensor
 * 
 * @param conf the configuration to set
 * @return esp_err_t 
 */
void VEMLSetConf(struct veml7700_conf *conf);

/**
 * @brief Read the configuration from the sensor
 * 
 * @param conf buffer to store the configuration
 * @return esp_err_t 
 */
veml7700_conf VEMLGetRead(struct veml7700_conf *conf);

/**
 * @brief Read the ALS values from the sensor
 * 
 * @param lux buffer to store the lux value
 * @return esp_err_t 
 */
void VEML7700GetLux(float lux);

/**
 * @brief Read the white values from the sensor
 * 
 * @param white buffer to store the white value
 * @return esp_err_t 
 */
void VEML7700GetWhite(float white);

/**
 * @brief Read the ALS and white values from the sensor
 * 
 * @param als_white buffer to store the als_white value
 * @return esp_err_t 
 */
void VEML7700AlsWhite(float *als_white);

/**
 * @brief Get the power saver mode conf
 * 
 * @return true if the power save mode is enabled
 * @return false if the power save mode is disabled
 */
bool VEML7700powerSaveEnable();

/**
 * @brief Set the power saver mode
 * 
 * @param mode mode to set
 */
void VEML7700SetPowerSaveMode(uint8_t mode);

/**
 * @brief Set the high threshold
 * 
 * @param threshold 
 */
void VEML7700SetHighThreshold(uint16_t threshold);

/**
 * @brief Set low threshold
 * 
 * @param threshold 
 */
void VEML7700SetLowThreshold(uint16_t threshold);

/**
 * @brief Set persistence mode
 * 
 * @param pers 
 */
void VEML7700SetPersistence(uint8_t pers);

/**
 * @brief Set interrupt enable
 * 
 * @param enable 
 */
void VEML7700SetInterruptEnable(uint8_t enable);

/**
 * @brief Get the high threshold
 * 
 * @param threshold 
 */
void VEML7700GetHighThreshold(uint16_t threshold);


/**
 * @brief Get the low threshold
 * 
 * @param threshold 
 */
void VEML7700GetLowThreshold(uint16_t threshold);

/**
 * @brief Get the persistence
 * 
 * @param pers 
 */
void VEML7700GetPersistence(uint8_t pers);

/**
 * @brief Get the interrupt enable
 * 
 * @param enable 
 */
void VEML7700GetInterruptEnable(uint8_t enable);

/**
 * @brief Get the integration time
 * 
 * @param it 
 */
void VEML7700GetResolution(uint8_t resolution);

/**
 * @brief Get the integration time
 * 
 * @param it 
 */
float VEML7700ComputeLux(float raw_als, uint8_t is_corrected);

/**
 * @brief Get the integration time
 * 
 * @param it 
 */
void VEML7700SetGain(uint8_t gain);

/**
 * @brief Get the integration time
 * 
 * @param it 
 */
void VEML7700SetIntegrationTime(uint8_t it);

/**
 * @brief Get the integration time
 * 
 * @param it 
 */
void VEML7700SetResolution(uint8_t resolution);

/**
 * @brief Get the integration time
 * 
 * @param it 
 */
void VEML7700GetGain(uint8_t gain);

/**
 * @brief Get the integration time
 * 
 * @param it 
 */
void VEML7700Enable(uint8_t enable);

/**
 * @brief Get the integration time
 * 
 * @param it 
 */
void VEML7700GetIntegrationTime(uint8_t it);

#endif // VEML7700_H