#include "TMP119.h"

#define TMP119_TEMP_REG                    0x00
#define TMP119_CONFIG_REG                  0x01
#define TMP119_DEVICE_ID_REG               0x0F
#define TMP119_DEVICE_ID                   0x2117

#define TMP119_CONV_SHIFT                  7
#define TMP119_CONV_MASK                   0x0380 // CONV[2:0], bits 9:7
#define TMP119_AVG_SHIFT                   5
#define TMP119_AVG_MASK                    0x0060 // AVG[1:0], bits 6:5

TMP119::TMP119(uint8_t address, TwoWire *wire) {
	_address = address;
	_wire = wire;
}

bool TMP119::init() {
	// Verify the device by reading its ID register
	_wire->beginTransmission(_address);
	_wire->write(TMP119_DEVICE_ID_REG);
	_wire->endTransmission();

	if ( _wire->requestFrom(_address, (uint8_t)2) != 2 ) {
		return false;
	}

	uint16_t id = (_wire->read() << 8) | _wire->read();
	if ( id != TMP119_DEVICE_ID ) {
		return false;
	}

	// Configure for the fastest update rate: no averaging and the shortest
	// standby delay, giving a ~15.5 ms conversion cycle (datasheet Table 8-6)
	uint16_t config = getConfig();
	config &= ~(TMP119_AVG_MASK | TMP119_CONV_MASK); // AVG = 1X, delay = 0 ms
	return setConfig(config);
}

void TMP119::read() {
	// The TMP119 powers up in continuous-conversion mode, so the
	// temperature register always holds the most recent conversion.
	_wire->beginTransmission(_address);
	_wire->write(TMP119_TEMP_REG);
	_wire->endTransmission();

	_wire->requestFrom(_address, (uint8_t)2);
	int16_t raw = (_wire->read() << 8) | _wire->read();

	// Each LSB represents 0.0078125 deg C, data is in 2's complement format
	TEMP = raw * 0.0078125f;
}

float TMP119::temperature() {
	return TEMP;
}

uint16_t TMP119::getConfig() {
	_wire->beginTransmission(_address);
	_wire->write(TMP119_CONFIG_REG);
	_wire->endTransmission();

	_wire->requestFrom(_address, (uint8_t)2);
	return (_wire->read() << 8) | _wire->read();
}

bool TMP119::setConfig(uint16_t config) {
	_wire->beginTransmission(_address);
	_wire->write(TMP119_CONFIG_REG);
	_wire->write(config >> 8);   // MSB first
	_wire->write(config & 0xFF); // LSB
	return _wire->endTransmission() == 0;
}

bool TMP119::setAveraging(tmp119_average_count_t avg) {
	uint16_t config = getConfig();
	config = (config & ~TMP119_AVG_MASK) | ((uint16_t)avg << TMP119_AVG_SHIFT);
	return setConfig(config);
}

bool TMP119::setReadDelay(tmp119_delay_t delay) {
	uint16_t config = getConfig();
	config = (config & ~TMP119_CONV_MASK) | ((uint16_t)delay << TMP119_CONV_SHIFT);
	return setConfig(config);
}
