#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include "../input/input_handler.h"
#include "../renderer.h"

// Selection mode lets the up/down buttons change only the left or the right item of a page
struct Selection {
  bool active = false;
  SelectedItem item = SelectedItem::Left;
};

// One screen of the counter. The App sends the up/down input of the current page to it.
class Page {
public:
  virtual ~Page() = default;

  // Stored with the page's state, so a saved state is only loaded into the same type of page
  virtual const char *type() const = 0;

  // Up/down input, each returns true when the screen needs to be redrawn
  virtual bool onPress(Direction /*direction*/) { return false; }
  virtual bool onRelease(Direction /*direction*/, bool /*shortPress*/) { return false; }
  virtual bool onRepeat(Direction /*direction*/) { return false; }

  // Whether holding the menu button and pressing up/down enters selection mode on this page
  virtual bool supportsSelection() const { return false; }

  // Draws the page itself, the App draws the shared parts (delta, page indicator, battery)
  virtual void render(Renderer &renderer) const = 0;

  virtual void save(JsonObject state) const = 0;
  virtual void load(JsonObjectConst state) = 0;
};
