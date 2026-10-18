#include "xy_counter_page.h"
#include "../config.h"

bool XYCounterPage::onRelease(Direction direction, bool shortPress) {
  if (!shortPress) {
    return false;
  }
  step(direction, SHORT_PRESS_STEP);
  return true;
}

bool XYCounterPage::onRepeat(Direction direction) {
  step(direction, REPEAT_STEP);
  return true;
}

void XYCounterPage::step(Direction direction, int amount) {
  int change = static_cast<int>(direction) * amount;
  if (!selection.active) {
    x.add(change);
    delta.record(y.add(change));
  } else if (selection.item == SelectedItem::Left) {
    delta.record(x.add(change));
  } else {
    delta.record(y.add(change));
  }
}

void XYCounterPage::render(Renderer &renderer) const {
  renderer.drawXYCounter(x.get(), y.get(), selection.active, selection.item);
}

void XYCounterPage::save(JsonObject state) const {
  state["valueX"] = x.get();
  state["valueY"] = y.get();
}

void XYCounterPage::load(JsonObjectConst state) {
  x.set(state["valueX"] | 0);
  y.set(state["valueY"] | 0);
}
