
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

unsigned char message[] = "Test Message \0";
uint8_t messageLen = strlen(message);

unsigned long previousMillis = 0;

// Function to process the shift registers
void shiftRegisters()
{
      uint8_t i, j;
      uint8_t currentRow = 1; // A bit vector for the LED row

      // Loop for each row
      for (j = 0; j < 8; j++) {
        digitalWrite(latchPin, LOW);
        // Loop for each col, writing one bit per clock
        for (i = 0; i < 8; i++)  {

          // In this code we have to extract the relevant bits to shift into 
          // the register
          digitalWrite(dataPin1, !!(currentRow & (1 << (i))));

          // We write the data in backwards to scroll left
          // (remove the 7 - to scroll right, but that would be weird)
          digitalWrite(dataPin2, !(frameBuffer[j] & (1 << (7 - i))));

          // Pulse the clock
          digitalWrite(clockPin, HIGH);
          digitalWrite(clockPin, LOW);
        }
        // Shift the row bit
        currentRow = currentRow << 1;
        digitalWrite(latchPin, HIGH);
      }
}

void setup() {
//set pins to output so you can control the shift register
pinMode(latchPin, OUTPUT);
pinMode(clockPin, OUTPUT);
pinMode(dataPin1, OUTPUT);
pinMode(dataPin2, OUTPUT);
}

void loop() {

  // We use the number of milliseconds to know when to take various actions
  unsigned long currentMillis = millis(); 
  
  // Scroll the display at a reasonable speed
  if (currentMillis - previousMillis >= messageDelay) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // Copy message data into framebuffer
    for (int i = 0; i < 8; i++) {
      // We will probably be reading two letters for this
      uint8_t letterPos;
      if (((messageCount + i) / 6) >= messageLen) {
        letterPos = message[(i / 6)] - 0x20;
      } else {
        letterPos = message[((messageCount + i) / 6)] - 0x20;
      }
      //if (letterPos >= messageLen) letterPos = 0;
      uint8_t pos = (messageCount + i) % 6;
      byte letterChar = pgm_read_byte(font + (letterPos * 6) + pos);
      frameBuffer[i] = letterChar;
    }
    messageCount++;
    if (messageCount >= messageLen * 6) messageCount = 0;
  }
  
  shiftRegisters();
}
