//Init
/*
ALL STATIC VARIABLES
*/
#define SCREEN_WIDTH 64  // OLED display width, in pixels
#define SCREEN_HEIGHT 128 // OLED display height, in pixels
#define SCREEN_RESET -1     // can set an oled reset pin if desired
#define SCREEN_ROTATION 3 //Set the screens orientation

#define SCREEN_SDA 20 //Pin for screen SDA line
#define SCREEN_SCL 21 //Pin for screen SCL line
#define SCREEN_ADR 0x3C //Hex adress of the screen

#define BOUDRATE 115200 //USB communication baudrate



/*
INCLUSION OF ALL LIBRARIES NEEDED IN THIS FILE
*/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

/*
INCLUSION OF ALL FILES OF THIS PROJECT
*/

#include <icons.h>
#include <inputs.h>
#include <renderer.h>


/*
SETUP FOR ALL GENERATED VARIABLES
*/
Adafruit_SH1107 display = Adafruit_SH1107(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET, 1000000, 100000);


/*
CONSTRUCTION OF ALL VARIABLES USED AND CHANGED DURING RUNTIME
*/
bool isAwake    =   true;


//Initialization of the hardware
void setup() {
    //Set up hardware connections
    Serial.begin(BAUDRATE);

    Wire.setSDA(SCREEN_SDA);
    Wire.setSCL(SCREEN_SCL);

    setupPins();

    //Let the device settle and screen power up
    delay(250);

    //Start the display
    LittleFS.begin();
    disply.begin(SCREEN_ADR, true);
    display.display();

    delay(100);
    display.setRotation(SCREEN_ROTATION);
    display.clearDisplay();

    //Draw a pixel to skip the displays start screen
    display.drawPixel(10, 10, SH110X_WHITE);
    display.display();
    delay(10);
    display.clearDisplay();

    //Log setup being done
    Serial.println("MTG Counter setup finished");
}

void wake() {
    /*TODO: RETRIEVE DATA*/
}

void hibernate() {
    /*TODO: SAVE DATA*/

    isAwake = false;
    /*TODO: GO INTO SLEEP MODE*/
}


//Main loop
void loop() {
    if (!isAwake) {
        wake();
        isAwake = true;
    }

    //Sleeping

    
}