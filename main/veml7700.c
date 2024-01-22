#include "veml7700.h"

uint8_t VEML7700Init(BusController *bus) {
    // Set gain, persistence, integration time and power saving mode
    // Set interrupt thresholds
    VEML7700Enable(0);
    // VEML7700SetInterruptEnable(0);
    // VEML7700SetPersistence(VEML7700_PERSISTENCE_1);
    // VEML7700SetGain(VEML7700_GAIN_1_8);
    // VEML7700SetIntegrationTime(VEML7700_INTEGRATION_TIME_100MS);
    // VEML7700SetPowerSavingMode(VEML7700_POWER_SAVING_MODE_1);
    // VEML7700Enable(1);

    return 1;
}

void VEML7700GetRead(struct veml7700_read *read) {
    read->als = VEML7700GetALS();
}

void VEML7700SetConf(struct veml7700_conf *conf) {
    
}

void VEML7700GetConf(struct veml7700_conf *conf) {
    
}

void VEML7700GetLux(float *lux) {
    uint16_t raw_als = 0x00;
    VEML7700GetALS(raw_als);
    *lux = VEML7700ComputeLux(raw_als, 1);
}

void VEML7700SetResolution(uint8_t resolution) {
    // Get gain and integration time
    uint8_t data_it;
    uint8_t data_gain;
    VEML7700GetIntegrationTime(data_it);
    VEML7700GetGain(data_gain);
    // Compute resolution
    resolution = MAX_RES * (IT_MAX / data_it) * (GAIN_MAX / data_gain);
}

void VEML7700GetResolution(uint8_t resolution) {
    // Get gain and integration time
    uint8_t data_it;
    uint8_t data_gain;
    VEML7700GetIntegrationTime(data_it);
    VEML7700GetGain(data_gain);
    // Compute resolution
    resolution = MAX_RES * (IT_MAX / data_it) * (GAIN_MAX / data_gain);
}

float VEML7700ComputeLux(float raw_als, uint8_t is_corrected) {
    // Get resolution
    uint8_t resolution;
    VEML7700GetResolution(resolution);
    float lux = resolution * raw_als;
    
    if(is_corrected) {
        lux = (((6.0135e-13 * lux - 9.3924e-9) * lux + 8.1488e-5) * lux + 1.0023) * lux;
    }

    return lux;
}

void VEML7700Enable(uint8_t enable) {
    uint8_t data = 0x00;
    if (enable) {
        data = 0x00;
    }
    else {
        data = 0x01;
    }
    I2CWriteRegister(VEML7700_I2C_ADDRESS, VEML7700_REG_ALS_CONFIG, data);
}

void VEML7700SetPersistence(uint8_t persistence) {
    uint8_t data = 0x00;
    data = I2CReadRegister(VEML7700_I2C_ADDRESS, VEML7700_REG_ALS_CONFIG);
    data &= 0xF3;
    data |= (persistence << 2);
    I2CWriteRegister(VEML7700_I2C_ADDRESS, VEML7700_REG_ALS_CONFIG, data);
}

void VEML7700SetGain(uint8_t gain) {
    uint8_t data = 0x00;
    data = I2CReadRegister(VEML7700_I2C_ADDRESS, VEML7700_REG_ALS_CONFIG);
    data &= 0xFC;
    data |= gain;
    I2CWriteRegister(VEML7700_I2C_ADDRESS, VEML7700_REG_ALS_CONFIG, data);
}

void VEML7700SetIntegrationTime(uint8_t integration_time) {
    uint8_t data = 0x00;
    data = I2CReadRegister(VEML7700_I2C_ADDRESS, VEML7700_REG_ALS_CONFIG);
    data &= 0xCF;
    data |= (integration_time << 4);
    I2CWriteRegister(VEML7700_I2C_ADDRESS, VEML7700_REG_ALS_CONFIG, data);
}

void VEML7700SetPowerSavingMode(uint8_t power_saving_mode) {
    uint8_t data = 0x00;
    data = I2CReadRegister(VEML7700_I2C_ADDRESS, VEML7700_REG_ALS_CONFIG);
    data &= 0xF8;
    data |= power_saving_mode;
    I2CWriteRegister(VEML7700_I2C_ADDRESS, VEML7700_REG_ALS_CONFIG, data);
}

uint16_t VEML7700GetALS() {
    uint16_t data = 0x00;
    data = I2CReadRegister(VEML7700_I2C_ADDRESS, VEML7700_REG_ALS_DATA);
    data <<= 8;
    data |= I2CReadRegister(VEML7700_I2C_ADDRESS, VEML7700_REG_ALS_DATA + 1);
    return data;
}

void VEML7700GetGain(uint8_t gain) {
    uint8_t data = 0x00;
    data = I2CReadRegister(VEML7700_I2C_ADDRESS, VEML7700_REG_ALS_CONFIG);
    data &= 0x03;
    gain = data;
}

void VEML7700GetIntegrationTime(uint8_t it) {
    uint8_t data = 0x00;
    data = I2CReadRegister(VEML7700_I2C_ADDRESS, VEML7700_REG_ALS_CONFIG);
    data &= 0x0C;
    it = data >> 2;
}

void VEML7700GetPowerSavingMode(uint8_t power_saving_mode) {
    uint8_t data = 0x00;
    data = I2CReadRegister(VEML7700_I2C_ADDRESS, VEML7700_REG_ALS_CONFIG);
    data &= 0x07;
    power_saving_mode = data;
}

void VEML7700GetPersistence(uint8_t persistence) {
    uint8_t data = 0x00;
    data = I2CReadRegister(VEML7700_I2C_ADDRESS, VEML7700_REG_ALS_CONFIG);
    data &= 0x30;
    persistence = data >> 4;
}

void VEML7700GetInterruptEnable(uint8_t interrupt_enable) {
    uint8_t data = 0x00;
    data = I2CReadRegister(VEML7700_I2C_ADDRESS, VEML7700_REG_ALS_INT_EN);
    interrupt_enable = data;
}

void VEML7700SetHighThreshold(uint16_t threshold) {
    uint8_t data = 0x00;
    data = threshold >> 8;
    I2CWriteRegister(VEML7700_I2C_ADDRESS, VEML7700_REG_ALS_THDH, data);
    data = threshold & 0xFF;
    I2CWriteRegister(VEML7700_I2C_ADDRESS, VEML7700_REG_ALS_THDH + 1, data);
}

void VEML7700SetLowThreshold(uint16_t threshold) {
    uint8_t data = 0x00;
    data = threshold >> 8;
    I2CWriteRegister(VEML7700_I2C_ADDRESS, VEML7700_REG_ALS_THDL, data);
    data = threshold & 0xFF;
    I2CWriteRegister(VEML7700_I2C_ADDRESS, VEML7700_REG_ALS_THDL + 1, data);
}

void VEML7700GetHighThreshold(uint16_t *threshold) {
    uint8_t data = 0x00;
    data = I2CReadRegister(VEML7700_I2C_ADDRESS, VEML7700_REG_ALS_THDH);
    data <<= 8;
    data |= I2CReadRegister(VEML7700_I2C_ADDRESS, VEML7700_REG_ALS_THDH + 1);
    threshold = data;
}

void VEML7700GetLowThreshold(uint16_t *threshold) {
    uint8_t data = 0x00;
    data = I2CReadRegister(VEML7700_I2C_ADDRESS, VEML7700_REG_ALS_THDL);
    data <<= 8;
    data |= I2CReadRegister(VEML7700_I2C_ADDRESS, VEML7700_REG_ALS_THDL + 1);
    threshold = data;
}

void VEML7700SetInterruptEnable(uint8_t enable) {
    uint8_t data = 0x00;
    if (enable) {
        data = 0x00;
    }
    else {
        data = 0x01;
    }
    I2CWriteRegister(VEML7700_I2C_ADDRESS, VEML7700_REG_ALS_INT_EN, data);
}

void VEML7700GetInterruptEnable(uint8_t enable) {
    uint8_t data = 0x00;
    data = I2CReadRegister(VEML7700_I2C_ADDRESS, VEML7700_REG_ALS_INT_EN);
    enable = data;
}

void VEML7700GetLowThreshold(uint16_t threshold) {
    uint8_t data = 0x00;
    data = I2CReadRegister(VEML7700_I2C_ADDRESS, VEML7700_REG_ALS_IF_L);
    flag = data;
}

void VEML7700GetHighThreshold(uint16_t threshold) {
    uint8_t data = 0x00;
    data = I2CReadRegister(VEML7700_I2C_ADDRESS, VEML7700_REG_ALS_IF_H);
    flag = data;
}

void VEML7700powerSaveEnable() {
    uint8_t data = 0x00;
    if (enable) {
        data = 0x00;
    }
    else {
        data = 0x01;
    }
    I2CWriteRegister(VEML7700_I2C_ADDRESS, VEML7700_REG_ALS_IF_L, data);
}

void VEML7700AlsWhite(float *als_white) {
    uint16_t data = 0x00;
    data = I2CReadRegister(VEML7700_I2C_ADDRESS, VEML7700_REG_WHITE_DATA);
    data <<= 8;
    data |= I2CReadRegister(VEML7700_I2C_ADDRESS, VEML7700_REG_WHITE_DATA + 1);
    als_white = data;
}