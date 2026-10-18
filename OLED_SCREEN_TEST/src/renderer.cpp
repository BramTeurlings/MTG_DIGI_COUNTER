#include "renderer.h"
#include "battery.h"
#include "bitmaps/bitmaps.h"
#include "config.h"

namespace {

String formatAsCounter(int x, int y) {
  String result = "";

  // Add sign and value for x
  if (x >= 0) {
    result += "+";
  }
  result += String(x);

  result += "|";

  // Add sign and value for y
  if (y >= 0) {
    result += "+";
  }
  result += String(y);

  return result;
}

} // namespace

Renderer::Renderer() : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, SCREEN_RESET_PIN, 1000000, 100000) {}

void Renderer::begin() {
  Wire.setSDA(SCREEN_SDA_PIN);
  Wire.setSCL(SCREEN_SCL_PIN);

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.begin(SCREEN_ADDRESS, true);
  display.display();
  delay(100);
  display.setRotation(SCREEN_ROTATION);

  // Clear the buffer.
  display.clearDisplay();

  // draw a single pixel
  display.drawPixel(10, 10, SH110X_WHITE);
  // Show the display buffer on the hardware.
  // NOTE: You _must_ call display after making any drawing commands
  // to make them visible on the display hardware!
  display.display();
  delay(10);
  display.clearDisplay();
}

void Renderer::clear() {
  display.clearDisplay();
}

void Renderer::show() {
  display.display();
}

void Renderer::setContrast(uint8_t contrast) {
  display.setContrast(contrast);
}

void Renderer::displayOn() {
  display.oled_command(SH110X_DISPLAYON);
}

void Renderer::displayOff() {
  display.oled_command(SH110X_DISPLAYOFF);
}

void Renderer::drawIconCounter(int iconIndex, int value, bool selecting, SelectedItem selected) {
  display.setTextSize(5);
  display.setTextColor(SH110X_WHITE);

  // Set up variables for getting text bounds
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(String(value), 0, 0, &x1, &y1, &w, &h);

  // Draw icon on left
  const Icon &icon = MANA_ICONS[iconIndex];
  int iconX = 0;
  int iconY = (h / 2) - (ICON_SIZE / 2) - 3; // Arbitrary 3 pixel offset (why???)
  display.drawBitmap(iconX, iconY, icon.bitmap, icon.width, icon.height, SH110X_WHITE);

  // Draw number on the right
  int numberX = 30;
  display.setCursor(numberX, 0); // leave space for icon
  display.print(value);

  // Underline logic
  if (selecting) {
    int underlineY = h + 3; // 3px below text baseline
    if (selected == SelectedItem::Left) {
      // Underline icon
      display.drawLine(iconX, underlineY, iconX + ICON_SIZE, underlineY, SH110X_WHITE);
    } else {
      // Underline number
      display.drawLine(numberX, underlineY, numberX + w, underlineY, SH110X_WHITE);
    }
  }
}

void Renderer::drawXYCounter(int x, int y, bool selecting, SelectedItem selected) {
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  if (abs(x) < 10 && abs(y) < 10) {
    display.setTextSize(4);
  } else {
    display.setTextSize(3);
  }
  display.print(formatAsCounter(x, y));

  // Underline logic, the sign is always one character wide so "+" stands in for "-" when measuring
  if (selecting) {
    int16_t x1, y1;
    uint16_t w, h;
    if (selected == SelectedItem::Left) {
      // Underline X
      String leftPart = "+" + String(abs(x));
      display.getTextBounds(leftPart, 0, 0, &x1, &y1, &w, &h);
      int underlineY = h + 3; // 3px below baseline
      display.drawLine(x1, underlineY, x1 + w, underlineY, SH110X_WHITE);
    } else {
      // Underline Y
      String leftPartOffset = "+" + String(abs(x)) + "|";
      display.getTextBounds(leftPartOffset, 0, 0, &x1, &y1, &w, &h);
      int offsetX = w; // x position where +y starts
      String rightPart = "+" + String(abs(y));
      display.getTextBounds(rightPart, offsetX, 0, &x1, &y1, &w, &h);
      int underlineY = h + 3; // 3px below baseline
      display.drawLine(x1, underlineY, x1 + w, underlineY, SH110X_WHITE);
    }
  }
}

void Renderer::drawDelta(int delta) {
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 50);
  // Negative numbers already print their own "-"
  if (delta > 0) {
    display.print("+");
  }
  display.print(delta);
}

void Renderer::drawSettings(int contrastLevel, int maxContrastLevel) {
  // Left: contrast icon with a bar showing the current level
  const int barMaxHeight = 45;
  display.drawRect(5, 5, 45, 45, SH110X_WHITE);
  display.drawRect(49, 5, 7, barMaxHeight, SH110X_WHITE);
  int barHeight = min(barMaxHeight / maxContrastLevel * contrastLevel, barMaxHeight);
  if (contrastLevel != 0) {
    display.fillRect(50, 50 - barHeight, 5, barHeight, SH110X_WHITE);
  }
  display.drawBitmap(11, 11, epd_bitmap_brightness_8bit, SETTINGS_ICON_SIZE, SETTINGS_ICON_SIZE, SH110X_WHITE);

  // Right: power icon
  display.drawRect(75, 5, 45, 45, SH110X_WHITE);
  display.drawBitmap(81, 11, epd_bitmap_power_8bit, SETTINGS_ICON_SIZE, SETTINGS_ICON_SIZE, SH110X_WHITE);
}

void Renderer::drawPageIndicator(int pageCount, int currentPage) {
  int boxSize = 6;
  int spacing = 10;
  int initialCursor = 98;
  for (int i = 0; i < pageCount; i++) {
    int x = initialCursor - i * spacing;
    if (i == currentPage) {
      display.fillRect(x, 55, boxSize, boxSize, SH110X_WHITE);
    } else {
      display.drawRect(x, 55, boxSize, boxSize, SH110X_WHITE);
    }
  }
}

void Renderer::drawBattery(const Battery &battery) {
  int batteryX = 108;
  int batteryY = 54;
  int barWidth = battery.scaledTo(14); // There are 14 pixels available inside the outline
  display.drawBitmap(batteryX, batteryY, battery_8bit, BATTERY_ICON_WIDTH, BATTERY_ICON_HEIGHT, SH110X_WHITE);
  display.fillRect(batteryX + 1, batteryY + 1, barWidth, 6, SH110X_WHITE);
}
