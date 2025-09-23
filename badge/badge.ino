
#include "constants.h"


void setup() {
//set pins to output so you can control the shift register

// We will talk straight to the input/output register
// By not using the arduino digitalWrite functionality
// We save 500 bytes and it's substantially faster
DDRB |= (1 << latchPin);			// output
DDRB |= (1 << clockPin);
DDRB |= (1 << dataPin1);
DDRB |= (1 << dataPin2);
DDRB &= ~(1 << buttonPin); // input

setMessage(defaultMessage);
}

void loop() {
  runTick();
  showMessage();

  // If we see a new keypress, load the menu
  // The bitwise XOR is here because we don't want a
  // held down button to triger the menu, which happens when you
  // select back from the menu (or anything else that could have an exit)
  if (CUR_BUTTON && (CUR_BUTTON ^ OLD_BUTTON)) {
    if (rngState == 0) {
      rngState = currentTick;
      if (rngState == 0) rngState = 7;
    }
    showMenu();
    setMessage(defaultMessage);
  }
}
