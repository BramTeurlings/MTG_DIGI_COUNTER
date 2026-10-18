#pragma once

#include "page.h"
#include "bounded_value.h"
#include "../delta_tracker.h"

// A single counter (0-99) with a mana icon. In selection mode the left item is the icon, the right item the counter.
class CounterPage : public Page {
public:
  CounterPage(DeltaTracker &delta, const Selection &selection) : delta(delta), selection(selection) {}

  const char *type() const override { return "counter"; }

  bool onPress(Direction direction) override;
  bool onRelease(Direction direction, bool shortPress) override;
  bool onRepeat(Direction direction) override;
  bool supportsSelection() const override { return true; }

  void render(Renderer &renderer) const override;

  void save(JsonObject state) const override;
  void load(JsonObjectConst state) override;

private:
  // Whether selection mode has the icon selected, then up/down change the icon instead of the counter
  bool iconSelected() const { return selection.active && selection.item == SelectedItem::Left; }
  // Goes to the next (up) or previous (down) mana icon, wrapping around
  void cycleIcon(Direction direction);
  // Adds or subtracts amount, depending on direction, and records the actual change in the delta
  void step(Direction direction, int amount);

  DeltaTracker &delta;
  const Selection &selection;
  int iconIndex = 0;
  BoundedValue value{0, 99};
};
