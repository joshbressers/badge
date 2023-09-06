
#include "font.h"


// Constants

//Pin connected to ST_CP of 74HC595
int latchPin = 1;

//Pin connected to SH_CP of 74HC595
uint8_t clockPin = 2;

////Pin connected to DS of 74HC595
uint8_t dataPin1 = 0; // SR 1
uint8_t dataPin2 = 3; // SR 2

uint8_t buttonPin = 4; // Button Input
uint8_t inputBits = 0x00;

// Frame buffer variables
byte currentRow = 0;
unsigned long messageCount = 0;
uint8_t messageDelay = 50;
byte frameBuffer[] = {0,0,0,0,0,0,0,0};

enum State {
  HOME,
  MENU,
  MENU2, // Private state for menu
  BUTTON
};
State currentState = HOME;

static const unsigned char defaultMessage[] PROGMEM = "Teh Badge ";
unsigned char *message;
unsigned int messageLen = 0;

unsigned long currentLoop = 0;
unsigned long lastLoop = 0;
unsigned long lastButton = 0;

// Button Constants
#define BTN_A 0x04
#define BTN_B 0x08
#define BTN_LEFT 0x40
#define BTN_RIGHT 0x10
#define BTN_UP 0x80
#define BTN_DOWN 0x20

uint8_t OLD_BUTTON = 0;

void setup() {
//set pins to output so you can control the shift register
pinMode(latchPin, OUTPUT);
pinMode(clockPin, OUTPUT);
pinMode(dataPin1, OUTPUT);
pinMode(dataPin2, OUTPUT);
pinMode(buttonPin, INPUT);
setMessage(defaultMessage);
}

void loop() {

  // We use the number of milliseconds to know when to take various actions
  currentLoop++;
  shiftRegisters();

  switch(currentState) {
    case HOME:
      showMessage();
      // If we see a new keypress, load the menu
      if (inputBits && (inputBits ^ OLD_BUTTON)) currentState = MENU;
      break;
    case MENU:
    case MENU2:
      showMenu();
      break;
    case BUTTON:
      buttonTest();
      break;
  }

  // If we detect any input at all, reset the lastButton counter
  if (inputBits) {
    lastButton = 0;
  } else {
    lastButton++;
  }

  // If nothing has happened in a while, go back home
  if (lastButton > 1000) {
    if (currentState != HOME) {
      currentState = HOME;
      setDefaultMessage();
      lastButton = 0;
    }
  }
}
