// Function to process the shift registers

#include "constants.h"

// This is the function run by the timer interrupt
ISR (TIMER1_OVF_vect) {

  currentTick++;
  shiftRegisters();
  tickDone = true;
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
      PORTB |= (1 << PB1);
      uint8_t currentRow = 1; // A bit vector for the LED row
      OLD_BUTTON = CUR_BUTTON;
      CUR_BUTTON = 0;

      // Loop for each row
      for (int j = 0; j < 9; j++) {
        //digitalWrite(latchPin, LOW);
        PORTB &= ~(1 << PB1);
        // Loop for each col, writing/reading one bit per clock

        // We have to clear the last column before we return
        // so just loop one extra time
        if (j == 8) currentRow = 0;
        
        for (int i = 0; i < 8; i++)  {

          // In this code we have to extract the relevant bits to shift into 
          // the register
          //digitalWrite(dataPin1, !!(currentRow & (1 << (i))));
          if (currentRow & (1 << (i))) {
            PORTB |= (1 << PB0);
          } else {
            PORTB &= ~(1 << PB0);
          }

          // Only run these if we're writing actual data. Loop 8 is to clear
          // The last column, this will save some cycles
          if (j < 8) {
            // We write the data in backwards to scroll left
            // (remove the 7 - to scroll right, but that would be weird)
            // Also, on is zero, so we negate what's in the frame buffer
            //digitalWrite(dataPin2, !(frameBuffer[j] & (1 << (7 - i))));
            if (!(frameBuffer[j] & (1 << (7 - i)))) {
              PORTB |= (1 << PB3);
            } else {
              PORTB &= ~(1 << PB3);
            }

            // Shift in the button presses
            CUR_BUTTON = CUR_BUTTON << 1;
            //if (digitalRead(buttonPin))
            if (PINB & (1 << PB4))
              CUR_BUTTON = CUR_BUTTON | 0x01;
          }

          // Pulse the clock
          //digitalWrite(clockPin, HIGH);
          PORTB |= (1 << PB2);
          //digitalWrite(clockPin, LOW);
          PORTB &= ~(1 << PB2);
          
        }
        // Shift the row bit
        currentRow = currentRow << 1;
        //digitalWrite(latchPin, HIGH);
        PORTB |= (1 << PB1);
        //digitalWrite(latchPin, LOW);
        PORTB &= ~(1 << PB1);        
      }

      if (CUR_BUTTON) {
        lastButton = 0;
      } else {
        lastButton++;
      }
}

bool buttonPressed(byte myButton, bool newPress) {
  if (newPress) {
    if ((CUR_BUTTON & myButton) && !(OLD_BUTTON & myButton)) {
      OLD_BUTTON = CUR_BUTTON;
      return true;
    } else {
      return false;
    }
  } else {
    return (CUR_BUTTON & myButton);
  }
}
