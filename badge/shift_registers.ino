// Function to process the shift registers

#include "constants.h"

void runTick() {
  
  // We count the number of loops instead of using millis()
  // On the ATTINY85 chip the millis() retuns odd things
  currentTick++;

  if (CUR_BUTTON) {
    lastButton = 0;
  } else {
    lastButton++;
  }

  shiftRegisters();
}


/*
 * The way we use shift registes is very odd, but given the extreme constraints
 * for this project, it works.
 * 
 * We drive the LED matrix with the two serial to parallel registers.
 * We read the buttons with the parallel to serial register.
 * 
 * All 3 shift registes share a common clock and latch
 * 
 * Because of this, it's impossible to read all 7 bits from the button register
 * (or it is and I can't figure it out)
 * 
 * As soon as we trip the latch, we start to write and read bits
 * 
 */
void shiftRegisters() {
      digitalWrite(latchPin, HIGH);
      uint8_t currentRow = 1; // A bit vector for the LED row
      OLD_BUTTON = CUR_BUTTON;

      // Loop for each row
      for (j = 0; j < 8; j++) {
        digitalWrite(latchPin, LOW);
        // Loop for each col, writing/reading one bit per clock
        // Set the button to 0, we will fill in the bits as we go
        CUR_BUTTON = 0;
        for (i = 0; i < 8; i++)  {

          // In this code we have to extract the relevant bits to shift into 
          // the register
          digitalWrite(dataPin1, !!(currentRow & (1 << (i))));

          // We write the data in backwards to scroll left
          // (remove the 7 - to scroll right, but that would be weird)
          digitalWrite(dataPin2, !(frameBuffer[j] & (1 << (7 - i))));

          // We handle each button press like this. It's sort of silly
          // but because we're already doing unspeakable things with these
          // shift registers, it's the only way to make it all work
          // (I tried to bitshift it all in, it just wasn't working)
          if (digitalRead(buttonPin)) {
            switch(i) {
              case 0:
                CUR_BUTTON = CUR_BUTTON | 0x01;
                break;
              case 1:
                CUR_BUTTON = CUR_BUTTON | 0x02;
                break;
              case 2:
                CUR_BUTTON = CUR_BUTTON | 0x04;
                break;
              case 3:
                CUR_BUTTON = CUR_BUTTON | 0x08;
                break;
              case 4:
                // Unused at the moment
                CUR_BUTTON = CUR_BUTTON | 0x10;
                break;
              case 5:
                CUR_BUTTON = CUR_BUTTON | 0x20;
                break;
              case 6:
                CUR_BUTTON = CUR_BUTTON | 0x40;
                break;
              case 7:
                // This one will never work, the bit is lost in the void
                CUR_BUTTON = CUR_BUTTON | 0x80;
                break;
             }
          }

          // Pulse the clock
          digitalWrite(clockPin, HIGH);
          digitalWrite(clockPin, LOW);
          
        }
        // Shift the row bit
        currentRow = currentRow << 1;
        digitalWrite(latchPin, HIGH);
        digitalWrite(latchPin, LOW);
      }  
}
