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
#define INTERRUPTBUTTONPIN 4

#define PRESSBUFFER 32

volatile bool asleep = false;

uint8_t button_presses[PRESSBUFFER];
uint8_t current_presses = 0;

void setupPins() {
  pinMode(LEFTBUTTONPIN,    INPUT_PULLDOWN);
  pinMode(MIDDLEBUTTONPIN,  INPUT_PULLDOWN); 
  pinMode(RIGHTBUTTONPIN,   INPUT_PULLDOWN); 
  pinMode(INTERRUPTBUTTONPIN, INPUT_PULLDOWN);

  attachInterrupt(digitalPinToInterrupt(INTERRUPTBUTTONPIN),     interrupt,      RISING);
  
  button_presses[PRESSBUFFER - 1] = 0; //Setup the ring buffers location tracker
}

void interrupt() {
    if asleep {
        asleep = false;
        //Wake function

        for (int i = 0; i < (PRESSBUFFER - 1); i++) { //Reset the buffer on wake
            button_presses[i] = 0;
        }
    }
}

void updatePinStates() {
    button_presses[PRESSBUFFER - 1] = button_presses[PRESSBUFFER - 1] + 1; //Increment ring buffer
    if button_presses[PRESSBUFFER - 1] == PRESSBUFFER - 1 {
        button_presses[PRESSBUFFER - 1] = 0; //Reset ring buffer counter
    }
    
    button_presses[button_presses[PRESSBUFFER - 1]] = 0; //Reset currently targetted rinbuffer
    //Store the button presses with offset
    button_presses[button_presses[PRESSBUFFER - 1]] = button_presses[button_presses[PRESSBUFFER - 1]] + (digitalRead(LEFTBUTTONPIN)   << 0);
    button_presses[button_presses[PRESSBUFFER - 1]] = button_presses[button_presses[PRESSBUFFER - 1]] + (digitalRead(MIDDLEBUTTONPIN) << 1);
    button_presses[button_presses[PRESSBUFFER - 1]] = button_presses[button_presses[PRESSBUFFER - 1]] + (digitalRead(RIGHTBUTTONPIN)  << 2);
    
}

uint8_t lastPress() {
    return button_presses[button_presses[PRESSBUFFER - 1]];
}