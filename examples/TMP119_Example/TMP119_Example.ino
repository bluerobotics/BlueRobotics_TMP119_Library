/* Blue Robotics TMP119 Library Example
 *
 * This example demonstrates the TMP119 Library with a connected sensor. It
 * reads the sensor and prints the values to the serial terminal, and shows how
 * to configure the conversion averaging and the standby delay between
 * conversions. After init(), the sensor is set to its fastest update rate;
 * here we override that with heavy averaging and a 1 second cycle for low-noise
 * readings.
 *
 * Designed for the Arduino Uno; compile and upload via the Arduino 1.0+ IDE.
 *
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Blue Robotics Inc.
 */

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
    // Averaging options (setAveraging) and the time each takes per result:
    //   TMP119_AVERAGE_1X  -> 15.5 ms
    //   TMP119_AVERAGE_8X  -> 125 ms
    //   TMP119_AVERAGE_32X -> 500 ms
    //   TMP119_AVERAGE_64X -> 1 s
    //
    // Minimum standby delay options (setReadDelay):
    //   TMP119_DELAY_NONE, TMP119_DELAY_125_MS, TMP119_DELAY_250_MS,
    //   TMP119_DELAY_500_MS, TMP119_DELAY_1000_MS, TMP119_DELAY_4000_MS,
    //   TMP119_DELAY_8000_MS, TMP119_DELAY_16000_MS
    //
    // The actual time between readings is the greater of the averaging time
    // above and the standby delay (datasheet Table 8-6). So 64x averaging with
    // TMP119_DELAY_1000_MS gives ~1 s, but 64x with TMP119_DELAY_NONE is also
    // ~1 s since the averaging itself takes 1 s.
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
