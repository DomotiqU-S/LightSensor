#include "veml7700.h"

/**
 * @brief List of all possible values for configuring sensor gain.
 * 
 */
static const uint8_t gain_values[VEML7700_GAIN_OPTIONS_COUNT] = {
    VEML7700_GAIN_2,
    VEML7700_GAIN_1,
    VEML7700_GAIN_1_8,
    VEML7700_GAIN_1_4
};
/**
 * @brief List of all possible values for configuring sensor integration time.
 * 
 */
static const uint8_t integration_time_values[VEML7700_IT_OPTIONS_COUNT] = {
    VEML7700_IT_800MS,
    VEML7700_IT_400MS,
    VEML7700_IT_200MS,
    VEML7700_IT_100MS,
    VEML7700_IT_50MS,
    VEML7700_IT_25MS
};

static const float resolution_map[VEML7700_IT_OPTIONS_COUNT][VEML7700_GAIN_OPTIONS_COUNT] = {
    { 0.0036, 0.0072, 0.0288, 0.0576 },
    { 0.0072, 0.0144, 0.0576, 0.1152 },
    { 0.0288, 0.0576, 0.2304, 0.4608 },
    { 0.0576, 0.1152, 0.4608, 0.9216 },
    { 0.1152, 0.2304, 0.9216, 1.8432 },
    { 0.2304, 0.4608, 1.8432, 3.6864 }
};

static const uint32_t maximums_map[VEML7700_IT_OPTIONS_COUNT][VEML7700_GAIN_OPTIONS_COUNT] = {
    {236, 472, 1887, 3775},
    {472, 944, 3775, 7550},
    {944, 1887, 7550, 15099},
    {1887, 3775, 15099, 30199},
    {3775, 7550, 30199, 60398},
    {7550, 15099, 60398, 120796}
};

VEML7700Conf *priv_conf;

uint8_t VEML7700Init(BusController *this, VEML7700Conf *conf) {
    priv_conf = malloc(sizeof(VEML7700Conf));
    VEML7700SetConfig(this, conf);
    return 1;
}

esp_err_t VEML7700SendConfiguration(BusController *this, VEML7700Conf *conf)
{
	uint16_t config_data = ( 
		(conf->als_gain << 11) |
		(conf->als_it << 6) |
		(conf->als_pers << 4) |
		(conf->als_int_en << 1) |
		(conf->als_sd << 0)
	);

    conf->als_gain = VEML7700GetResolution(this, conf);
    conf->maximum_lux = VEML7700GetCurrentMaximumLux(this, conf);

    return BusControllerWrite(this, VEML7700_ALS_CONF, &config_data, 2);
}

float VEML7700GetResolution(BusController *this, VEML7700Conf *conf) {
    int gain_index = VEML7700GetGainIndex(conf->als_gain);
    int it_index = VEML7700GetItIndex(conf->als_it);

    return resolution_map[it_index][gain_index];
}

static int VEML7700GetGainIndex(uint8_t gain) {
    return indexOf(gain, gain_values, VEML7700_GAIN_OPTIONS_COUNT);
}

static int VEML7700GetItIndex(uint8_t it) {
    return indexOf(it, integration_time_values, VEML7700_IT_OPTIONS_COUNT);
}

static uint8_t indexOf(uint8_t element, const uint8_t *array, uint8_t array_size) {
    for (int i = 0; i < array_size; i++) {
        if (array[i] == element) {
            return i;
        }
    }
    return -1;
}

static VEML7700GetCurrentMaximumLux(BusController *this, VEML7700Conf *conf) {
    int gain_index = VEML7700GetGainIndex(conf->als_gain);
    int it_index = VEML7700GetItIndex(conf->als_it);

    return maximums_map[it_index][gain_index];
}

void VEML7700SetConfig(BusController *this, VEML7700Conf *conf) {
    priv_conf->als_gain = conf->als_gain;
    priv_conf->als_it = conf->als_it;
    priv_conf->als_pers = conf->als_pers;
    priv_conf->als_int_en = conf->als_int_en;
    priv_conf->als_sd = conf->als_sd;
    priv_conf->resolution = conf->resolution;
    priv_conf->maximum_lux = conf->maximum_lux;
}
