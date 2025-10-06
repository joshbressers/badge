#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <avr/io.h>
#include "font.h"

// Things you might want to change are all located here

// Games to enable
//#define DotGame
//#define PongGame
//#define SpaceGame
//#define RainGame
//#define BlockGame
//#define CustomMessage
//#define EnableTests

// Comment this out if you don't want a secret code
//#define SecretCode

// This is the default message that scrolls across the badge by default
const char defaultMessage[] PROGMEM = "Default Badge Message ";
const char codez[] PROGMEM = "sekrit codez!!!  ";

// The speed at which the message scrolls. Bigger numbers scroll slower
#define messageDelay 30

// How many ticks should we wait until we return to home screen
// Bigger numbers wait longer
#define HOME_TIMEOUT 3000


// Below here are things you probably shouldn't change


// Constants

// Hardware related constants

// Shift register pins
#define latchPin PB1
#define clockPin PB2

////Pin connected to DS of 74HC595
#define dataPin1 PB0 // SR 1
#define dataPin2 PB3 // SR 2
#define buttonPin PB4 // Button Input

// Frame buffer variables
byte frameBuffer[] = {0,0,0,0,0,0,0,0};

// Message display constants
const char* message;
unsigned int messageLen = 0;

const char scoreMessage[] PROGMEM = "Score ";


// Constants used when writing the message to the framebuffer
unsigned int messageCount = 0;
unsigned int lastButton = 0;

// Tick constants
// The millis() on this chip isn't very reliable, so let's just count up a tick
// on every loop
unsigned int currentTick = 0;

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

// The Arduino random() code uses up 500 bytes, we'll use our own
#define RANDOM(rand_ceiling) (random32() % rand_ceiling)

// Button Constants
#define BTN_A 0x10
#define BTN_B 0x20
#define BTN_LEFT 0x04
#define BTN_RIGHT 0x02
#define BTN_UP 0x08
#define BTN_DOWN 0x01

uint8_t CUR_BUTTON = 0x00;
uint8_t OLD_BUTTON = 0x00;

// Button functionality

// This will return true if a button is pressed for the first time
// A held down button will return false
#define NEW_BUTTON(the_btn) ((CUR_BUTTON & the_btn) && !(OLD_BUTTON & the_btn))

// Return true if the button is pressed
#define PUSH_BUTTON(the_btn) (CUR_BUTTON & the_btn)

// Define our functions here
void setMessage(unsigned char *newMessage, bool progMem);
void printMessage(unsigned char *newMessage, bool progmemMessage);
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
void showRain();
void customMessage();
uint32_t random32();
void teh_code();
void badgeBlock();
void getString(unsigned char *theString);



// Main Menu things

// Menu static text
const char menu0[] PROGMEM = "Menu ";
const char menu1[] PROGMEM = "Test Buttons ";
const char menu2[] PROGMEM = "Dot ";
const char menu3[] PROGMEM = "Test Screen ";
const char menu4[] PROGMEM = "Pong ";
const char menu5[] PROGMEM = "Space ";
const char menu6[] PROGMEM = "Rain ";
const char menu7[] PROGMEM = "Custom Message ";
const char menu8[] PROGMEM = "Badgetris ";

typedef struct badgeStruct {
  const char *programName;
  void (*badgeFunction)(void);
};

badgeStruct mainMenu[] = {
  {
    menu0,
    NULL
  },
#ifdef CustomMessage
  {
    menu7,
    (*customMessage)
  },
#endif
#ifdef BlockGame
  {
    menu8,
    (*badgeBlock)
  },
#endif
#ifdef RainGame
  {
    menu6,
    (*showRain)
  },
#endif
#ifdef DotGame
  {
    menu2,
    (*moveDot)
  },
#endif
#ifdef PongGame
  {
    menu4,
    (*pongGame)
  },
#endif
#ifdef SpaceGame
  {
    menu5,
    (*spaceGame)
  },
#endif
#ifdef EnableTests
  {
    menu1,
    (*buttonTest)
  },

  {
    menu3,
    (*screenTest)
  }
#endif

};

uint8_t mainMenuLen = sizeof(mainMenu) / sizeof(mainMenu[0]);
void showMenu(badgeStruct *thePrograms, uint8_t menuMax, uint16_t menuTimeout);

#endif
