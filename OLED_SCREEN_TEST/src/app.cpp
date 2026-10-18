#include "app.h"
#include "config.h"
#include "power.h"
#include "storage.h"

App::App()
    : power(renderer, input),
      counter1(delta, selection),
      counter2(delta, selection),
      xyCounter(delta, selection),
      settings(renderer, [this]() { powerOff(); }),
      pages{&counter1, &counter2, &xyCounter, &settings} {}

void App::begin() {
  input.begin();
  battery.begin();

  // Todo: Remove the debug logic to toggle the LED
  pinMode(DEBUG_LED_PIN, OUTPUT);
  digitalWrite(DEBUG_LED_PIN, HIGH);

  delay(250); // wait for the OLED to power up

  storage::begin();
  Serial.println("MTG Counter Initialized.");

  renderer.begin();

  // Load saved state from flash and display it
  storage::load(pages, NUM_PAGES, currentPageIndex);
  battery.update();
  render();
}

void App::update() {
  bool redraw = input.update(*this);

  if (delta.update(millis())) {
    redraw = true;
    // save(); // Save once the changes have settled
  }

  battery.update();

  if (redraw) {
    render();
  }

  // Go dormant after a while without input, the display keeps showing the current screen
  if (input.anyButtonPressed()) {
    lastInputTime = millis();
  } else if (millis() - lastInputTime > IDLE_DORMANT_MS) {
    Serial.println("No input for a while, going dormant...");
    power.sleepUntilButtonPress();
    onWokeUp();
  }

  delay(DEBOUNCE_MS);
}

bool App::onPress(Direction direction) {
  return currentPage().onPress(direction);
}

bool App::onRelease(Direction direction, bool shortPress) {
  bool changed = currentPage().onRelease(direction, shortPress);
  if (changed) {
    delay(DEBOUNCE_MS);
  }
  return changed;
}

bool App::onRepeat(Direction direction) {
  bool changed = currentPage().onRepeat(direction);
  if (changed) {
    delay(DEBOUNCE_MS);
  }
  return changed;
}

bool App::onMenuTap() {
  // A tap leaves selection mode, otherwise it goes to the next page
  if (selection.active) {
    selection.active = false;
  } else {
    nextPage();
  }
  return true;
}

bool App::onSelect(SelectedItem item) {
  if (!currentPage().supportsSelection()) {
    return false;
  }
  selection.active = true;
  selection.item = item;
  return true;
}

bool App::onPowerHold() {
  powerOff();
  return true;
}

void App::nextPage() {
  currentPageIndex = (currentPageIndex + 1) % NUM_PAGES;
  delta.reset();
  // save(); // All save() calls are commented out while debugging to spare the flash
}

void App::render() {
  renderer.clear();
  currentPage().render(renderer);
  if (delta.isVisible()) {
    renderer.drawDelta(delta.value());
  }
  renderer.drawPageIndicator(NUM_PAGES, currentPageIndex);
  renderer.drawBattery(battery);
  renderer.show();
}

void App::save() {
  storage::save(pages, NUM_PAGES, currentPageIndex);
}

void App::powerOff() {
  // save();
  power.powerOff();
  onWokeUp();
}

void App::onWokeUp() {
  delta.reset();
  lastInputTime = millis();
}
