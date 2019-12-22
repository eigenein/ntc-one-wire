#include <Arduino.h>
#include <DS18B20.h>

#include "table.h"

// NTC input.
constexpr uint8_t adcPin = A1;

// 1-Wire I/O.
auto hub = OneWireHub(8);
auto ds18b20 = DS18B20(0x28, 0x92, 0x23, 0xC5, 0x1E, 0x19, 0x01); // 289223C51E190171

// Error flag indicator.
constexpr uint8_t ledPin = 13;

// Error code indicators.
constexpr uint8_t errorPin1 = 2;
constexpr uint8_t errorPin2 = 3;
constexpr uint8_t errorPin3 = 5;
constexpr uint8_t errorPin4 = 7;

// Livolo thermostat patch of `OneWireHub_config.h`:
//
// constexpr timeOW_t ONEWIRE_TIME_RESET_MAX[2]         = {   50000_us, 80_us }; // from ds2413
// constexpr timeOW_t ONEWIRE_TIME_MSG_HIGH_TIMEOUT     = { 30000_us };
// constexpr timeOW_t ONEWIRE_TIME_READ_MIN[2]          = {    35_us,  4_us };

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

    hub.attach(ds18b20);
}

void loop() {
    static uint32_t nextMillis = millis();

    hub.poll();

    auto error = (uint8_t)hub.getError();
    if (error == 3) {
        // Not interested in reset timeouts.
        error = 0;
    }
    digitalWrite(13, (error != 0) ? HIGH : LOW);

    // Display the error code for debugging.
    digitalWrite(2, error & 1);
    digitalWrite(3, (error >> 1) & 1);
    digitalWrite(5, (error >> 2) & 1);
    digitalWrite(7, (error >> 3) & 1);

    if (millis() > nextMillis) {
        nextMillis += 1000;

        const int adcValue = analogRead(adcPin);
        const float temperature = adc_to_temperature(adcValue);
        // ds18b20.setTemperature(temperature);
        ds18b20.setTemperature(21.0f);
    }
}
