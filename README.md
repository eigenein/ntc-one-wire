I've made this device to connect my [NTC thermistor](https://en.wikipedia.org/wiki/Thermistor#NTC) to a [Livolo thermostat](https://www.livolo.eu/c-4632899/livolo-thermostat/) which only works with a [DS18B20 digital thermometer](https://www.maximintegrated.com/en/products/sensors/DS18B20.html).

![GitHub tag (latest by date)](https://img.shields.io/github/v/tag/eigenein/ntc-one-wire)

## Instructions

### `table.h`

You need to generate the `table.h` for your particular NTC thermistor. It contains an array which maps a measured ADC value to actual temperature. You can do this via the provided `table.py` script.

Measure your NTC resistance at two different temperature points. Then call `table.py` specifying your measurements:

```sh
python table.py --t1 <temperature-1> --r1 <resistance-at-t1> --t2 <temperature-2> --r2 <resistance-at-t2> --rf <fixed-resistor>
```

I recommend to measure your fixed resistor actual resistance to improve accuracy.

If your program memory is too small, you can specify `--min-t` and/or `--max-t` parameters. Then the table will be generated in a specified temperature range generating less array entries thus taking less memory.

See [Measuring the temperature with NTCs](http://www.giangrandi.ch/electronics/ntc/ntc.shtml) for more information on how the conversion is implemented.

### Note for Livolo thermostats

Livolo's 1-Wire master has non-standard timings and does not read all the data transmitted. You'll need to patch OneWireHub's `OneWireHub_config.h` and `DS18B20.cpp` using the included `livolo.patch`.

See also https://github.com/orgua/OneWireHub/issues/84.

### Smoothing filter

The code implementes the simplest [exponential smoothing](https://en.wikipedia.org/wiki/Exponential_smoothing) of converted temperature. You may want to adjust `smoothingFactor` according to your needs.

### Connections

- Connect your NTC to GND and the ADC pin.
- Connect Vcc and the ADC pin using a fixed resistor.
- If possible, I also recommend connecting a capacitor in parallel with the NTC. This will reduce random measurement glitches. 100nF worked for me.
