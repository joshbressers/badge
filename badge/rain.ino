
#include "constants.h"

void showRain() {
  uint8_t x = 4;
  uint8_t y = 3;
  uint8_t loopCount = 0;
  uint8_t rainSpeed = 50;

  while(true) {

    if (rainSpeed == 0) rainSpeed = 1;

    clearFrameBuffer();

    if (loopCount == 0)
      setFrameBuffer(x, y);
    else
      circleBres(x, y, loopCount);

    for (int i = 0; i < rainSpeed; i++) {
      LOOP(0);
      if (NEW_BUTTON(BTN_DOWN)) rainSpeed++;
      if (NEW_BUTTON(BTN_UP)) rainSpeed--;
      if (NEW_BUTTON(BTN_B)) return;
    }
    loopCount++;
    if (loopCount > 9) {
      loopCount = 0;
      x = RANDOM(8);
      y = RANDOM(8);
    }
  }
}

void drawCircle(int xc, int yc, int x, int y){
    setFrameBuffer(xc+x, yc+y);
    setFrameBuffer(xc-x, yc+y);
    setFrameBuffer(xc+x, yc-y);
    setFrameBuffer(xc-x, yc-y);
    setFrameBuffer(xc+y, yc+x);
    setFrameBuffer(xc-y, yc+x);
    setFrameBuffer(xc+y, yc-x);
    setFrameBuffer(xc-y, yc-x);
}

void circleBres(int xc, int yc, int r){
    int x = 0, y = r;
    int d = 3 - 2 * r;
    drawCircle(xc, yc, x, y);
    while (y >= x){
      
        // check for decision parameter
        // and correspondingly 
        // update d, y
        if (d > 0) {
            y--; 
            d = d + 4 * (x - y) + 10;
        }
        else
            d = d + 4 * x + 6;

        // Increment x after updating decision parameter
        x++;
        
        // Draw the circle using the new coordinates
        drawCircle(xc, yc, x, y);
    }
}
