#pragma once

#include "page.h"
#include "bounded_value.h"
#include "../delta_tracker.h"

// A "+X|+Y" counter (each -99 to 99), e.g. for +1/+1 counters. Up/down change both, unless selection mode
// picks X (left) or Y (right). The delta shows the change of Y when both change.
class XYCounterPage : public Page {
public:
  XYCounterPage(DeltaTracker &delta, const Selection &selection) : delta(delta), selection(selection) {}

  const char *type() const override { return "xy_counter"; }

  bool onRelease(Direction direction, bool shortPress) override;
  bool onRepeat(Direction direction) override;
  bool supportsSelection() const override { return true; }

  void render(Renderer &renderer) const override;

  void save(JsonObject state) const override;
  void load(JsonObjectConst state) override;

private:
  // Adds or subtracts amount, depending on direction, to X, Y or both (see the class comment), and records
  // the actual change in the delta
  void step(Direction direction, int amount);

  DeltaTracker &delta;
  const Selection &selection;
  BoundedValue x{-99, 99};
  BoundedValue y{-99, 99};
};
