#pragma once

#include <Arduino.h>
#include "config.h"

// The recent net change shown under a counter, e.g. "+3". It will disappear `DELTA_DISPLAY_MS` after the last change
class DeltaTracker {
public:
  // Adds a change to the net change and (re)starts the time it stays visible. A change of 0 also restarts it.
  void record(int change) {
    netChange += change;
    lastChangeTime = millis();
    visible = true;
  }

  // Hides the delta and starts counting from 0 again
  void reset() {
    netChange = 0;
    visible = false;
  }

  // Returns true when the delta just disappeared, i.e. the changes have settled and the screen needs a redraw
  bool update(unsigned long now) {
    if (visible && now - lastChangeTime > DELTA_DISPLAY_MS) {
      reset();
      return true;
    }
    return false;
  }

  // Whether there's a net change other than 0 to show
  bool isVisible() const { return visible && netChange != 0; }
  // The net change since the last reset, negative for a decrease
  int value() const { return netChange; }

private:
  int netChange = 0;
  unsigned long lastChangeTime = 0;
  bool visible = false;
};
