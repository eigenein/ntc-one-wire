#include <Arduino.h>
#include <DS18B20.h>

#include "table.h"

constexpr uint8_t ledPin { PB1 };
constexpr uint8_t adcPin { A1 };

auto hub = OneWireHub(PB0); // LED
auto ds18b20 = DS18B20(DS18B20::family_code, 0x00, 0x00, 0xB2, 0x18, 0xDA, 0x00);

void setup() {
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
    hub.attach(ds18b20);
}

void loop() {
    static uint32_t nextMillis = millis();
    static uint8_t ledState = LOW;

    hub.poll();

    if (millis() > nextMillis) {
        nextMillis += 5000;

        const auto adcValue = analogRead(adcPin);
        ds18b20.setTemperature(adc_to_temperature(adcValue));

        ledState = ledState == HIGH ? LOW : HIGH;
        digitalWrite(ledPin, ledState);
    }
}
