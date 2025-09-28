// Function to process the shift registers

#include "constants.h"


// The RNG state
uint32_t rngState = 0;

/* The state must be initialized to non-zero */
uint32_t random32()
{

  // The state can't be zero ever, we set it the first time
  if (rngState == 0) {
    rngState = currentTick;
    if (rngState == 0) rngState = 7;
  }
  
  /* Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs" */
  uint32_t x = rngState;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  return rngState = x;
}


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
      PORTB |= (1 << latchPin); // HIGH
      uint8_t currentRow = 1; // A bit vector for the LED row
      OLD_BUTTON = CUR_BUTTON;

      // Loop for each row
      for (int j = 0; j < 8; j++) {
        uint8_t currentCol = 0x01;
        PORTB &= ~(1 << latchPin); // LOW
        // Loop for each col, writing/reading one bit per clock
        // Set the button to 0, we will fill in the bits as we go
        CUR_BUTTON = 0;
        for (int i = 0; i < 8; i++)  {

          // In this code we have to extract the relevant bits to shift into 
          // the register
          if (!!(currentRow & currentCol)) {
            PORTB |= (1 << dataPin1); // HIGH
          } else {
            PORTB &= ~(1 << dataPin1); // LOW
          }

          // We write the data in backwards to scroll left
          // (remove the 7 - to scroll right, but that would be weird)
          if (!(frameBuffer[j] & (1 << (7 - i)))) {
            PORTB |= (1 << dataPin2); // HIGH
          } else {
            PORTB &= ~(1 << dataPin2); // LOW
          }

          // Shift in the button presses
          CUR_BUTTON = CUR_BUTTON << 1;
          // READ the button input
          if (PINB & (1 << buttonPin))
            CUR_BUTTON = CUR_BUTTON | 0x01;

          // Pulse the clock
          PORTB |= (1 << clockPin); // HIGH
          PORTB &= ~(1 << clockPin); // LOW
          currentCol = currentCol << 1;
        }
        // Shift the row bit
        currentRow = currentRow << 1;
        PORTB |= (1 << latchPin); // HIGH
        PORTB &= ~(1 << latchPin); // LOW
      }  
}
