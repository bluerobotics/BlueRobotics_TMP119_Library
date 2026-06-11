/* Blue Robotics Arduino TMP119 Temperature Sensor Library
 *
 * This library provides utilities to communicate with and read data from the
 * Texas Instruments TMP119 high-accuracy temperature sensor.
 *
 * Author: Zachariah Mears, Blue Robotics Inc.
 *
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Blue Robotics Inc.
 */

#ifndef TMP119_H_BLUEROBOTICS
#define TMP119_H_BLUEROBOTICS

#include "Arduino.h"
#include <Wire.h>

/** Conversion averaging mode (AVG[1:0], configuration register bits 6:5).
 *  Determines how many conversions are accumulated and averaged before the
 *  temperature register updates. More averaging reduces noise but takes longer
 *  to produce each result. The time to compute one averaged result is the
 *  per-mode minimum conversion time shown below (datasheet Table 8-6).
 *  Power-on default: TMP119_AVERAGE_8X.
 */
typedef enum {
	TMP119_AVERAGE_1X,  // No averaging   (min conversion time 15.5 ms)
	TMP119_AVERAGE_8X,  // 8 conversions  (min conversion time 125 ms, default)
	TMP119_AVERAGE_32X, // 32 conversions (min conversion time 500 ms)
	TMP119_AVERAGE_64X, // 64 conversions (min conversion time 1 s)
} tmp119_average_count_t;

/** Minimum standby delay inserted between conversions in continuous-conversion
 *  mode (CONV[2:0], configuration register bits 9:7).
 *
 *  This is not the actual time between readings on its own. The actual
 *  conversion cycle time is the greater of this standby delay and the time
 *  needed by the selected averaging mode (see tmp119_average_count_t). For
 *  example, TMP119_DELAY_NONE with TMP119_AVERAGE_64X still produces a ~1 s
 *  cycle because the 64x average alone takes 1 s. See datasheet Table 8-6.
 *
 *  Power-on default: TMP119_DELAY_1000_MS.
 */
typedef enum {
	TMP119_DELAY_NONE,    // no added standby delay
	TMP119_DELAY_125_MS,
	TMP119_DELAY_250_MS,
	TMP119_DELAY_500_MS,
	TMP119_DELAY_1000_MS, // power-on default
	TMP119_DELAY_4000_MS,
	TMP119_DELAY_8000_MS,
	TMP119_DELAY_16000_MS,
} tmp119_delay_t;

class TMP119 {
public:

	/** The TMP119 supports up to four I2C addresses (0x48 - 0x4B), selected
	 *  by the ADD0 pin. The default address (ADD0 to GND) is 0x48.
	 */
	TMP119(uint8_t address = 0x48, TwoWire *wire = &Wire);

	/** Initialize the sensor connection - returns false if not detected.
	 *  Configures the sensor for the fastest update rate (no averaging and
	 *  the shortest standby delay). Call setAveraging() / setReadDelay()
	 *  afterwards to override.
	 */
	bool init();

	/** Reads the latest temperature conversion from the sensor.
	 */
	void read();

	/** Temperature returned in deg C.
	 */
	float temperature();

	/** Sets the conversion averaging mode. Returns false if the I2C write
	 *  fails. Other configuration bits are preserved.
	 */
	bool setAveraging(tmp119_average_count_t avg);

	/** Sets the minimum standby delay between conversions in
	 *  continuous-conversion mode. Returns false if the I2C write fails.
	 *  Other configuration bits are preserved.
	 */
	bool setReadDelay(tmp119_delay_t delay);

	/** Reads the raw 16-bit configuration register (address 0x01).
	 */
	uint16_t getConfig();

	/** Writes the raw 16-bit configuration register (address 0x01). Returns
	 *  false if the I2C write fails. Read-only bits are ignored by the device.
	 */
	bool setConfig(uint16_t config);

private:
	TwoWire* _wire;
	uint8_t  _address;
	float    TEMP;

};

#endif
