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
 * As soon as we trip the latch, we start to write and read bits
 * 
 */
void shiftRegisters() {
      //digitalWrite(latchPin, HIGH);
      PORTB |= (1 << latchPin);
      uint8_t currentRow = 1; // A bit vector for the LED row
      OLD_BUTTON = CUR_BUTTON;

      // Loop for each row
      for (int j = 0; j < 8; j++) {
        //digitalWrite(latchPin, LOW);
        PORTB &= ~(1 << latchPin);
        // Loop for each col, writing/reading one bit per clock
        // Set the button to 0, we will fill in the bits as we go
        CUR_BUTTON = 0;
        for (int i = 0; i < 8; i++)  {

          // In this code we have to extract the relevant bits to shift into 
          // the register
          //digitalWrite(dataPin1, !!(currentRow & (1 << (i))));
          if (!!(currentRow & (1 << (i)))) {
            PORTB |= (1 << dataPin1);
          } else {
            PORTB &= ~(1 << dataPin1);
          }

          // We write the data in backwards to scroll left
          // (remove the 7 - to scroll right, but that would be weird)
          //digitalWrite(dataPin2, !(frameBuffer[j] & (1 << (7 - i))));
          if (!(frameBuffer[j] & (1 << (7 - i)))) {
            PORTB |= (1 << dataPin2);
          } else {
            PORTB &= ~(1 << dataPin2);
          }

          // Shift in the button presses
          CUR_BUTTON = CUR_BUTTON << 1;
          //if (digitalRead(buttonPin))
          if (PINB & (1 << buttonPin))
            CUR_BUTTON = CUR_BUTTON | 0x01;

          // Pulse the clock
          //digitalWrite(clockPin, HIGH);
          PORTB |= (1 << clockPin);
          //digitalWrite(clockPin, LOW);
          PORTB &= ~(1 << clockPin);
        }
        // Shift the row bit
        currentRow = currentRow << 1;
        //digitalWrite(latchPin, HIGH);
        PORTB |= (1 << latchPin);
        //digitalWrite(latchPin, LOW);
        PORTB &= ~(1 << latchPin);
      }  
}
