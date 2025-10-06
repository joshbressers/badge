
#include "constants.h"

uint8_t code_count = 0;

const char secretCode[] = {
  BTN_UP,
  BTN_UP,
  BTN_DOWN,
  BTN_DOWN,
  BTN_LEFT,
  BTN_RIGHT,
  BTN_LEFT,
  BTN_RIGHT,
  BTN_B,
  BTN_A
};

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

setMessage(defaultMessage, true);
}

void loop() {
  runTick();
  showMessage();

  // If we see a new keypress, load the menu
  // The bitwise XOR is here because we don't want a
  // held down button to triger the menu, which happens when you
  // select back from the menu (or anything else that could have an exit)
#ifdef SecretCode
  if (CUR_BUTTON && (CUR_BUTTON ^ OLD_BUTTON)) {
    if (secretCode[code_count] == CUR_BUTTON) {
      code_count++;
      if (code_count == sizeof(secretCode)) {
        code_count = 0;
        printMessage(codez, true);
        setMessage(defaultMessage, true);
      }
    } else {
      code_count = 0;
      showMenu(mainMenu, mainMenuLen, HOME_TIMEOUT);
      setMessage(defaultMessage, true);
    }
  }
#endif
#ifndef SecretCode
  if (CUR_BUTTON && (CUR_BUTTON ^ OLD_BUTTON)) {
      showMenu(mainMenu, mainMenuLen, HOME_TIMEOUT);
      setMessage(defaultMessage, true);
  }
#endif
}
