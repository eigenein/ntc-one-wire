I've made this device to connect my [NTC thermistor](https://en.wikipedia.org/wiki/Thermistor#NTC) to a [Livolo thermostat](https://www.livolo.eu/c-4632899/livolo-thermostat/) which only works with a [DS18B20 digital thermometer](https://www.maximintegrated.com/en/products/sensors/DS18B20.html).

### Instructions

- Generate `table.h` for your particular NTC thermistor. There's `table.py` script for this that accepts two NTC measurements for calibration and a fixed resistance that is used in the device. See [Measuring the temperature with NTCs](http://www.giangrandi.ch/electronics/ntc/ntc.shtml) for more information. The file included in the repo was generated for a 10kOhm NTC.
- **For Livolo thermostats.** Livolo's 1-Wire master has non-standard timings and does not read all the data transmitted. You'll need to patch `OneWireHub_config.h` and `DS18B20.cpp` using the included `livolo.patch`.
- Connect your NTC to GND and the ADC pin.
- Connect Vcc and the ADC pin using a fixed resistor.
