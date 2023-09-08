
#include "constants.h"

float x = 4;
float y = 4;
float x_vel = 0;
float y_vel = 0;

void moveDot() {
  clearFrameBuffer();
  
  if (NEW_BUTTON(BTN_RIGHT)) x_vel += 0.1;
  if (NEW_BUTTON(BTN_DOWN)) y_vel += 0.1;
  if (NEW_BUTTON(BTN_UP)) y_vel -= 0.1;
  if (NEW_BUTTON(BTN_LEFT)) x_vel -= 0.1;

  if (NEW_BUTTON(BTN_A) || NEW_BUTTON(BTN_B)) {
    x = 4;
    y = 4;
    x_vel = 0;
    y_vel = 0;
  }

  x = x + x_vel;
  y = y + y_vel;

  if (x > 7) x = 0;
  if (y > 7) y = 0;
  if (x < 0) x = 7;
  if (y < 0) y = 7;

  frameBuffer[(int)x] = 1 << (int)y;

}
