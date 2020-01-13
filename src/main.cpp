#include <Arduino.h>
#include <DS18B20.h>

#include "table.h"

// NTC input.
constexpr uint8_t adcPin = A1;

// Temperature measurement interval (approximate).
constexpr unsigned long interval = 1000;

// 1-Wire I/O.
auto hub = OneWireHub(12); // PB4/MISO
auto ds18b20 = DS18B20(0x28, 0x92, 0x23, 0xC5, 0x1E, 0x19, 0x01); // Copied from a real sensor: 289223C51E190171.

// Exponential smoothing factor.
constexpr float smoothingFactor = 0.2;

// Error flag indicator.
constexpr uint8_t ledPin = 13;

void setup() {
    // TODO: setup WDT.

    pinMode(adcPin, INPUT);

    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);

    // Default temperature according to the datasheet.
    ds18b20.setTemperatureRaw(0x0550);
    hub.attach(ds18b20);
}

float readTemperature(const uint8_t pin) {
    return adcToTemperature(analogRead(pin));
}

void loop() {
    static uint32_t nextMillis = millis();
    static float temperature = readTemperature(adcPin);

    // TODO: reset WDT before polling the hub.
    hub.poll();

    // Display the error flag. Timeouts are okay.
    const auto error = (uint8_t)hub.getError();
    digitalWrite(13, ((error != 0) && (error != 3)) ? HIGH : LOW);

    if (millis() > nextMillis) {
        // Schedule the next measurement.
        nextMillis = millis() + 1000;

        // Apply basic exponential smoothing filter.
        // https://en.wikipedia.org/wiki/Exponential_smoothing#Basic_exponential_smoothing
        temperature = temperature + smoothingFactor * (readTemperature(adcPin) - temperature);
        ds18b20.setTemperature(temperature);
    }
}
