
#include "constants.h"

void buttonTest() {
  while(true) {
    LOOP(HOME_TIMEOUT);

    // Clear the frame buffer
    clearFrameBuffer();

    // Print dots on the display to represent the buttons
    if (CUR_BUTTON & BTN_A) frameBuffer[6] = 0x08; // A
    if (CUR_BUTTON & BTN_B) frameBuffer[4] = 0x08; // B
    if (CUR_BUTTON & BTN_RIGHT) frameBuffer[2] = 0x08; // Right
    if (CUR_BUTTON & BTN_DOWN) frameBuffer[1] = frameBuffer[1] | 0x10; // Down
    if (CUR_BUTTON & BTN_UP) frameBuffer[1] = frameBuffer[1] | 0x04; // Up
    if (CUR_BUTTON & BTN_LEFT) frameBuffer[0] = 0x08; // Left
  }
}

void screenTest() {
  uint8_t pattern = 0x00;

  while (true) {
    LOOP(0);

    if (TICK(100)) {
      if (!pattern) pattern = 0xFF;
      else pattern = pattern << 1;
    }
    if (TICK(10)) {
      for (int i = 0; i < 8; i++) {
        frameBuffer[i] = pattern;
      }
    }
    if (NEW_BUTTON(BTN_A)) {
      return;
    }
  }
}
