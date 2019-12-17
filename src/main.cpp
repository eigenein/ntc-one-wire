#include "Arduino.h"
#include "DS18B20.h"

constexpr uint8_t ledPin { PB1 };
constexpr uint8_t adcPin { PB2 };

auto hub = OneWireHub(PB0); // LED
auto ds18b20 = DS18B20(DS18B20::family_code, 0x00, 0x00, 0xB2, 0x18, 0xDA, 0x00);

void setup() {
    pinMode(ledPin, OUTPUT);
    hub.attach(ds18b20);
}

void loop() {
    static uint32_t nextMillis = millis();

    hub.poll();

    if (millis() > nextMillis) {
        nextMillis += 1000;
        const auto value = analogRead(adcPin);
        // TODO: http://www.giangrandi.ch/electronics/ntc/ntc.shtml
        // TODO: generate table via a script.
        ds18b20.setTemperature(20.0f);
    }
}
