#define BATTERYPIN A1
const float V_REF = 3.3;                        //Ref voltage
const float R_BITS = 10.0;                      //ADC Resolution
const float ADC_STEPS = (1 << int(R_BITS)) -1;  // Steps in resolution


float percentage = 100.0;
float voltage = 0.0;
float rawReading = 0.0;

void updateBattery() {
    rawReading = analogRead(BATTERYPIN);
    voltage = (rawReading / ADC_STEPS) * V_REF;
    updatePercentage();
}

void updatePercentage() {
    //Transform voltage to percentage and update

    percentage = percentage + 1.0; //Dummy function to show this is being called
    if (percentage > 100.0) {
        percentage = percentage - 100.0;
    }
}

//Public functions

int getPercentage() {
    updateBattery();
    return int(percentage);
}

float getVoltage() {
    updateBattery();
    return voltage;
}

float getRaw() {
    updateBattery();
    return rawReading;
}