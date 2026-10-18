#pragma once

#include <Arduino.h>
#include "../config.h"

// One push button wired to positive with a pull-down. Call update() once per loop, then ask about edges and holds.
class Button {
public:
  explicit Button(uint8_t pin) : pin(pin) {}

  void begin() {
    pinMode(pin, INPUT_PULLDOWN);
  }

  // Reads the pin and remembers the previous state, so wasPressed()/wasReleased() describe this update
  void update(unsigned long now) {
    lastState = state;
    state = digitalRead(pin);
    if (wasPressed()) {
      pressStart = now;
      lastRepeat = now;
      repeatInterval = INITIAL_REPEAT_MS;
    }
  }

  // Forget the current press, e.g. after waking up. The button counts as released.
  void reset() {
    state = LOW;
    lastState = LOW;
    ignoreUntilRelease = false;
  }

  // State as of the last update()
  bool isDown() const { return state == HIGH; }
  // State right now, for use outside the update loop
  bool readNow() const { return digitalRead(pin) == HIGH; }
  // Went down / up in the last update()
  bool wasPressed() const { return lastState == LOW && state == HIGH; }
  bool wasReleased() const { return lastState == HIGH && state == LOW; }
  // Time since the button last went down, also valid right after it was released
  unsigned long heldFor(unsigned long now) const { return now - pressStart; }

  // True once per repeat while held past the hold threshold, repeating faster the longer it's held
  bool repeatDue(unsigned long now) {
    if (!isDown() || heldFor(now) < HOLD_THRESHOLD_MS || now - lastRepeat < repeatInterval) {
      return false;
    }
    lastRepeat = now;
    if (repeatInterval > MIN_REPEAT_MS + REPEAT_STEP_DECREASE_MS) {
      repeatInterval -= REPEAT_STEP_DECREASE_MS;
    } else {
      repeatInterval = MIN_REPEAT_MS;
    }
    return true;
  }

  // Set when this press was used for something else (a chord), so it shouldn't change anything until released
  bool ignoreUntilRelease = false;

private:
  uint8_t pin;
  bool state = LOW;
  bool lastState = LOW;
  unsigned long pressStart = 0;
  unsigned long lastRepeat = 0;
  unsigned long repeatInterval = INITIAL_REPEAT_MS;
};
