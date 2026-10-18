// MTG counter on a SparkFun Pro Micro RP2040 with a 128x64 SH1107 OLED and three buttons.
// See app.h for how it fits together, and config.h for the pins and timings.

#include <Arduino.h>
#include "app.h"
#include "config.h"

App app;

void setup() {
  Serial.begin(SERIAL_BAUDRATE);
  set_sys_clock_khz(SYS_CLOCK_KHZ, true);
  app.begin();
}

void loop() {
  app.update();
}
