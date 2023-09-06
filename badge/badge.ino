
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
uint8_t messageCount = 0;
uint8_t messageDelay = 50;
byte frameBuffer[] = {0,0,0,0,0,0,0,0};

uint8_t currentState = 0;

unsigned char message[] = "Some message 01234\0";
unsigned int messageLen = strlen(message);

unsigned long currentLoop = 0;
unsigned long lastLoop = 0;
unsigned long lastButton = 0;

void setup() {
//set pins to output so you can control the shift register
pinMode(latchPin, OUTPUT);
pinMode(clockPin, OUTPUT);
pinMode(dataPin1, OUTPUT);
pinMode(dataPin2, OUTPUT);
pinMode(buttonPin, INPUT);
  
}

void loop() {

  // We use the number of milliseconds to know when to take various actions
  currentLoop++;
  shiftRegisters();

  if (currentState == 0) {
    showMessage();

    // Add a special check for input here to set the state
    if (inputBits) {
      currentState = 1;
      lastButton = 0;
    }

  } else if (currentState == 1) {
    buttonTest();
  }

  if (inputBits) {
    lastButton = 0;
  } else {
    lastButton++;
  }

  if (lastButton > 2000) {
    currentState = 0;
    lastButton = 0;
  }

//  sprintf(message, "|%x|", inputBits);
//  messageLen = 4;
}
