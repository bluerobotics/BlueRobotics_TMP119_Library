# BlueRobotics TMP119 Temperature Sensor Library

Arduino library for the TMP119 temperature sensor. The TMP119 is an ultra-high-accuracy, low-power digital temperature sensor from Texas Instruments with an I2C-compatible interface.

# Documentation

Please see the examples for normal operation. Below are the available functions used in the library.

``` cpp
/** Construct an instance of the sensor manager. The TMP119 supports up to four
 *  I2C addresses (0x48 - 0x4B), selected by the ADD0 pin; the default address
 *  (ADD0 to GND) is 0x48. Uses the default Wire bus if left unspecified.
 *  (e.g. "TMP119 mySensor;" is equivilent to "TMP119 mySensor(0x48, &Wire);")
 */
TMP119(uint8_t address = 0x48, TwoWire *wire = &Wire);

/** Initialize the sensor connection - return false if not detected.
 *  Configures the sensor for the fastest update rate (no averaging, shortest
 *  standby delay); call setAveraging()/setReadDelay() afterwards to override.
 */
bool init();

/** Reads the latest temperature conversion from the sensor.
 */
void read();

/** Temperature returned in deg C.
 */
float temperature();

/** Sets the conversion averaging mode (TMP119_AVERAGE_1X, _8X, _32X, _64X).
 *  More averaging reduces noise but takes longer per result (1X=15.5ms,
 *  8X=125ms, 32X=500ms, 64X=1s). Returns false if the I2C write fails.
 */
bool setAveraging(tmp119_average_count_t avg);

/** Sets the minimum standby delay between conversions in continuous-conversion
 *  mode (TMP119_DELAY_NONE, TMP119_DELAY_125_MS, _250_MS, _500_MS, _1000_MS,
 *  _4000_MS, _8000_MS, _16000_MS). Returns false if the I2C write fails.
 */
bool setReadDelay(tmp119_delay_t delay);

/** Reads / writes the raw 16-bit configuration register (address 0x01)
 *  for advanced use.
 */
uint16_t getConfig();
bool setConfig(uint16_t config);

```

The TMP119 runs in continuous-conversion mode, so `read()` always returns the
latest result. `init()` configures the sensor for the fastest update rate (no
averaging, no added standby delay, ~15.5 ms cycle). To trade speed for lower
noise, call `setAveraging()` and `setReadDelay()` after `init()`.

`setReadDelay()` sets only the *minimum* standby delay. The actual time between
readings is the greater of the averaging time and that standby delay (datasheet
Table 8-6). For example, `TMP119_AVERAGE_64X` always yields at least a ~1 s
cycle because the averaging alone takes 1 s, regardless of the delay setting.

``` cpp
TMP119 sensor;
sensor.init();                                  // fastest rate by default
sensor.setAveraging(TMP119_AVERAGE_64X);        // 64-sample averaging
sensor.setReadDelay(TMP119_DELAY_1000_MS);      // ~1 s standby between reads
```

See `examples/TMP119_Example` for a complete sketch that reads the sensor
and configures averaging and the read delay (with all options listed).

# Versions

- `1.0.0` - initial release

# Reference

You can find the [TMP119 datasheet here](https://www.ti.com/lit/ds/symlink/tmp119.pdf).
