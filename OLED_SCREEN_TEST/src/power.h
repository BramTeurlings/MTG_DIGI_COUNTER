#pragma once

#include "input/input_handler.h"
#include "renderer.h"

// Puts the device to sleep. Both ways of sleeping put the CPU in dormant mode until any button is pressed, then
// restore the clocks and USB, and swallow the wake-up press so it doesn't also change anything.
class Power {
public:
  Power(Renderer &renderer, InputHandler &input) : renderer(renderer), input(input) {}

  // Clears and turns off the display, then sleeps until a button is pressed and turns the display back on
  void powerOff();

  // Sleeps until a button is pressed, the display keeps showing its last image while sleeping
  void sleepUntilButtonPress();

private:
  Renderer &renderer;
  InputHandler &input;
};
