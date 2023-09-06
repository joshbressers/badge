
#include "font.h"
#include "constants.h"


void setup() {
//set pins to output so you can control the shift register
pinMode(latchPin, OUTPUT);
pinMode(clockPin, OUTPUT);
pinMode(dataPin1, OUTPUT);
pinMode(dataPin2, OUTPUT);
pinMode(buttonPin, INPUT);
setMessage(defaultMessage);
}

void loop() {

  // We count the number of loops instead of using millis()
  // On the ATTINY85 chip the millis() retuns odd things
  currentLoop++;

  // Load up the shift registers
  shiftRegisters();

  // We have a global state tracker. The functions we call are expected to set
  // this to HOME to return back to the menu
  switch(currentState) {
    case HOME:
      showMessage();
      // If we see a new keypress, load the menu
      // The bitwise XOR is here because we don't want a
      // held down button to triger the menu, which happens when you
      // select back from the menu (or anything else that could have an exit)
      if (CUR_BUTTON && (CUR_BUTTON ^ OLD_BUTTON)) currentState = MENU;
      break;
    case MENU:
    case MENU2:
      showMenu();
      break;
    case BUTTON:
      buttonTest();
      break;
  }

  // If we detect any input at all, reset the lastButton counter
  if (CUR_BUTTON) {
    lastButton = 0;
  } else {
    lastButton++;
  }

  // If nothing has happened in a while, go back home
  // This is a timeout function that when a badge isn't being used, it starts to show
  // the default message
  if (lastButton > 1000) {
    if (currentState != HOME) {
      currentState = HOME;
      setMessage(defaultMessage);
      lastButton = 0;
    }
  }
}
