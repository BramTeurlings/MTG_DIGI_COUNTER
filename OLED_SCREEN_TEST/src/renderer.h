#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "input/input_handler.h"

class Battery;

// Owns the OLED and draws every UI element. Pages call the draw* methods between clear() and show().
class Renderer {
public:
  Renderer();

  // Starts I2C and the display, and skips the Adafruit splash screen
  void begin();

  // Clears the frame buffer, the draw* methods draw into it and show() sends it to the display
  void clear();
  void show();

  // 0-127, higher is brighter
  void setContrast(uint8_t contrast);
  // Turns the panel on or off, what it shows is kept
  void displayOn();
  void displayOff();

  // A mana icon on the left and a big number on the right; selection mode underlines one of them
  void drawIconCounter(int iconIndex, int value, bool selecting, SelectedItem selected);
  // "+x|+y", selection mode underlines x or y
  void drawXYCounter(int x, int y, bool selecting, SelectedItem selected);
  // The recent net change in the bottom left, e.g. "+3" or "-2". Only call it with a change other than 0.
  void drawDelta(int delta);
  // Brightness bar on the left, power button on the right
  void drawSettings(int contrastLevel, int maxContrastLevel);
  // One box per page in the bottom right, filled for the current page
  void drawPageIndicator(int pageCount, int currentPage);
  void drawBattery(const Battery &battery);

private:
  Adafruit_SH1107 display;
};
