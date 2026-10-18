#pragma once

// All pins, timings and other tunables in one place.

#include <Arduino.h>

// System
constexpr unsigned long SERIAL_BAUDRATE = 115200;
constexpr uint32_t SYS_CLOCK_KHZ = 200000; // 200000 kHz for debugging but 18000 kHz for release.
constexpr uint8_t DEBUG_LED_PIN = 17;      // On while awake

// Display
constexpr uint8_t SCREEN_WIDTH = 64;    // OLED display width, in pixels
constexpr uint8_t SCREEN_HEIGHT = 128;  // OLED display height, in pixels
constexpr int8_t SCREEN_RESET_PIN = -1; // can set an oled reset pin if desired
constexpr uint8_t SCREEN_ROTATION = 3;  // rotate so it's 128 wide × 64 tall
constexpr uint8_t SCREEN_SDA_PIN = 20;
constexpr uint8_t SCREEN_SCL_PIN = 21;
constexpr uint8_t SCREEN_ADDRESS = 0x3C;

// Buttons, all wired to positive with a pull-down
constexpr uint8_t UP_BUTTON_PIN = 1;   // GP1: +, contrast on the settings page, picks the left item in selection mode
constexpr uint8_t DOWN_BUTTON_PIN = 2; // GP2: -, power off on the settings page, picks the right item in selection mode
constexpr uint8_t MENU_BUTTON_PIN = 3; // GP3: tap for next page, hold + up/down for selection mode, hold to power off

// Battery
constexpr uint8_t BATTERY_PIN = 26;        // Pin to read out current battery level
constexpr uint8_t BATTERY_ENABLE_PIN = 10; // Pin to enable reading the current battery level when pulled high

// Input timings (ms)
constexpr unsigned long DEBOUNCE_MS = 10;
constexpr unsigned long HOLD_THRESHOLD_MS = 1000;      // 1s before a held button starts repeating
constexpr unsigned long INITIAL_REPEAT_MS = 1000;      // first repeat after 1s
constexpr unsigned long REPEAT_STEP_DECREASE_MS = 200; // speed up each repeat
constexpr unsigned long MIN_REPEAT_MS = 200;           // fastest repeat speed
constexpr unsigned long MENU_TAP_MS = 500;             // shorter menu presses count as a tap
constexpr unsigned long POWER_HOLD_MS = 3000;          // holding the menu button this long powers off

// Other timings (ms)
constexpr unsigned long DELTA_DISPLAY_MS = 2000; // how long the recent change stays visible
constexpr unsigned long IDLE_DORMANT_MS = 30000; // no input this long makes the CPU go dormant, the display stays on

// Counter steps
constexpr int SHORT_PRESS_STEP = 1;
constexpr int REPEAT_STEP = 10;
