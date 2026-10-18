#pragma once

#include <functional>
#include "page.h"

// Up cycles the display brightness, down powers the device off. Both act when the button is released.
class SettingsPage : public Page {
public:
  SettingsPage(Renderer &renderer, std::function<void()> powerOff) : renderer(renderer), powerOff(powerOff) {}

  const char *type() const override { return "settings"; }

  bool onRelease(Direction direction, bool shortPress) override;

  void render(Renderer &renderer) const override;

  void save(JsonObject state) const override;
  void load(JsonObjectConst state) override;

private:
  static constexpr int NUM_CONTRAST_LEVELS = 4;
  static constexpr uint8_t CONTRAST_LEVELS[NUM_CONTRAST_LEVELS] = {0, 42, 85, 127}; // approx 0%, 33%, 66%, 100%

  // Sends the current contrast level to the display
  void applyContrast();

  Renderer &renderer;
  std::function<void()> powerOff;
  int contrastLevel = NUM_CONTRAST_LEVELS - 1; // start at 100%
};
