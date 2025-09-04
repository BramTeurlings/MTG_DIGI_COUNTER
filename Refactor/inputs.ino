//All button inputs and mechanics are to take place and be handled here
//This should be a class instantiated with the to be attached functions not defined here

//The main things contained here;
//Buttons
//Interrupts
//Debouncing function
//Multi-button combos

//Defenitions
#define LEFTBUTTONPIN   1
#define MIDDLEBUTTONPIN 2
#define RIGHTBUTTONPIN  3

#define PRESSBUFFER 32

volatile uint8_t button_presses[PRESSBUFFER];


void attachInterrupts() {
  pinMode(LEFTBUTTONPIN,    INPUT_PULLDOWN);
  pinMode(MIDDLEBUTTONPIN,  INPUT_PULLDOWN); 
  pinMode(RIGHTBUTTONPIN,   INPUT_PULLDOWN); 

  attachInterrupt(digitalPinToInterrupt(LEFTBUTTONPIN),     leftInterrupt,      RISING);
  attachInterrupt(digitalPinToInterrupt(MIDDLEBUTTONPIN),   middleInterrupt,    RISING);
  attachInterrupt(digitalPinToInterrupt(RIGHTBUTTONPIN),    rightInterrupt,     RISING);
}

void leftInterrupt() {
    button_presses += 1;
}

void middleInterrupt() {
    button_presses += 2;
}

void rightInterrupt() {
    button_presses += 3;
}

void generalInterrupt() {
    button_presses += 0;
}