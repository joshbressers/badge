

// constants won't change. Used here to set a pin number:
const int SR_SERIAL = 0;  // pin 14
const int SR_LATCH = 1; // pin 12
const int SR_CLK = 2; // pin 11


//**************************************************************//
//  Name    : shiftOutCode, Hello World
//  Author  : Carlyn Maw,Tom Igoe, David A. Mellis
//  Date    : 25 Oct, 2006
//  Modified: 23 Mar 2010
//  Version : 2.0
//  Notes   : Code for using a 74HC595 Shift Register           //
//          : to count from 0 to 255
//****************************************************************
//Pin connected to ST_CP of 74HC595
int latchPin = 1;
//Pin connected to SH_CP of 74HC595
int clockPin = 2;
////Pin connected to DS of 74HC595
int dataPin1 = 0;
int dataPin2 = 3;
byte currentRow = 0;
uint8_t messageCount = 60;
uint8_t messageDelay = 5000;

uint8_t frameBuffer[] = {0,0,0,0,0,0,0,0};

uint8_t message[] = {
        0x36, 0x49, 0x49, 0x49, 0x36, 0x00,  // 0x38 8
        0x46, 0x49, 0x49, 0x29, 0x1E, 0x00,  // 0x39 9
        0x00, 0x00, 0x14, 0x00, 0x00, 0x00,  // 0x3A :
        0x00, 0x40, 0x34, 0x00, 0x00, 0x00,  // 0x3B ;
        0x00, 0x08, 0x14, 0x22, 0x41, 0x00,  // 0x3C <
        0x14, 0x14, 0x14, 0x14, 0x14, 0x00,  // 0x3D =
        0x00, 0x41, 0x22, 0x14, 0x08, 0x00,  // 0x3E >
        0x02, 0x01, 0x59, 0x09, 0x06, 0x00,  // 0x3F ?
        0x3E, 0x41, 0x5D, 0x59, 0x4E, 0x00,  // 0x40 @
        0x7C, 0x12, 0x11, 0x12, 0x7C, 0x00,  // 0x41 A

};

unsigned long previousMillis = 0;

void shiftRegisters()
{
      uint8_t i, j;
      uint8_t currentRow = 1;

      for (j = 0; j < 8; j++) {
        digitalWrite(latchPin, LOW);
        for (i = 0; i < 8; i++)  { 
          digitalWrite(dataPin1, !!(currentRow & (1 << (i))));
          
          digitalWrite(dataPin2, !(frameBuffer[j] & (1 << (7 - i))));
          
          digitalWrite(clockPin, HIGH);
          digitalWrite(clockPin, LOW);
        }
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

  unsigned long currentMillis = millis(); 
  
  // Check delay loop
  if (currentMillis - previousMillis >= messageDelay) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // Copy message data into framebuffer
    for (int i = 0; i < 8; i++) {
      frameBuffer[i] = message[(messageCount + i) % 60]; 
    }
    messageCount++;
    if (messageCount >= 60) messageCount = 0;
  }
  
shiftRegisters();
//}
}
