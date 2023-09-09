#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "font.h"

// Constants

// Hardware related constants

// Shift register pins
#define latchPin 1
#define clockPin 2

////Pin connected to DS of 74HC595
#define dataPin1 0 // SR 1
#define dataPin2 3 // SR 2
#define buttonPin 4 // Button Input

// Random variables shared by things
uint8_t i, j;

// Frame buffer variables
byte currentRow = 0;
byte frameBuffer[] = {0,0,0,0,0,0,0,0};

// Global state tracking
enum State {
  HOME,
  MENU,
  MENU2, // Private state for menu
  DOT,
  SCREEN,
  BUTTON
};
State currentState = HOME;

// How many ticks should we wait until we return to home screen
#define HOME_TIMEOUT 2000

// Message display constants
const char defaultMessage[] PROGMEM = "Badge Test ";
const char* message;
unsigned int messageLen = 0;

// Constants used when writing the message to the framebuffer
unsigned long messageCount = 0;
#define messageDelay 30
unsigned long lastButton = 0;

// Tick constants
// The millis() on this chip isn't very reliable, so let's just count up a tick
// on every loop
unsigned long currentTick = 0;

// TICK can be used for delay loops. Pass it in a value so you only execute
// after a certain amount of time has passed.
#define TICK(the_tick) (!(currentTick % the_tick))

// This has to be run by every function. A timeout of zero can be passed in
// to not every timeout. If no timeout is used it's up to the function
// to return
#define LOOP(loop_define_timeout) {\
  runTick();\
  if (loop_define_timeout && (lastButton > loop_define_timeout)) {\
    return;\
  }\
}

// Button Constants
#define BTN_A 0x04
#define BTN_B 0x08
#define BTN_LEFT 0x40
#define BTN_RIGHT 0x10
#define BTN_UP 0x80
#define BTN_DOWN 0x20

uint8_t CUR_BUTTON = 0x00;
uint8_t OLD_BUTTON = 0x00;

// Button functionality

// This will return true if a button is pressed for the first time
// A held down button will return false
#define NEW_BUTTON(the_btn) ((CUR_BUTTON & the_btn) && !(OLD_BUTTON & the_btn))

// Define our functions here
void setMessage(unsigned char *newMessage);
void shiftRegisters();
void showMessage();
void clearFrameBuffer();
void moveDot();
void showMenu();
void screenTest();
void buttonTest();
void runTick();

#endif
