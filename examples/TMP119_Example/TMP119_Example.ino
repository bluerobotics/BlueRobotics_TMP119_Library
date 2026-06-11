
/* Blue Robotics TMP119 Library Example
-----------------------------------------------------
 
Title: Blue Robotics TMP119 Library Example

Description: This example demonstrates the TMP119 Library with a connected
sensor. It reads the sensor and prints the values to the serial terminal,
and shows how to configure the conversion averaging and the standby delay
between conversions. After init(), the sensor is set to its fastest update
rate; here we override that with heavy averaging and a 1 second cycle for
low-noise readings.

-------------------------------
The MIT License (MIT)

Copyright (c) 2026 Blue Robotics Inc.

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

#include <Wire.h>
#include "TMP119.h"

// Optional constructor parameters: TMP119(address, wire)
//   address - I2C address set by the ADD0 pin (default 0x48):
//             0x48 (GND), 0x49 (V+), 0x4A (SDA), 0x4B (SCL)
//   wire    - I2C bus to use (default &Wire)
// e.g. "TMP119 sensor;" is equivalent to "TMP119 sensor(0x48, &Wire);"
TMP119 sensor;

void setup() {
    Serial.begin(9600);
    Serial.println("Starting");

    Wire.begin();

    while (!sensor.init()) {
        Serial.println("TMP119 device failed to initialize!");
        delay(2000);
    }

    // Override the fast defaults set by init() with low-noise settings:
    // average 64 conversions and update roughly once per second.
    //
    // Averaging options (setAveraging):
    //   TMP119_AVERAGE_1X, TMP119_AVERAGE_8X, TMP119_AVERAGE_32X, TMP119_AVERAGE_64X
    //
    // Standby delay options (setReadDelay):
    //   TMP119_DELAY_0_MS, TMP119_DELAY_125_MS, TMP119_DELAY_250_MS,
    //   TMP119_DELAY_500_MS, TMP119_DELAY_1000_MS, TMP119_DELAY_4000_MS,
    //   TMP119_DELAY_8000_MS, TMP119_DELAY_16000_MS
    // (the resulting conversion cycle time depends on both settings,
    //  see datasheet Table 8-6)
    sensor.setAveraging(TMP119_AVERAGE_64X);
    sensor.setReadDelay(TMP119_DELAY_1000_MS);
}

void loop() {
    sensor.read();
    Serial.print("Temperature: ");
    Serial.print(sensor.temperature());
    Serial.println(" deg C");
    delay(1000);
}
