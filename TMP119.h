/* Blue Robotics Arduino TMP119 Temperature Sensor Library
------------------------------------------------------------
 
Title: Blue Robotics Arduino TMP119 Temperature Sensor Library
Description: This library provides utilities to communicate with and to
read data from the Texas Instruments TMP119 high-accuracy temperature
sensor.
Authors: Rustom Jehangir, Blue Robotics Inc.
		 Jonathan Newman, Blue Robotics Inc.
         Adam Šimko, Blue Robotics Inc.
-------------------------------
The MIT License (MIT)
Copyright (c) 2016 Blue Robotics Inc.
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-------------------------------*/ 

#ifndef TMP119_H_BLUEROBOTICS
#define TMP119_H_BLUEROBOTICS

#include "Arduino.h"
#include <Wire.h>

/** Conversion averaging mode (AVG[1:0], configuration register bits 6:5).
 *  Determines how many conversions are accumulated and averaged before the
 *  temperature register updates. More averaging reduces noise but lengthens
 *  the conversion cycle. Power-on default: TMP119_AVERAGE_8X.
 */
typedef enum {
	TMP119_AVERAGE_1X,  // No averaging
	TMP119_AVERAGE_8X,  // 8 averaged conversions (power-on default)
	TMP119_AVERAGE_32X, // 32 averaged conversions
	TMP119_AVERAGE_64X, // 64 averaged conversions
} tmp119_average_count_t;

/** Minimum standby delay between conversions in continuous-conversion mode
 *  (CONV[2:0], configuration register bits 9:7). This sets the standby time;
 *  the total conversion cycle time also depends on the averaging setting (see
 *  datasheet Table 8-6). Power-on default: TMP119_DELAY_1000_MS.
 */
typedef enum {
	TMP119_DELAY_0_MS,
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
