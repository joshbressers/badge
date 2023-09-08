#ifndef CONSTANTS_H
#define CONSTANTS_H

// Constants

// Hardware related constants

// Shift register pins
uint8_t latchPin = 1;
uint8_t clockPin = 2;

////Pin connected to DS of 74HC595
uint8_t dataPin1 = 0; // SR 1
uint8_t dataPin2 = 3; // SR 2
uint8_t buttonPin = 4; // Button Input

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
  BUTTON
};
State currentState = HOME;

// Message display constants
static const unsigned char defaultMessage[] PROGMEM = "Teh Badge ";
unsigned char *message;
unsigned int messageLen = 0;

// Constants used when writing the message to the framebuffer
unsigned long messageCount = 0;
uint8_t messageDelay = 50;
unsigned long lastButton = 0;

// Tick constants
// The millis() on this chip isn't very reliable, so let's just count up a tick
// on every loop
unsigned long currentTick = 0;
#define TICK(the_tick) (!(currentTick % the_tick))

// Button Constants
#define BTN_A 0x04
#define BTN_B 0x08
#define BTN_LEFT 0x40
#define BTN_RIGHT 0x10
#define BTN_UP 0x80
#define BTN_DOWN 0x20

uint8_t CUR_BUTTON = 0x00;
uint8_t OLD_BUTTON = 0;

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

#endif
