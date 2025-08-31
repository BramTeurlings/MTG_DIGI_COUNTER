//Responsible for rendering all things;

//New Renderer(SPI CONNECTION/WIRES)
uint8_t bigTextSize = 5;
uint8_t smallTextSize = 2;


//RenderPage(Page)


//RenderObject(object, x position, y position)

//RenderBattery(BatteryPercentage, x, y)

private: 
void clear() {
    display.clearDisplay();
}

void resetRenderer() {
    //idk jus treset all things i guess
}

void renderIcon(uint8_t Icon, uint8_t x, uint8_t y) {
    display.drawBitmap(
        x, y,
        icons[Icon].bitmap,
        icons[Icon].width,
        icons[Icon].height,
        SH110X_WHITE
    );
}

void renderCounter(uint8_t counterX, uint8_t counterY, uint8_t x, uint8_t y) {
    display.setTextSize(bigTextSize);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(x, y);

    String counter = "";

    // Add sign and value for x
    if (counterX >= 0) {
        counter += "+";
    } else {
        counter += "-";
    }
    
    counter += String(counterX);

    counter += "|";

    // Add sign and value for y
    if (counterY >= 0) {
            counter += "+";
    } else {
    counter += "-";
    }

    counter += String(counterY);

    display.print(counter);
}

void renderDelta(uint8_t delta, bool isPositive, uint8_t x, uint8_t y) {
    display.setTextSize(smallTextSize);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(x, y);

    if (isPositive) {
        display.print("+");
    } else {
        display.print("-");
    }

    display.print(delta);
}

void renderBattery(float batteryPercentage, uint8_t x, uint8_t y) {
    display.setTextColor(SH110X_WHITE);
    display.setCursor(x, y);


}