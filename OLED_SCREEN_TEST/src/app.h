#pragma once

#include "battery.h"
#include "delta_tracker.h"
#include "input/input_handler.h"
#include "pages/counter_page.h"
#include "pages/page.h"
#include "pages/settings_page.h"
#include "pages/xy_counter_page.h"
#include "power.h"
#include "renderer.h"

// The counter device: owns the pages and hardware, routes input to the current page, renders and sleeps.
class App : public InputListener {
public:
  App();

  // Sets up the hardware, loads the saved state and draws the first screen. Call from setup().
  void begin();
  // Handles input, the delta timeout, the battery reading, redraws when needed and sleeps when idle. Call from loop().
  void update();

  // InputListener, see input_handler.h
  bool onPress(Direction direction) override;
  bool onRelease(Direction direction, bool shortPress) override;
  bool onRepeat(Direction direction) override;
  bool onMenuTap() override;
  bool onSelect(SelectedItem item) override;
  bool onPowerHold() override;

private:
  static constexpr int NUM_PAGES = 4;

  Page &currentPage() { return *pages[currentPageIndex]; }
  void nextPage();
  // Draws the whole screen: the current page plus the delta, page indicator and battery
  void render();
  // Writes the state of all pages to flash
  void save();

  // Saves, then turns the display off and sleeps until a button is pressed
  void powerOff();
  // Resets the state that shouldn't survive sleeping: the delta and the idle timer
  void onWokeUp();

  Renderer renderer;
  InputHandler input;
  Power power;
  Battery battery;
  DeltaTracker delta;
  Selection selection;

  CounterPage counter1;
  CounterPage counter2;
  XYCounterPage xyCounter;
  SettingsPage settings;
  Page *const pages[NUM_PAGES];
  int currentPageIndex = 0;

  unsigned long lastInputTime = 0;
};
