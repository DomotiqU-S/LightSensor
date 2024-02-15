#include "VEML7700.hpp"

VEML7700::~VEML7700() {
}

uint8_t VEML7700::getAlsIntEn() {
    return m_als_int_en;
}

uint8_t VEML7700::getAlsSd() {
    return m_als_sd;
}

uint8_t VEML7700::getGain() {
    return m_gain;
}

uint8_t VEML7700::getIntegrationTime() {
    return m_integration_time;
}

uint8_t VEML7700::getPersistence() {
    return 0;
}

uint8_t VEML7700::getPowerSaveMode() {
    return m_als_sd;
}

void VEML7700::setAlsIntEn(uint8_t als_int_en) {
    m_als_int_en = als_int_en;
}

void VEML7700::setAlsSd(uint8_t als_sd) {
    m_als_sd = als_sd;
}

void VEML7700::setGain(uint8_t gain) {
    m_gain = gain;
}

void VEML7700::setIntegrationTime(uint8_t integration_time) {
    m_integration_time = integration_time;
}

void VEML7700::setPersistence(uint8_t persistence) {
    m_persistence = persistence;
}

void VEML7700::setPowerSaveMode(uint8_t power_save_mode) {
    m_als_sd = power_save_mode;
}

float VEML7700::readAlsLux() {
    uint8_t buffer[2];
    esp_err_t ret = m_bus.read(buffer, ALS, 2, 1);
    if (ret != ESP_OK) {
        return -1;
    }

    uint16_t raw = (buffer[0] | buffer[1] << 8);
    return (float)raw * m_resolution;
}

float VEML7700::readAlsWhite() {
    uint8_t buffer[2];
    esp_err_t ret = m_bus.read(buffer, WHITE, 2, 1);
    if (ret != ESP_OK) {
        return -1;
    }

    uint16_t raw = (buffer[0] | buffer[1] << 8);
    return (float)raw * m_resolution;
}

void VEML7700::sendConfiguration() {
    uint16_t config_data = ( 
        (m_gain << 11) |
        (m_integration_time << 6) |
        (m_persistence << 4) |
        (m_als_int_en << 1) |
        (m_als_sd << 0)
    );

    uint8_t data[2] = { (uint8_t)(config_data >> 8), (uint8_t)(config_data & 0xFF)};

    m_resolution = getResolution();
    m_maximum_lux = getCurrentMaximumLux();

    m_bus.write(data, ALS_CONF, 2);
}

float VEML7700::getResolution() {
    int gain_index = getGainIndex(m_gain);
    int it_index = getItIndex(m_integration_time);

    return resolution_map[it_index][gain_index];
}

uint8_t VEML7700::getGainIndex(uint8_t gain) {
    return indexOf(gain, gain_values, VEML7700_GAIN_OPTIONS_COUNT);
}

uint8_t VEML7700::getItIndex(uint8_t it) {
    return indexOf(it, integration_time_values, VEML7700_IT_OPTIONS_COUNT);
}

uint32_t VEML7700::getCurrentMaximumLux() {
    int gain_index = getGainIndex(m_gain);
    int it_index = getItIndex(m_integration_time);

    return maximums_map[it_index][gain_index];
}

uint8_t VEML7700::indexOf(uint8_t element, const uint8_t *array, uint8_t array_size) {
    for (int i = 0; i < array_size; i++) {
        if (array[i] == element) {
            return i;
        }
    }
    return -1;
}