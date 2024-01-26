#include "veml7700.h"

/**
 * @brief List of all possible values for configuring sensor gain.
 * 
 */
static const uint8_t gain_values[VEML7700_GAIN_OPTIONS_COUNT] = {
    ALS_GAIN_2,
    ALS_GAIN_1,
    ALS_GAIN_1_4,
    ALS_GAIN_1_8
};
/**
 * @brief List of all possible values for configuring sensor integration time.
 * 
 */
static const uint8_t integration_time_values[VEML7700_IT_OPTIONS_COUNT] = {
    ALS_IT_800MS,
    ALS_IT_400MS,
    ALS_IT_200MS,
    ALS_IT_100MS,
    ALS_IT_50MS,
    ALS_IT_25MS
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

esp_err_t VEML7700Init(BusController *this, VEML7700Conf *conf) {
    priv_conf = malloc(sizeof(VEML7700Conf));
    priv_conf = conf;
    VEML7700SetConfig(this, priv_conf);
    VEML7700SendConfiguration(this, priv_conf);
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

    uint8_t data[2] = { config_data >> 8, config_data & 0xFF};

    conf->resolution = VEML7700GetResolution(this, conf);
    conf->maximum_lux = VEML7700GetCurrentMaximumLux(this);

    return BusControllerWrite(this, ALS_CONF, data, 2);
}

float VEML7700GetResolution(BusController *this, VEML7700Conf *conf) {
    int gain_index = VEML7700GetGainIndex(conf->als_gain);
    int it_index = VEML7700GetItIndex(conf->als_it);

    return resolution_map[it_index][gain_index];
}

int VEML7700GetGainIndex(uint8_t gain) {
    return indexOf(gain, gain_values, VEML7700_GAIN_OPTIONS_COUNT);
}

int VEML7700GetItIndex(uint8_t it) {
    return indexOf(it, integration_time_values, VEML7700_IT_OPTIONS_COUNT);
}

uint8_t indexOf(uint8_t element, const uint8_t *array, uint8_t array_size) {
    for (int i = 0; i < array_size; i++) {
        if (array[i] == element) {
            return i;
        }
    }
    return -1;
}

uint32_t VEML7700GetCurrentMaximumLux(BusController *this) {
    int gain_index = VEML7700GetGainIndex(priv_conf->als_gain);
    int it_index = VEML7700GetItIndex(priv_conf->als_it);

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

esp_err_t VEML7700ReadAlsLux(BusController *this, float *lux) {
    uint8_t buffer[2];
    esp_err_t ret = BusControllerRead(this, ALS, buffer, 2, 1);
    if (ret != ESP_OK) {
        return ret;
    }

    uint16_t raw = (buffer[0] | buffer[1] << 8);
    *lux = (float)raw * priv_conf->resolution;

    return ret;
}

esp_err_t VEML7700ReadAlsLuxAuto(BusController *this, float *lux) {
    
    // Read the lux value for the current configuration
    VEML7700ReadAlsLux(this, lux);

    // Check if the current configuration is optimal, if not, optimize it
    esp_err_t ret = VEML7700OptimizeConfiguration(this, lux);
    if(ret == ESP_OK) {
        ESP_LOGI("VEML7700", "Optimized configuration");
        return VEML7700ReadAlsLux(this, lux);
    }

    ESP_LOGI("VEML7700", "Configuration is optimal");
    return ESP_OK;
}

esp_err_t VEML7700OptimizeConfiguration(BusController *this, float *lux) {
    if (ceil(*lux) >= VEML7700GetCurrentMaximumLux(this)) {
		if (priv_conf->maximum_lux == VEML7700GetMaxLux(this)) {
			return ESP_FAIL;
		}
		VEML7700DecreaseResolution(this);
	} 
    else if (*lux < VEML7700GetLowerLux(this)) {
		if (priv_conf->maximum_lux == VEML7700GetLowestMaxLux(this)) {
			return ESP_FAIL;
		}
		VEML7700IncreaseResolution(this);
	} 
    else {
		return ESP_FAIL;
	}

	return ESP_OK;
}

void VEML7700DecreaseResolution(BusController *this) {
    int gain_index = VEML7700GetGainIndex(priv_conf->als_gain);
    int it_index = VEML7700GetItIndex(priv_conf->als_it);

    if (gain_index == 3) {
        priv_conf->als_it = integration_time_values[it_index + 1];
    }
    else if (gain_index == 5) {
        priv_conf->als_gain = gain_values[gain_index - 1];
    }
    else {
        if (resolution_map[it_index + 1][gain_index] > priv_conf->resolution) {
            if (resolution_map[it_index + 1][gain_index] <= resolution_map[it_index][gain_index + 1]) {
                priv_conf->als_it = integration_time_values[it_index + 1];
            }
        }
        else if (resolution_map[it_index][gain_index + 1] > priv_conf->resolution) {
			if (resolution_map[it_index][gain_index + 1] <= resolution_map[it_index + 1][gain_index]) {
				priv_conf->als_gain = gain_values[gain_index + 1];
			}
		}
    }

    VEML7700SendConfiguration(this, priv_conf);
}

void VEML7700IncreaseResolution(BusController *this) {
    int gain_index = VEML7700GetGainIndex(priv_conf->als_gain);
    int it_index = VEML7700GetItIndex(priv_conf->als_it);

    if ((gain_index > 0) && (it_index > 0)) {
		if (maximums_map[it_index][gain_index - 1] >= maximums_map[it_index - 1][gain_index]) {
			priv_conf->als_gain = gain_values[gain_index - 1];
		} else {
			priv_conf->als_it = integration_time_values[it_index - 1];
		}
	} else if ((gain_index > 0) && (it_index == 0)) {
		priv_conf->als_gain = gain_values[gain_index - 1];
	} else {
		priv_conf->als_it = integration_time_values[it_index - 1];
	}

	VEML7700SendConfiguration(this, priv_conf);
}

uint32_t VEML7700GetMaxLux(BusController *this) {
    return maximums_map[VEML7700_IT_OPTIONS_COUNT - 1][VEML7700_GAIN_OPTIONS_COUNT - 1];
}

uint32_t VEML7700GetLowestMaxLux(BusController *this) {
    return maximums_map[0][0];
}

uint32_t VEML7700GetLowerLux(BusController *this) {
    int gain_index = VEML7700GetGainIndex(priv_conf->als_gain);
    int it_index = VEML7700GetItIndex(priv_conf->als_it);

    if ((gain_index > 0) && (it_index > 0)) {
		if (maximums_map[it_index][gain_index - 1] >= maximums_map[it_index - 1][gain_index]) {
			return maximums_map[it_index][gain_index - 1];
		} else {
			return maximums_map[it_index - 1][gain_index];
		}
	} else if ((gain_index > 0) && (it_index == 0)) {
		return maximums_map[it_index][gain_index - 1];
	} else {
		return maximums_map[it_index - 1][gain_index];
	}
}

esp_err_t VEML7700ReadAlsWhite(BusController *this, float *white) {
    uint8_t buffer[2];
    esp_err_t ret = BusControllerRead(this, WHITE, buffer, 2, 1);
    if (ret != ESP_OK) {
        return ret;
    }

    uint16_t raw = (buffer[0] | buffer[1] << 8);
    *white = (float)raw * priv_conf->resolution;

    return ret;
}