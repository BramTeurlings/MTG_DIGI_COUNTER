#pragma once

#include <Arduino.h>

// A number that stays within [minValue, maxValue]
class BoundedValue {
public:
  BoundedValue(int minValue, int maxValue) : minValue(minValue), maxValue(maxValue) {}

  // Adds change, clamped to the range. Returns how much the value actually changed.
  int add(int change) {
    int before = current;
    set(current + change);
    return current - before;
  }

  // Sets the value, clamped to the range
  void set(int value) {
    current = constrain(value, minValue, maxValue);
  }

  int get() const { return current; }

private:
  int minValue;
  int maxValue;
  int current = 0;
};
