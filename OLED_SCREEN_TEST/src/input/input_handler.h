#pragma once

#include <Arduino.h>
#include "button.h"

// +1 for the up button, -1 for the down button
enum class Direction : int8_t { Up = 1, Down = -1 };

// Left = the icon or X, right = the number or Y
enum class SelectedItem : uint8_t { Left, Right };

// Receives the input events. Each method returns true when the screen needs to be redrawn.
class InputListener {
public:
  virtual bool onPress(Direction direction) = 0;
  virtual bool onRelease(Direction direction, bool shortPress) = 0;
  virtual bool onRepeat(Direction direction) = 0;
  virtual bool onMenuTap() = 0;
  virtual bool onSelect(SelectedItem item) = 0;
  virtual bool onPowerHold() = 0;

protected:
  ~InputListener() = default;
};

// Turns the raw state of the three buttons into input events:
// - up/down: press, release (short or long) and repeat while held
// - menu tap: a short press of the menu button on its own
// - select: up/down pressed while the menu button is held; that press changes nothing else, even after
//   the menu button is let go, until it's released
// - power hold: the menu button held for POWER_HOLD_MS
// Up and down pressed together cancel both until each is released.
class InputHandler {
public:
  InputHandler();

  // Sets up the button pins
  void begin();

  // Reads the buttons and sends events to the listener. Returns true when the screen needs to be redrawn.
  bool update(InputListener &listener);

  // Forget all presses, e.g. after waking up
  void reset();

  // Reads the pins right now, so it also works outside update()
  bool anyButtonPressed() const;

  // Blocks until no button is pressed anymore, debounced
  void waitUntilReleased() const;

private:
  // Turns the state of the up or down button into select, press, release and repeat events. Returns true to redraw.
  bool updateUpDown(Button &button, Direction direction, InputListener &listener, unsigned long now);

  Button up;
  Button down;
  Button menu;

  // Set when up or down is pressed while the menu button is held, so releasing the menu button quickly afterwards
  // isn't treated as a tap (which would immediately leave the selection mode that was just entered).
  bool otherPressedDuringMenuHold = false;
};
