
#include "constants.h"

void moveDot() {
  uint8_t x = 4 * 32;
  uint8_t y = 4 * 32;
  uint8_t x_vel = 0;
  uint8_t y_vel = 0;

  while(true) {
    LOOP(0);
    clearFrameBuffer();
  
    if (NEW_BUTTON(BTN_RIGHT)) x_vel++;
    if (NEW_BUTTON(BTN_DOWN)) y_vel++;
    if (NEW_BUTTON(BTN_UP)) y_vel--;
    if (NEW_BUTTON(BTN_LEFT)) x_vel--;

    if (NEW_BUTTON(BTN_A)) {
      x = 4 * 32;
      y = 4 * 32;
      x_vel = 0;
      y_vel = 0;
    } else if (NEW_BUTTON(BTN_B)) {
      return;
    }

    x = x + x_vel;
    y = y + y_vel;

    frameBuffer[x/32] = 1 << (y/32);
  }
}
