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


#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };

// Defining pins for buttons
#define BUTTON_PIN1 1  // GP1
#define BUTTON_PIN2 2  // GP2
#define BUTTON_PIN3 3  // GP3

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
const int MAX_PAGES = 4;
// The value where we store the main number being displayed
int counters[MAX_PAGES - 1] = {0, 0, 0};
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
  for (int i = 0; i < MAX_PAGES - 1; i++) {
    arr.add(counters[i]);
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
  for (int val : arr) {
    if (i < MAX_PAGES - 1) counters[i] = val;
    i++;
  }
}

void setup()   {
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
  delay(40); // debounce delay
}

void switchPage(int direction) {
  currentPage += direction;
  if (currentPage < 0) currentPage = MAX_PAGES - 1;
  if (currentPage >= MAX_PAGES) currentPage = 0;

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

void handleButton(int pin, bool &lastState, unsigned long &pressStart,
                  unsigned long &lastRepeat, unsigned long &currentInterval,
                  bool isPositive) {

  bool state = digitalRead(pin);
  unsigned long now = millis();

  if (lastState == LOW && state == HIGH) {
    // Button pressed
    pressStart = now;
    lastRepeat = now;
    currentInterval = initialRepeat;
  }

  if (lastState == HIGH && state == LOW) {
    // Button released
    // Simply call switchPage if button 3 is pressed for now
    if (pin == 3) {
      switchPage(1);
      lastState = state;
      return;
    }
    if (currentPage == MAX_PAGES - 1) {
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
  if (pin != 3 && currentPage < MAX_PAGES - 1 && state == HIGH) {
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

  lastState = state;
}

void loop() {
  handleButton(BUTTON_PIN1, lastButtonState1, pressStart, lastRepeatTime, currentInterval, true);
  handleButton(BUTTON_PIN2, lastButtonState2, pressStart, lastRepeatTime, currentInterval, false);
  handleButton(BUTTON_PIN3, lastButtonState3, pressStart, lastRepeatTime, currentInterval, true);

    // Handle delta timeout
  if (deltaVisible && millis() - lastActionTime > changeDisplayDuration) {
    netChange = 0;
    deltaVisible = false;
    renderScreen();
  }

  delay(10); // debounce delay
}

void renderScreen() {
  display.clearDisplay();

  if (currentPage < MAX_PAGES - 1) {
    // Big number
    display.setTextSize(5);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(20, 0);
    display.print(counters[currentPage]);

    // Delta (if active)
    if (deltaVisible && netChange != 0) {
      display.setTextSize(2);
      if (counters[currentPage] < 10) {
        display.setCursor(50, 0);
      } else {
        display.setCursor(80, 0); // adjust Y position
      }
      if (netChange > 0) {
        display.print("+");
      }
      display.print(netChange);
    }
  } else {
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
  for (int i = 0; i < MAX_PAGES; i++) {
    int y = i * spacing;
    if (i == currentPage) {
      display.fillRect(0, y, boxSize, boxSize, SH110X_WHITE);
    } else {
      display.drawRect(0, y, boxSize, boxSize, SH110X_WHITE);
    }
  }

  display.display();
}
