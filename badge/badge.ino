
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
  runTick();
  showMessage();

  // If we see a new keypress, load the menu
  // The bitwise XOR is here because we don't want a
  // held down button to triger the menu, which happens when you
  // select back from the menu (or anything else that could have an exit)
  if (CUR_BUTTON && (CUR_BUTTON ^ OLD_BUTTON)) {
    // Seed the RNG on the first button press
    if (!randomSet) {
      randomSeed(currentTick);
      randomSet = true;
    }
    showMenu();
    setMessage(defaultMessage);
  }
}
