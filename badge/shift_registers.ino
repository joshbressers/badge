// Function to process the shift registers
void shiftRegisters()
{
      digitalWrite(latchPin, HIGH);
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

          // We handle each button press like this. It's sort of silly
          // but because we're already doing unspeakable things with these
          // shift registers, it's the only way to make it all work
          // (I tried to bitshift it all in)
          if (digitalRead(buttonPin)) {
            switch(i) {
              case 0:
                inputBits = inputBits | 0x01;
                break;
              case 1:
                inputBits = inputBits | 0x02;
                break;
              case 2:
                inputBits = inputBits | 0x04;
                break;
              case 3:
                inputBits = inputBits | 0x08;
                break;
              case 4:
                // Unused at the moment
                inputBits = inputBits | 0x10;
                break;
              case 5:
                inputBits = inputBits | 0x20;
                break;
              case 6:
                inputBits = inputBits | 0x40;
                break;
              case 7:
                // This one will never work, the bit is lost in the void
                inputBits = inputBits | 0x80;
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
        //digitalWrite(clockPin, HIGH);
      }  
}
