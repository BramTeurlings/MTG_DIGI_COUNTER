/*********************************************************************
  This is an example for our Monochrome OLEDs based on SH1107 drivers

  This example is for a 128x128 size display using I2C to communicate

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada  for Adafruit Industries.
  BSD license, check license.txt for more information
  All text above, and the splash screen must be included in any redistribution
*********************************************************************/



#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

#define SCREEN_WIDTH 64  // OLED display width, in pixels
#define SCREEN_HEIGHT 128 // OLED display height, in pixels
#define OLED_RESET -1     // can set an oled reset pin if desired
Adafruit_SH1107 display = Adafruit_SH1107(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET, 1000000, 100000);


#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define ICON_HEIGHT   24
#define ICON_WIDTH    24


// 'w_8bit_bw', 24x24px
const unsigned char epd_bitmap_w_8bit_bw [] PROGMEM = {
0b00000111, 0b11111111, 0b11100000,
0b00011111, 0b11110001, 0b11111000,
0b00111111, 0b11100111, 0b11111100,
0b01110011, 0b11000111, 0b11111110,
0b01100111, 0b11000011, 0b10001110,
0b11100011, 0b11100011, 0b00010111,
0b11100000, 0b11111111, 0b00111111,
0b11110001, 0b11111111, 0b00111111,
0b11111111, 0b11100111, 0b11111111,
0b10111111, 0b10111101, 0b11111111,
0b10110011, 0b11100111, 0b11011111,
0b10000011, 0b01000010, 0b11000011,
0b11000011, 0b01000010, 0b11000001,
0b11111011, 0b11100111, 0b11001101,
0b11111111, 0b10111101, 0b11111101,
0b11111111, 0b11100111, 0b11111111,
0b11111100, 0b11111111, 0b10001111,
0b11111100, 0b11111111, 0b00000111,
0b11101000, 0b11000111, 0b11000111,
0b01110001, 0b11000011, 0b11100110,
0b01111111, 0b11100011, 0b11001110,
0b00111111, 0b11100111, 0b11111100,
0b00011111, 0b10001111, 0b11111000,
0b00000111, 0b11111111, 0b11100000
};
// 'b_8bit_bw', 24x24px
const unsigned char epd_bitmap_b_8bit_bw [] PROGMEM = {
0b00000111, 0b11111111, 0b11100000, 
0b00011111, 0b00000000, 0b11111000,
0b00111100, 0b00000000, 0b00011100,
0b01111100, 0b00000000, 0b00011110,
0b01110100, 0b00000000, 0b00100110,
0b11100110, 0b00000000, 0b11000011,
0b11000011, 0b00000000, 0b00000011,
0b11000001, 0b10000000, 0b00000011,
0b10000000, 0b00000000, 0b00000001,
0b10001111, 0b00000000, 0b01110001,
0b10011111, 0b10000000, 0b11111101,
0b10011111, 0b10000001, 0b11111101,
0b10011111, 0b11000011, 0b11111101,
0b10001111, 0b10000001, 0b11110001,
0b11000000, 0b00110100, 0b00000011,
0b11100000, 0b00110100, 0b00000111,
0b11111100, 0b00110100, 0b00111111,
0b11111100, 0b00111100, 0b00111111,
0b11111100, 0b00000000, 0b00111111,
0b01111100, 0b01000010, 0b00111110,
0b01111100, 0b01001010, 0b00111110,
0b00111100, 0b01001010, 0b00111100,
0b00011111, 0b01001110, 0b01111000,
0b00000111, 0b11111111, 0b11100000
};
// 'c_8bit_bw', 24x24px
const unsigned char epd_bitmap_c_8bit_bw [] PROGMEM = {
0b00000111, 0b11111111, 0b11100000,
0b00011111, 0b11111111, 0b11111000,
0b00111111, 0b11100111, 0b11111100,
0b01111111, 0b11000011, 0b11111110,
0b01111111, 0b11000011, 0b11111110,
0b11111111, 0b10000001, 0b11111111,
0b11111111, 0b00011000, 0b11111111,
0b11111110, 0b00011000, 0b01111111,
0b11111100, 0b00111100, 0b00111111,
0b11111000, 0b01111110, 0b00011111,
0b11000001, 0b11111111, 0b10000011,
0b11000001, 0b11111111, 0b10000011,
0b10000011, 0b11111111, 0b11000001,
0b11000001, 0b11111111, 0b10000011,
0b11111000, 0b11111111, 0b00011111,
0b11111100, 0b01111110, 0b00111111,
0b11111110, 0b00111100, 0b01111111,
0b11111111, 0b00011000, 0b11111111,
0b11111111, 0b10000001, 0b11111111,
0b01111111, 0b11000011, 0b11111110,
0b01111111, 0b11000011, 0b11111110,
0b00111111, 0b11000011, 0b11111100,
0b00011111, 0b11111111, 0b11111000,
0b00000111, 0b11111111, 0b11100000
};
// 'g_8bit_bw', 24x24px
const unsigned char epd_bitmap_g_8bit_bw [] PROGMEM = {
0b00000111, 0b11111111, 0b11100000,
0b00011111, 0b10000011, 0b11111000,
0b00110011, 0b00000001, 0b10001100,
0b01110011, 0b00000001, 0b10000110,
0b01000000, 0b00000000, 0b00000010,
0b10000000, 0b00000000, 0b00000001,
0b10000000, 0b00100001, 0b10000001,
0b10000000, 0b00100001, 0b10000001,
0b10000000, 0b10000010, 0b00000001,
0b11000000, 0b01000000, 0b00000011,
0b10000011, 0b10000001, 0b11000001,
0b10000011, 0b10000001, 0b11000001,
0b10000000, 0b01000010, 0b00000001,
0b10000011, 0b00000000, 0b01000001,
0b11000111, 0b11000011, 0b11100011,
0b11111111, 0b11000011, 0b11110111,
0b11111111, 0b11000011, 0b11111111,
0b11111111, 0b11000011, 0b11111111,
0b11111111, 0b10000001, 0b11111111,
0b01111000, 0b00000000, 0b00011110,
0b01110000, 0b00000000, 0b01111110,
0b00110000, 0b00000000, 0b01110100,
0b00011111, 0b01011011, 0b00111000,
0b00000111, 0b11111111, 0b11100000
};
// 'u_8bit_bw', 24x24px
const unsigned char epd_bitmap_u_8bit_bw [] PROGMEM = {
0b00000111, 0b11111111, 0b11100000,
0b00011111, 0b11111100, 0b00111000,
0b00111111, 0b11110000, 0b01111100,
0b01111111, 0b11110000, 0b11111110,
0b01111111, 0b11000001, 0b11111110,
0b11111111, 0b10000001, 0b11111111,
0b11111111, 0b00000001, 0b11111111,
0b11111111, 0b00000000, 0b11111111,
0b11111111, 0b00000000, 0b01111111,
0b11111100, 0b00000000, 0b00111111,
0b11111100, 0b00000000, 0b00111111,
0b11111100, 0b00000000, 0b00111111,
0b11111000, 0b00000001, 0b10011111,
0b11111000, 0b00000001, 0b11011111,
0b11111000, 0b00000001, 0b11011111,
0b11111000, 0b00000001, 0b11011111,
0b11111000, 0b00000000, 0b11011111,
0b11111000, 0b00000000, 0b00011111,
0b11111100, 0b00000000, 0b00011111,
0b01111100, 0b00000000, 0b00111110,
0b01111110, 0b00000000, 0b01111110,
0b00111111, 0b00000000, 0b11111100,
0b00011111, 0b11000011, 0b11111000,
0b00000111, 0b11111111, 0b11100000
};
// 'r_8bit_bw', 24x24px
const unsigned char epd_bitmap_r_8bit_bw [] PROGMEM = {
0b00000111, 0b11111111, 0b11100000,
0b00011100, 0b00000001, 0b11111000,
0b00111111, 0b11100000, 0b00111100,
0b01111111, 0b11100000, 0b00110010,
0b01110001, 0b10001111, 0b00011110,
0b11000011, 0b11111111, 0b11001101,
0b10110111, 0b01000010, 0b11001101,
0b10111111, 0b01000011, 0b11000001,
0b11111111, 0b10001111, 0b10000001,
0b11111111, 0b11111111, 0b11100001,
0b11111111, 0b11111101, 0b00100001,
0b11111111, 0b11111101, 0b00000011,
0b11111111, 0b11000011, 0b10000011,
0b11111000, 0b00000011, 0b11000011,
0b11110000, 0b00001110, 0b00001111,
0b11100000, 0b00000000, 0b00011111,
0b11000000, 0b00000000, 0b00000011,
0b11000000, 0b00000000, 0b00000011,
0b11000000, 0b00000000, 0b00001111,
0b01000000, 0b00000001, 0b11111110,
0b01100000, 0b00000000, 0b00011110,
0b00110000, 0b00000000, 0b00011100,
0b00011100, 0b00000000, 0b11111000,
0b00000111, 0b11111111, 0b11100000
};


// Icon storage
#define ICON_SIZE 24

struct Icon {
  const unsigned char *bitmap;
  uint8_t width;
  uint8_t height;
};

static const Icon icons[] = {
  { epd_bitmap_w_8bit_bw, ICON_HEIGHT, ICON_HEIGHT },
  { epd_bitmap_u_8bit_bw, ICON_HEIGHT, ICON_HEIGHT },
  { epd_bitmap_b_8bit_bw, ICON_HEIGHT, ICON_HEIGHT },
  { epd_bitmap_r_8bit_bw, ICON_HEIGHT, ICON_HEIGHT },
  { epd_bitmap_g_8bit_bw, ICON_HEIGHT, ICON_HEIGHT },
  { epd_bitmap_c_8bit_bw, ICON_HEIGHT, ICON_HEIGHT },
  // Add more icons here later...
};

constexpr int NUM_ICONS = sizeof(icons) / sizeof(icons[0]);

// Defining pins for buttons
#define BUTTON_PIN1 1  // GP1
#define BUTTON_PIN2 2  // GP2
#define BUTTON_PIN3 3  // GP3

// Selection modes
static bool button3Held = false; // Todo: Refactor this because it's stupid and only used for the selectionmode
bool button3OtherPressedDuringHold = false;
unsigned long button3PressTime = 0;
const unsigned long BUTTON3_TAP_MS = 500; // ms threshold to distinguish tap vs hold
bool selectionMode = false;  
int selectedItem = 0;  // 0 = left, 1 = right

// Button state tracking
bool lastButtonState1 = LOW; // default LOW if using pull-down
bool lastButtonState2 = LOW; // default LOW if using pull-down
bool lastButtonState3 = LOW; // default LOW if using pull-down

// Holds the timestamp for the current press down action
unsigned long pressStart = 0;

// Hold settings
const unsigned long holdThreshold = 1000; // 1s before starting acceleration
const unsigned long initialRepeat = 1000; // first repeat after 1s
const unsigned long stepDecrease = 200;   // speed up each cycle
const unsigned long minInterval = 200;    // fastest speed

// For tracking repeat timing
unsigned long lastRepeatTime = 0;
unsigned long currentInterval = initialRepeat;

// Net change tracking
int netChange = 0;
unsigned long lastActionTime = 0;
const unsigned long changeDisplayDuration = 2000;
bool deltaVisible = false;

// Contrast modes
uint8_t contrastLevels[4] = {0, 42, 85, 127}; // approx 0%, 33%, 66%, 100%
int currentContrast = 3; // start at 100%

// Page system
const int NUM_PAGES = 4;
int counters[NUM_PAGES - 1] = {0, 0, 0}; // The value where we store the singlular number being displayed
int countersX[NUM_PAGES - 1] = {0, 0, 0}; // The value where we store the X number being displayed
int countersY[NUM_PAGES - 1] = {0, 0, 0}; // The value where we store the Y number being displayed
int selectedIcons[NUM_PAGES - 1] = {0, 0, 0}; // Index of icon per page
int currentPage = 0;

// File handling
const char* filename = "/data.json";

void saveState() {
  // Create a JSON document
  JsonDocument doc;
  doc["currentPage"] = currentPage;
  doc["contrast"] = currentContrast;

  JsonArray arr = doc["pages"].to<JsonArray>();
  arr.clear(); // make sure it’s empty before filling
  for (int i = 0; i < NUM_PAGES - 1; i++) {
    JsonObject page = arr.add<JsonObject>();
    page["value"] = counters[i];
    page["valueX"] = countersX[i];
    page["valueY"] = countersY[i];
    page["icon"] = selectedIcons[i];
  }

  File file = LittleFS.open(filename, "w");
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  serializeJson(doc, file);
  file.close();
  Serial.println("State saved");
}

void loadState() {
  if (!LittleFS.exists(filename)) {
    Serial.println("No save file found, starting fresh.");
    return;
  }

  File file = LittleFS.open(filename, "r");
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, file);
  file.close();

  if (error) {
    Serial.println("Failed to parse JSON, starting fresh.");
    return;
  }

  currentPage = doc["currentPage"] | 0;
  currentContrast = doc["contrast"] | 3;

  JsonArray arr = doc["pages"];
  int i = 0;
  for (JsonObject page : arr) {
    if (i < NUM_PAGES - 1) {
      counters[i] = page["value"] | 0;
      countersX[i] = page["valueX"] | 0;
      countersY[i] = page["valueY"] | 0;
      selectedIcons[i] = page["icon"] | 0;
    }
    i++;
  }
}

void setup() {
  Serial.begin(115200);

  Wire.setSDA(20);
  Wire.setSCL(21);
  
  pinMode(BUTTON_PIN1, INPUT_PULLDOWN); // button to positive
  pinMode(BUTTON_PIN2, INPUT_PULLDOWN); // button to positive
  pinMode(BUTTON_PIN3, INPUT_PULLDOWN); // button to positive

 
  delay(250); // wait for the OLED to power up

  LittleFS.begin();
  Serial.println("MTG Counter Initialized.");

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.

  display.begin(0x3C, true); // Address 0x3D default
  //display.setContrast (0); // dim display 
 
  display.display();
  delay(100);
  display.setRotation(3);   // rotate so it’s 128 wide × 64 tall

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

  // Load saved state from flash and display the retrieved number
  loadState();
  renderScreen();
}

void mutateNumber(bool isPositive, int step) {
  int before = counters[currentPage];

  // Apply step
  if (isPositive) {
    counters[currentPage] += step;
  } else {
    counters[currentPage] -= step;
  }

  // Clamp
  if (counters[currentPage] > 99) counters[currentPage] = 99;
  if (counters[currentPage] < 0)  counters[currentPage] = 0;

  // Actual change is the difference between before and after
  int actualDelta = counters[currentPage] - before;
  netChange += actualDelta;

  // Keep time since last mutation.
  lastActionTime = millis();
  deltaVisible = true;

  renderScreen(); // update the screen with the new number
  // saveState();  // write to flash every change
  delay(10); // debounce delay
}

void switchPage(int direction) {
  currentPage += direction;
  if (currentPage < 0) currentPage = NUM_PAGES - 1;
  if (currentPage >= NUM_PAGES) currentPage = 0;

  netChange = 0;
  deltaVisible = false;
  lastActionTime = millis();

  renderScreen();
  // saveState(); Todo: Uncomment all saveState() calls later
}

void settingsAction(bool isLeft) {
  if (isLeft) {
    // Cycle contrast
    currentContrast = (currentContrast + 1) % 4;
    display.setContrast(contrastLevels[currentContrast]);
    Serial.print("Contrast set to ");
    Serial.println(contrastLevels[currentContrast]);
    saveState();
    renderScreen();
  } else {
    // Display a blank image on the display
    display.clearDisplay();
    display.display();
    // Todo: put MCU into sleep here if desired
    Serial.println("Todo: Powering off...");
  }
}

// Call this inside handleButton when button 3 is involved
void handleSelectionMode(int pin, bool state, bool lastState) {
  unsigned long now = millis();

  // Handle button 3 press/release/tap detection
  if (pin == BUTTON_PIN3) {
    // pressed down
    if (!lastState && state) {
      button3Held = true;
      button3PressTime = now;
      button3OtherPressedDuringHold = false; // reset for this press cycle -> Todo: pretty sure this var is bullshit and can be deleted
      return;
    } else if (lastState && !state) { // released
      unsigned long held = now - button3PressTime;
      button3Held = false;

      // If no other button was pressed while button3 was held and duration < TAP_MS => a "tap"
      if (!button3OtherPressedDuringHold && held < BUTTON3_TAP_MS) {
        // Treat as single-tap: if in selection mode exit it, otherwise do the normal page switch
        if (selectionMode) {
          selectionMode = false;
          renderScreen();
        } else {
          switchPage(1);
          lastState = state;
          return;
        }
      }
      // else: this was a hold (or a chord) — do not toggle selectionMode here.
      return;
    }
  }
}

// Increments or decrements the +x/+y counter. Limit set to +99/-99
void handleXYMutation(bool isX, bool isPositiveIncrementer, bool lastState, bool state) {
  if (lastState == LOW && state == HIGH) { 
    if (isPositiveIncrementer) {
      if (isX) {
        if (countersX[currentPage] + 1 < 100) {
          countersX[currentPage] += 1;
        } else {
          countersX[currentPage] = 99;
        }
      } else {
        if (countersY[currentPage] + 1 < 100) {
          countersY[currentPage] += 1;
        } else {
          countersY[currentPage] = 99;
        }
      }
    } else {
      if (isX) {
        if (countersX[currentPage] - 1 > -100) {
          countersX[currentPage] -= 1;
        } else {
          countersX[currentPage] = -99;
        }
      } else {
        if (countersY[currentPage] - 1 > -100) {
          countersY[currentPage] -= 1;
        } else {
          countersY[currentPage] = -99;
        }
      }
    }
    renderScreen();
  }
}

// Cycles the selected icons based on if input is positive or negative. Keep in mind NUM_ICONS is 1 bigger than ICONS because it's 0-indexed.
void cycleIcons(bool isPositiveIncrementer, bool lastState, bool state) {
  if (lastState == LOW && state == HIGH) { 
    if (isPositiveIncrementer) {
        if (selectedIcons[currentPage] < NUM_ICONS - 1) {
          selectedIcons[currentPage] += 1;
        } else {
          selectedIcons[currentPage] = 0;
        }
    } else {
        if (selectedIcons[currentPage] < 1) {
          selectedIcons[currentPage] = NUM_ICONS - 1;
        } else {
          selectedIcons[currentPage] -= 1;
        }
    }
    renderScreen();
  }
}

void handleNonMenuButton(int pin, bool &lastState, unsigned long &pressStart,
                  unsigned long &lastRepeat, unsigned long &currentInterval,
                  bool isPositive, bool state, unsigned long now) {
  if (lastState == LOW && state == HIGH) {
      // Button pressed
      pressStart = now;
      lastRepeat = now;
      currentInterval = initialRepeat;
    }

    if (pin != 3 && lastState == HIGH && state == LOW) {
      // Button released
      if (currentPage == NUM_PAGES - 1) {
        // On settings page → short press activates setting
        settingsAction(isPositive); // left = contrast, right = power
      } else {
        if (now - pressStart < holdThreshold) {
          // Short press → ±1
          mutateNumber(isPositive, 1);
        }
      }
    }

    // Long press handling
    if (pin != 3 && currentPage < NUM_PAGES - 1 && state == HIGH) {
      if (now - pressStart >= holdThreshold) {
        if (now - lastRepeat >= currentInterval) {
          mutateNumber(isPositive, 10);
          lastRepeat = now;

          // Ramp speed
          if (currentInterval > minInterval + stepDecrease) {
            currentInterval -= stepDecrease;
          } else {
            currentInterval = minInterval;
          }
        }
      }
    }
}

void handleButton(int pin, bool &lastState, unsigned long &pressStart,
                  unsigned long &lastRepeat, unsigned long &currentInterval,
                  bool isPositive) {

  bool state = digitalRead(pin);
  unsigned long now = millis();

  // Todo: This invocation has a check that is also internally done, please clean this up (pin 3 check)
  // Handle entering/exiting selection mode
  if (pin == 3) {
    handleSelectionMode(pin, state, lastState);
  }

  // If button 3 is held and button 1 or 2 pressed → enter selection mode
  if (button3Held) {
    if (pin == 1 && lastState == LOW && state == HIGH) {
      button3OtherPressedDuringHold = true;
      selectionMode = true;
      selectedItem = 0; // left
      renderScreen();
      return;
    }
    if (pin == 2 && lastState == LOW && state == HIGH) {
      button3OtherPressedDuringHold = true;
      selectionMode = true;
      selectedItem = 1; // right
      renderScreen();
      return;
    }
  }

  // If in selection mode, up/down modifies either icon or number
  if (selectionMode) {
    if (pin == 1) {
      // Up
      if (selectedItem == 0) {
        if (currentPage == NUM_PAGES - 2) {
          // +x/+y counter page
          handleXYMutation(true, true, lastState, state);
        } else {
          // cycle icon
          cycleIcons(true, lastState, state);
        }
      } else {
        if (currentPage == NUM_PAGES - 2) {
          // +x/+y counter page
          handleXYMutation(false, true, lastState, state);
        } else {
          // Handle number mutation
          handleNonMenuButton(pin, lastState, pressStart, lastRepeat, currentInterval, isPositive, state, now);
        }
      }
    } else if (pin == 2) {
      // Down
      if (selectedItem == 0) {
        if (currentPage == NUM_PAGES - 2) {
          // +x/+y counter page
          handleXYMutation(true, false, lastState, state);
        } else {
          // cycle icon backwards
          cycleIcons(false, lastState, state);
        }
      } else {
        if (currentPage == NUM_PAGES - 2) {
          // +x/+y counter page
          handleXYMutation(false, false, lastState, state);
        }
        // Handle number mutation
        handleNonMenuButton(pin, lastState, pressStart, lastRepeat, currentInterval, isPositive, state, now);
      }
    }
    lastState = state;
    return; // don’t fall through to normal handling
  } else {
    handleNonMenuButton(pin, lastState, pressStart, lastRepeat, currentInterval, isPositive, state, now);
  }

  lastState = state;

  // Todo: Pretty sure we can always just call renderScreen() here and we can remove it everywhere else? If not, we should refactor so it's called after handleButton is ready.
}

void loop() {
  handleButton(BUTTON_PIN3, lastButtonState3, pressStart, lastRepeatTime, currentInterval, true);
  handleButton(BUTTON_PIN1, lastButtonState1, pressStart, lastRepeatTime, currentInterval, true);
  handleButton(BUTTON_PIN2, lastButtonState2, pressStart, lastRepeatTime, currentInterval, false);

    // Handle delta timeout
  if (deltaVisible && millis() - lastActionTime > changeDisplayDuration) {
    netChange = 0;
    deltaVisible = false;
    renderScreen();
  }

  delay(10); // debounce delay
}

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

// Utility function for drawing UI feedback
void drawSelectionUnderline(int16_t x, int16_t y, int16_t w) {
  display.drawLine(x, y + ICON_HEIGHT + 2, x + w, y + ICON_HEIGHT + 2, SH110X_WHITE);
}

void renderScreen() {
  display.clearDisplay();

  if (currentPage < NUM_PAGES - 1) {
    // Big number
    display.setTextSize(5);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 0);
    // Set up variables for getting text bounds
    int16_t x1, y1;
    uint16_t w, h;
    if (currentPage == NUM_PAGES - 2) {
      display.setTextSize(3);
      display.print(formatAsCounter(countersX[currentPage], countersY[currentPage]));
      // Underline logic
      if (selectionMode) {
        if (selectedItem == 0) {
          // Underline X
          String leftPart = "+" + String(abs(countersX[currentPage]));
          display.getTextBounds(leftPart, 0, 0, &x1, &y1, &w, &h);
          int underlineY = h + 3; // 3px below baseline
          display.drawLine(x1, underlineY, x1 + w, underlineY, SH110X_WHITE);
        } else if (selectedItem == 1) {
          // Underline Y
          String leftPartOffset = "+" + String(abs(countersX[currentPage])) + "|";
          display.getTextBounds(leftPartOffset, 0, 0, &x1, &y1, &w, &h);
          int offsetX = w; // x position where +y starts
          String rightPart = "+" + String(abs(countersY[currentPage]));
          display.getTextBounds(rightPart, offsetX, 0, &x1, &y1, &w, &h);
          int underlineY = h + 3; // 3px below baseline
          display.drawLine(x1, underlineY, x1 + w, underlineY, SH110X_WHITE);
        }   
      }
      display.setTextSize(5);
    } else {
      String textToDraw = String(counters[currentPage]);
      display.getTextBounds(textToDraw, 0, 0, &x1, &y1, &w, &h);
      // Draw icon on left
      int iconX = 0;
      int iconY = (h / 2) - (ICON_HEIGHT / 2) - 3; // Arbitrary 3 pixel offset (why???)
      display.drawBitmap(iconX, iconY,
        icons[selectedIcons[currentPage]].bitmap,
        icons[selectedIcons[currentPage]].width,
        icons[selectedIcons[currentPage]].height,
        SH110X_WHITE);

      // Draw number on the right
      int numberX = 30;
      display.setCursor(numberX, 0);  // leave space for icon
      display.print(counters[currentPage]);

      // Underline logic
      if (selectionMode) {
        int underlineY = h + 3; // 3px below text baseline
        if (selectedItem == 0) {
          // Underline icon
          display.drawLine(iconX, underlineY,
                           iconX + ICON_HEIGHT, underlineY,
                           SH110X_WHITE);
        } else if (selectedItem == 1) {
          // Underline number
          display.drawLine(numberX, underlineY,
                           numberX + w, underlineY,
                           SH110X_WHITE);
        }   
      }
    }

    // Delta (if active)
    if (deltaVisible && netChange != 0) {
      display.setTextSize(2);
      display.setCursor(0, 50);
      if (netChange > 0) {
        display.print("+");
      }
      display.print(netChange);
    }
  } 
  else {
    // Settings page
    // Left: contrast icon
    display.drawRect(20, 10, 40, 40, SH110X_WHITE);
    display.setCursor(28, 25);
    display.setTextSize(2);
    display.print("C");

    // Right: power icon
    display.drawRect(80, 10, 40, 40, SH110X_WHITE);
    display.setCursor(92, 25);
    display.setTextSize(2);
    display.print("P");
  }

  // Page indicator
  int boxSize = 6;
  int spacing = 10;
  int initialCursor = 120;
  for (int i = 0; i < NUM_PAGES; i++) {
    int x = initialCursor - i * spacing;
    if (i == currentPage) {
      display.fillRect(x, 55, boxSize, boxSize, SH110X_WHITE);
    } else {
      display.drawRect(x, 55, boxSize, boxSize, SH110X_WHITE);
    }
  }

  display.display();
}
