// Function to process the shift registers
void shiftRegisters()
{
      uint8_t i, j;
      uint8_t currentRow = 1; // A bit vector for the LED row

      // Loop for each row
      for (j = 0; j < 8; j++) {
        digitalWrite(latchPin, LOW);
        // Loop for each col, writing one bit per clock
        inputBits = 0;
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
          
          //// Read the button input
          if (digitalRead(dataPin3)) {
            // We only have to write the 1's
            inputBits = inputBits | (1 << i);
          }
        }
        // Shift the row bit
        currentRow = currentRow << 1;
        digitalWrite(latchPin, HIGH);
        digitalWrite(latchPin, LOW);
        //digitalWrite(clockPin, HIGH);
      }  
}
