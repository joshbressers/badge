
#include "constants.h"

void buttonTest() {
  while(true) {
    LOOP(1000);

    // Clear the frame buffer
    clearFrameBuffer();

    // Print dots on the display to represent the buttons
    if (CUR_BUTTON & BTN_A) setFrameBuffer(6, 4); // A
    if (CUR_BUTTON & BTN_B) setFrameBuffer(4, 4); // B
    if (CUR_BUTTON & BTN_RIGHT) setFrameBuffer(2, 4); // Right
    if (CUR_BUTTON & BTN_DOWN) setFrameBuffer(1, 5); // Down
    if (CUR_BUTTON & BTN_UP) setFrameBuffer(1, 3); // Up
    if (CUR_BUTTON & BTN_LEFT) setFrameBuffer(0, 4); // Left
  }
}

void screenTest() {
  uint8_t pattern = 0x00;

  while (true) {
    LOOP(0);

    if (TICK(200)) {
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
