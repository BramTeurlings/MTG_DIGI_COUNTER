#pragma once

#include <Arduino.h>
#include "config.h"

// Reads the battery level through a voltage divider that is only switched on while reading.
class Battery {
public:
  void begin() {
    pinMode(BATTERY_PIN, INPUT);
    pinMode(BATTERY_ENABLE_PIN, OUTPUT); // write to this pin to turn on transistor
  }

  // Takes a new reading, raw() and scaledTo() return the last one
  void update() {
    digitalWrite(BATTERY_ENABLE_PIN, HIGH);
    rawLevel = analogRead(BATTERY_PIN);
    digitalWrite(BATTERY_ENABLE_PIN, LOW);
  }

  // Raw ADC reading, 0-1023
  int raw() const { return rawLevel; }

  // The level scaled to 0..maxPixels, for drawing a battery bar
  int scaledTo(int maxPixels) const {
    return min(rawLevel / (ADC_MAX / maxPixels), maxPixels);
  }

private:
  static constexpr int ADC_MAX = 1023;
  int rawLevel = 0;
};
