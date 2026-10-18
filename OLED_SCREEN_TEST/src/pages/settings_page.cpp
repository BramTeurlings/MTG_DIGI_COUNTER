#include "settings_page.h"

// Long presses count too, the settings don't repeat while held
bool SettingsPage::onRelease(Direction direction, bool /*shortPress*/) {
  if (direction == Direction::Up) {
    contrastLevel = (contrastLevel + 1) % NUM_CONTRAST_LEVELS;
    applyContrast();
  } else {
    powerOff();
  }
  return true;
}

void SettingsPage::applyContrast() {
  renderer.setContrast(CONTRAST_LEVELS[contrastLevel]);
}

void SettingsPage::render(Renderer &renderer) const {
  renderer.drawSettings(contrastLevel, NUM_CONTRAST_LEVELS - 1);
}

void SettingsPage::save(JsonObject state) const {
  state["contrast"] = contrastLevel;
}

void SettingsPage::load(JsonObjectConst state) {
  contrastLevel = constrain(state["contrast"] | (NUM_CONTRAST_LEVELS - 1), 0, NUM_CONTRAST_LEVELS - 1);
  applyContrast();
}
