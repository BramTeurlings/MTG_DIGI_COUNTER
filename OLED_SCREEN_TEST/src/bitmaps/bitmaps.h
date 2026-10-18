#pragma once

#include <Arduino.h>

struct Icon {
  const unsigned char *bitmap;
  uint8_t width;
  uint8_t height;
};

// Mana icons shown next to a counter, 24x24px
constexpr uint8_t ICON_SIZE = 24;
extern const Icon MANA_ICONS[];
extern const int NUM_MANA_ICONS;

// Settings page icons, 32x32px
constexpr uint8_t SETTINGS_ICON_SIZE = 32;
extern const unsigned char epd_bitmap_brightness_8bit[];
extern const unsigned char epd_bitmap_power_8bit[];

// Battery outline, 16x8px
constexpr uint8_t BATTERY_ICON_WIDTH = 16;
constexpr uint8_t BATTERY_ICON_HEIGHT = 8;
extern const unsigned char battery_8bit[];
