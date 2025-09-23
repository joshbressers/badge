
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

    //frameBuffer[x/32] = 1 << (y/32);
    setFrameBuffer(x/32, y/32);
  }
}

void pongGame() {
  float ball_x = 3;
  float ball_y = 2;
  unsigned int score = 0;
  float ball_x_vel = 0.012;
  float ball_y_vel = 0.01;

  float paddle_x = 0;

  if (RANDOM(2)) ball_x_vel = -0.012;
  if (RANDOM(2)) ball_y_vel = -0.01;

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
        if (!RANDOM(3)) ball_x_vel = ball_x_vel * -1;

      } else {
        printScore(score);
        return;
      }
    }

    // Draw the ball
    setFrameBuffer((int)ball_x, (int)ball_y);

    // Draw the paddle
    for (int i = 0; i < 3; i++) {
      setFrameBuffer((int)paddle_x+i, 7);
    }
  }
}

void spaceGame() {
  unsigned int score = 0;

  // The ship will be offset by one
  // If we try to use -1 as the leftmost range
  // We end up with the problme of >-1 to <1 is zero,
  // Which is too wide of a range. Think of zero on
  // ship as the middle, not the side
  float ship_x = 4;
  float bullet_x = 0;
  float bullet_y = 0;

  float alien_x = 0;
  float alien_y = -1;

  while(true) {
    LOOP(0);
    clearFrameBuffer();

    if (alien_y < 0) {
      // No alien, let's add one
      // Also, why are aliens always the bad guys?
      // Very Ender's Game

      alien_y = 0;
      alien_x = RANDOM(7);
      
    } else if (alien_y >= 7) {
      printScore(score);
      return;
    }else {
      setFrameBuffer((int)alien_x, (int)alien_y);
      setFrameBuffer((int)alien_x + 1, (int)alien_y);
      alien_y = alien_y + 0.005 + score * 0.001;
    }

    
    if (PUSH_BUTTON(BTN_RIGHT) && ship_x < 7) {
      ship_x = ship_x + 0.04;
    }
    if (PUSH_BUTTON(BTN_LEFT) && ship_x > 0) {
      ship_x = ship_x - 0.04;
    }
    if (NEW_BUTTON(BTN_A) && bullet_y <= 0) {
      bullet_x = ship_x;
      bullet_y = 6;
    }

    if (bullet_y > 0) {
      setFrameBuffer((int)bullet_x, (int)bullet_y);

      if ((int(bullet_y) == (int)alien_y) && ((int)bullet_x >= (int)alien_x && (int)bullet_x <= ((int)alien_x + 1))) {
        alien_y = -10;
        alien_x = -10;
        bullet_y = -10;
        score++;
      }
      
      bullet_y = bullet_y - 0.05;
    }

    // Draw the ship
    setFrameBuffer((int)ship_x - 1, 7);
    setFrameBuffer((int)ship_x, 6);
    setFrameBuffer((int)ship_x, 7);
    setFrameBuffer((int)ship_x + 1, 7);
  }
}
