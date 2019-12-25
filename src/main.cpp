#include <Arduino.h>
#include <DS18B20.h>

#include "table.h"

// NTC input.
constexpr uint8_t adcPin = A1;

// Temperature measurement interval (approximate).
constexpr unsigned long interval = 1000;

// 1-Wire I/O.
auto hub = OneWireHub(2);
auto ds18b20 = DS18B20(0x28, 0x92, 0x23, 0xC5, 0x1E, 0x19, 0x01); // Real sensor ID: 289223C51E190171.

// Error flag indicator.
constexpr uint8_t ledPin = 13;

// Error code indicators.
constexpr uint8_t errorPin1 = 9;
constexpr uint8_t errorPin2 = 10;
constexpr uint8_t errorPin3 = 11;
constexpr uint8_t errorPin4 = 12;

// (e.g. `./.pio/libdeps/nanoatmega328/OneWireHub_ID526/src/OneWireHub_config.h`):

void setup() {
    pinMode(adcPin, INPUT);

    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    pinMode(errorPin1, OUTPUT);
    digitalWrite(errorPin2, LOW);
    pinMode(errorPin2, OUTPUT);
    digitalWrite(errorPin2, LOW);
    pinMode(errorPin3, OUTPUT);
    digitalWrite(errorPin3, LOW);
    pinMode(errorPin4, OUTPUT);
    digitalWrite(errorPin4, LOW);

    ds18b20.setTemperatureRaw(0x0550);
    hub.attach(ds18b20);
}

void loop() {
    static uint32_t nextMillis = millis();

    hub.poll();

    // Display the error flag. Timeouts are okay.
    const auto error = (uint8_t)hub.getError();
    digitalWrite(13, ((error != 0) && (error != 3)) ? HIGH : LOW);

    // Display the error code to debug when needed.
    digitalWrite(errorPin1, error & 1);
    digitalWrite(errorPin2, (error >> 1) & 1);
    digitalWrite(errorPin3, (error >> 2) & 1);
    digitalWrite(errorPin4, (error >> 3) & 1);

    if (millis() > nextMillis) {
        // Schedule the next measurement.
        nextMillis = millis() + 1000;

        const auto adcValue = analogRead(adcPin);
        const auto temperature = adc_to_temperature(adcValue);
        ds18b20.setTemperature(temperature);
    }
}
