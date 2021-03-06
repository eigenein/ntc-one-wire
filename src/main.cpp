#include <Arduino.h>
#include <avr/wdt.h>

#include <DS18B20.h>

#include "table.h"

// NTC input pin. Another NTC wire goes to the ground.
// This pin should be pulled up by a fixed resistor with a known resistance.
constexpr uint8_t adcPin = A1;

// Temperature measurement interval (approximate).
constexpr unsigned long interval = 1000;

// 1-Wire I/O.
auto hub = OneWireHub(12); // PB4 aka MISO
auto ds18b20 = DS18B20(0x28, 0x92, 0x23, 0xC5, 0x1E, 0x19, 0x01); // Copied from a real sensor: 289223C51E190171.

// Error LED pin.
constexpr uint8_t ledPin = 13;

// Livolo thermostat performs some calibration.
// This is the temperature offset we apply to match the actual temperature.
constexpr float livoloOffset = +3.0f;

void setup() {
    // I've never experienced a freeze on `hub.poll()`, but just in case enabling 8 seconds WDT.
    wdt_disable();
    wdt_enable(WDTO_8S);

    // Note the baud rate if you want to run a serial monitor.
    // I've set the maximum for Arduino Nano to reduce the time spent by `Serial.print()` in `loop()`.
    Serial.begin(2000000);
    while (!Serial);

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

    wdt_reset();
    hub.poll();

    // Display the error flag. Timeouts are okay.
    const auto error = (uint8_t)hub.getError();
    const auto isError = (error != 0) && (error != 3);
    digitalWrite(13, isError ? HIGH : LOW);
    if (isError) {
        Serial.print("E: ");
        Serial.println(error);
    }

    if (millis() > nextMillis) {
        // Schedule the next measurement.
        nextMillis = millis() + interval;

        const auto reading = readTemperature(adcPin);
        ds18b20.setTemperature(reading + livoloOffset);

        Serial.print(millis());
        Serial.print(" ms | ");
        Serial.print(reading);
        Serial.println(" °C");
        Serial.flush();
    }
}
