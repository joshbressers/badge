// Function that displays a message on the screen

#include "constants.h"

// Set the message to a string in PROGMEM
// A string cannot be passed in here
void setMessage(unsigned char *newMessage) {
  message = newMessage;
  messageCount = 0;

  // We have to write our own strlen as PROGMEM strings are different
  messageLen = 0;
  while (true) {
    if (pgm_read_byte(message + messageLen) == 0) {
      break;
    } else {
      messageLen++;
    }
  }
}


void showMessage()
{
  // Scroll the display at a reasonable speed
  if (TICK(messageDelay)) {

    /*
     * Copy message data into framebuffer
     * 
     * We have two loops, the row then the column. We have to write the
     * framebuffer data one row at a time.
     * The way we are using the shift registers and LED matrix, rows on the matix are
     * columns to us humans
     * 
     * I use the term "row" often, a row in our context is one column on the matrix
     * This was done becuase it makes scrolling letters much much easier
     * 
     */
    
    for (i = 0; i < 8; i++) {
      /*
       * There will always be 2 letters on the screen at a time. The letters
       * are 6 pixels wide, the screen is 8
       * 
       * Don't think of the screen in terms of the letter we're showing
       * thikn of it in terms of the column we are showing in the total message
       */

      // letterPos will track the current letter we are displaying
      uint8_t letterPos;

      // This block figures out where the curent letter we're to display is
      // messageCount is the current row we are to display for the message
      if (((messageCount + i) / 6) >= messageLen) {
        // This the case where we hit the end of the message
        // and have to wrap back to 0
        letterPos = pgm_read_word(message) - 0x20;
        // We subtrace 0x20 from the letter so our font index lines up
      } else {
        uint8_t pgmIndex = (messageCount + i) / 6;
        letterPos = pgm_read_word(message + pgmIndex) - 0x20;
      }

      // pos contians the actual row we are going to display
      uint8_t pos = (messageCount + i) % 6;
      byte letterChar = pgm_read_byte(font + (letterPos * 6) + pos);
      frameBuffer[i] = letterChar;
    }

    // messageCount is the row we're displaying, when we get to the end of the
    // string, we flip back to zero
    messageCount++;
    if (messageCount >= messageLen * 6) messageCount = 0;
  }

}

// Write all zeros to the framebuffer
// Basically, clear the screen
void clearFrameBuffer() {
  for (i = 0; i < 8; i++) {
    frameBuffer[i] = 0;
  }
  // Reset the message to the beginning
  messageCount = 0;
}
