#include "counter_page.h"
#include "../bitmaps/bitmaps.h"
#include "../config.h"

bool CounterPage::onPress(Direction direction) {
  // The icon changes as soon as the button goes down, the counter on release or while held
  if (!iconSelected()) {
    return false;
  }
  cycleIcon(direction);
  return true;
}

bool CounterPage::onRelease(Direction direction, bool shortPress) {
  if (iconSelected() || !shortPress) {
    return false;
  }
  step(direction, SHORT_PRESS_STEP);
  return true;
}

bool CounterPage::onRepeat(Direction direction) {
  if (iconSelected()) {
    return false;
  }
  step(direction, REPEAT_STEP);
  return true;
}

void CounterPage::cycleIcon(Direction direction) {
  iconIndex = (iconIndex + static_cast<int>(direction) + NUM_MANA_ICONS) % NUM_MANA_ICONS;
}

void CounterPage::step(Direction direction, int amount) {
  delta.record(value.add(static_cast<int>(direction) * amount));
}

void CounterPage::render(Renderer &renderer) const {
  renderer.drawIconCounter(iconIndex, value.get(), selection.active, selection.item);
}

void CounterPage::save(JsonObject state) const {
  state["value"] = value.get();
  state["icon"] = iconIndex;
}

void CounterPage::load(JsonObjectConst state) {
  value.set(state["value"] | 0);
  iconIndex = constrain(state["icon"] | 0, 0, NUM_MANA_ICONS - 1);
}
