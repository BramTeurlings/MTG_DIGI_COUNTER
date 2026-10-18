#include "input_handler.h"
#include "../config.h"

InputHandler::InputHandler() : up(UP_BUTTON_PIN), down(DOWN_BUTTON_PIN), menu(MENU_BUTTON_PIN) {}

void InputHandler::begin() {
  up.begin();
  down.begin();
  menu.begin();
}

void InputHandler::reset() {
  up.reset();
  down.reset();
  menu.reset();
}

bool InputHandler::anyButtonPressed() const {
  return up.readNow() || down.readNow() || menu.readNow();
}

void InputHandler::waitUntilReleased() const {
  while (anyButtonPressed()) {
    delay(10);
  }
  delay(50); // debounce the release
}

bool InputHandler::update(InputListener &listener) {
  unsigned long now = millis();
  up.update(now);
  down.update(now);
  menu.update(now);

  bool redraw = false;

  // Menu button
  if (menu.wasPressed()) {
    otherPressedDuringMenuHold = false; // reset for this press cycle
  }
  if (menu.wasReleased() && !otherPressedDuringMenuHold && menu.heldFor(now) < MENU_TAP_MS) {
    redraw |= listener.onMenuTap();
  }
  if (menu.isDown() && menu.heldFor(now) > POWER_HOLD_MS) {
    // The listener resets us after waking up, so the other buttons have nothing left to handle
    listener.onPowerHold();
    return true;
  }

  // Up & down pressed together cancel both
  if (up.isDown() && down.isDown()) {
    up.ignoreUntilRelease = true;
    down.ignoreUntilRelease = true;
  }

  redraw |= updateUpDown(up, Direction::Up, listener, now);
  redraw |= updateUpDown(down, Direction::Down, listener, now);
  return redraw;
}

bool InputHandler::updateUpDown(Button &button, Direction direction, InputListener &listener, unsigned long now) {
  bool redraw = false;

  // While the menu button is held, up & down only pick the item for selection mode
  if (menu.isDown()) {
    if (button.wasPressed()) {
      otherPressedDuringMenuHold = true;
      redraw = listener.onSelect(direction == Direction::Up ? SelectedItem::Left : SelectedItem::Right);
    }
    if (button.isDown()) {
      button.ignoreUntilRelease = true;
    }
    return redraw;
  }

  // Swallow a press that was used for something else, including its hold and release
  if (button.ignoreUntilRelease) {
    if (!button.isDown()) {
      button.ignoreUntilRelease = false;
    }
    return false;
  }

  if (button.wasPressed()) {
    redraw |= listener.onPress(direction);
  }
  if (button.wasReleased()) {
    redraw |= listener.onRelease(direction, button.heldFor(now) < HOLD_THRESHOLD_MS);
  }
  if (button.repeatDue(now)) {
    redraw |= listener.onRepeat(direction);
  }
  return redraw;
}
