
#include "constants.h"

uint8_t code_count = 0;

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
    if (code_count == 0 && CUR_BUTTON == BTN_UP) code_count++;
    else if (code_count == 1 && CUR_BUTTON == BTN_UP) code_count++;
    else if (code_count == 2 && CUR_BUTTON == BTN_DOWN) code_count++;
    else if (code_count == 3 && CUR_BUTTON == BTN_DOWN) code_count++;
    else if (code_count == 4 && CUR_BUTTON == BTN_LEFT) code_count++;
    else if (code_count == 5 && CUR_BUTTON == BTN_RIGHT) code_count++;
    else if (code_count == 6 && CUR_BUTTON == BTN_LEFT) code_count++;
    else if (code_count == 7 && CUR_BUTTON == BTN_RIGHT) code_count++;
    else if (code_count == 8 && CUR_BUTTON == BTN_B) code_count++;
    else if (code_count == 9 && CUR_BUTTON == BTN_A) {
      code_count = 0;
      teh_code();
      setMessage(defaultMessage);
    } else {
      code_count = 0;
      showMenu();
      setMessage(defaultMessage);
    }
  }
}
