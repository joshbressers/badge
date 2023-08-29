
#include "font.h"


// Constants

//Pin connected to ST_CP of 74HC595
int latchPin = 1;

//Pin connected to SH_CP of 74HC595
uint8_t clockPin = 2;

////Pin connected to DS of 74HC595
uint8_t dataPin1 = 0;
uint8_t dataPin2 = 3;

// Frame buffer variables
byte currentRow = 0;
uint8_t messageCount = 60;
uint8_t messageDelay = 5000;
byte frameBuffer[] = {0,0,0,0,0,0,0,0};

unsigned char message[] = "Test Message2 \0";
uint8_t messageLen = strlen(message);

unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

void setup() {
//set pins to output so you can control the shift register
pinMode(latchPin, OUTPUT);
pinMode(clockPin, OUTPUT);
pinMode(dataPin1, OUTPUT);
pinMode(dataPin2, OUTPUT);
}

void loop() {

  // We use the number of milliseconds to know when to take various actions
  currentMillis = millis(); 
  
  showMessage();  
  shiftRegisters();
}
