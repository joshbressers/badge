
#include "constants.h"

void moveDot() {
  uint8_t x = 4 * 32;
  uint8_t y = 4 * 32;
  int8_t x_vel = 0;
  int8_t y_vel = 0;

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

void pongGame() {
  float ball_x = 4;
  float ball_y = 2;
  unsigned int score = 0;
  float ball_x_vel = 0.01;
  float ball_y_vel = 0.01;

  float paddle_x = 0;

  if (!ball_x_vel) ball_x_vel = 0.05;
  if (!ball_y_vel) ball_y_vel = 0.05;

  while(true) {
    LOOP(0);
    clearFrameBuffer();
  
    if (PUSH_BUTTON(BTN_RIGHT) && paddle_x < 5) {
      paddle_x = paddle_x + 0.05;
    }
    if (PUSH_BUTTON(BTN_LEFT) && paddle_x > 0) {
      paddle_x = paddle_x - 0.05;
    }

    if (NEW_BUTTON(BTN_B)) {
      return;
    }

    ball_x = ball_x + ball_x_vel;
    if ((ball_x >= 7) || (ball_x < 1)) ball_x_vel = ball_x_vel * -1;
    ball_y = ball_y + ball_y_vel;
    if ((ball_y >= 7) || (ball_y < 1)) ball_y_vel = ball_y_vel * -1;

    // We're at the paddle, did we hit it?
    if (ball_y >= 7) {
      if ((ball_x >= paddle_x) && (ball_x <= paddle_x + 3 )) {
        score++;
      } else {
        printScore(score);
        return;
      }
    }

    // Draw the ball
    frameBuffer[(int)ball_x] = 1 << (int)ball_y;

    // Draw the paddle
    for (int i = 0; i < 3; i++) {
      frameBuffer[(int)paddle_x + i] = frameBuffer[(int)paddle_x + i] | 0x80;
    }
  }
}
