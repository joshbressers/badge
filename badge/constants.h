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

// Frame buffer variables
byte currentRow = 0;
byte frameBuffer[] = {0,0,0,0,0,0,0,0};

// How many ticks should we wait until we return to home screen
#define HOME_TIMEOUT 2000

// Message display constants
const char defaultMessage[] PROGMEM = "Default Badge Message ";
const char* message;
unsigned int messageLen = 0;

const char scoreMessage[] PROGMEM = "Score ";
unsigned char scoreString[10];


// Constants used when writing the message to the framebuffer
unsigned long messageCount = 0;
#define messageDelay 30
unsigned long lastButton = 0;

// Tick constants
// The millis() on this chip isn't very reliable, so let's just count up a tick
// on every loop
unsigned long currentTick = 0;

// State booleans
bool donePrinting = false;

// TICK can be used for delay loops. Pass it in a value to return true
// after a certain amount of time has passed.
#define TICK(the_tick) (!(currentTick % the_tick))

// This has to be run by every function. A timeout of zero can be passed in
// to not ever timeout. If no timeout is used it's up to the function
// to return
#define LOOP(loop_define_timeout) {\
  runTick();\
  if (loop_define_timeout && (lastButton > loop_define_timeout)) {\
    return;\
  }\
}

// This is random enough for us. The Arduino random() code
// uses up 500 bytes
#define RANDOM(rand_ceiling) (currentTick % rand_ceiling)

// Button Constants
#define BTN_A 0x20
#define BTN_B 0x10
#define BTN_LEFT 0x02
#define BTN_RIGHT 0x08
#define BTN_UP 0x01
#define BTN_DOWN 0x04

uint8_t CUR_BUTTON = 0x00;
uint8_t OLD_BUTTON = 0x00;

// Button functionality

// This will return true if a button is pressed for the first time
// A held down button will return false
#define NEW_BUTTON(the_btn) ((CUR_BUTTON & the_btn) && !(OLD_BUTTON & the_btn))

// Return true if the button is pressed
#define PUSH_BUTTON(the_btn) (CUR_BUTTON & the_btn)

// Define our functions here
void setMessage(unsigned char *newMessage);
void setMemMessage(unsigned char *newMessage);
void printMessage(unsigned char *newMessage, bool memMessage);
void printScore(unsigned int score);
void shiftRegisters();
void showMessage();
void clearFrameBuffer();
void moveDot();
void showMenu();
void screenTest();
void buttonTest();
void runTick();
void pongGame();
void spaceGame();

#endif
