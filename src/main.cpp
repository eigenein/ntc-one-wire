#include <Arduino.h>
#include <DS18B20.h>

#include "table.h"

constexpr uint8_t ledPin = PB1;
constexpr uint8_t adcPin = A1;

auto hub = OneWireHub(PB0);
auto ds18b20 = DS18B20(0x28, 0x92, 0x23, 0xC5, 0x1E, 0x19, 0x01); // 289223C51E190171

void setup() {
    pinMode(ledPin, OUTPUT);
    pinMode(adcPin, INPUT);
    digitalWrite(ledPin, LOW);
    hub.attach(ds18b20);
}

void loop() {
    static uint32_t nextMillis = millis();
    static uint8_t ledState = LOW;

    digitalWrite(ledPin, hub.poll() ? HIGH : LOW);

    if (millis() > nextMillis) {
        nextMillis += 1000;

        const int adcValue = analogRead(adcPin);
        const float temperature = adc_to_temperature(adcValue);
        ds18b20.setTemperature(temperature);

        // ledState = ledState == HIGH ? LOW : HIGH;
        // digitalWrite(ledPin, ledState);
    }
}
