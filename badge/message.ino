// Function that displays a message on the screen

#include "constants.h"

bool progmemMessage = true;

// Turn the score into a string
// Using sprintf takes up 2K of program space
void printScore(unsigned int score) {
  uint8_t i = 0;
  unsigned int digit;

  printMessage(scoreMessage, false);

  scoreString[i++] = ' ';

  // We have to extract the score digits
  // They are reversed
  do {
    digit = score % 10;
    score = score / 10;
    scoreString[i++] = digit + 0x30;
  } while (score);

  // We need to reverse the string to fix the digits
  for(int j = 1; j <= i/2; j++) {
    scoreString[0] = scoreString[j]; // We are using slot 0 as our temp variable
    scoreString[j] = scoreString[i-j];
    scoreString[i-j] = scoreString[0];
  }
  scoreString[0] = ' '; // Don't forget to put this back
  scoreString[i++] = ' ';
  scoreString[i] = 0x00; // NULL terminator

  printMessage(scoreString, true);
}

// Print a message then return
void printMessage(unsigned char *newMessage, bool memMessage) {
  if (memMessage) {
    setMemMessage(newMessage);
  } else {
    setMessage(newMessage);
  }
  while (true) {
    LOOP(0);
    showMessage();
    if (donePrinting) return;
  }
}

// These are PROGMEM strings
void setMessage(unsigned char *newMessage) {
  progmemMessage = true;
  realSetMessage(newMessage);
}

void setMemMessage(unsigned char *newMessage) {
  progmemMessage = false;
  realSetMessage(newMessage);
}

// Set the message to a string in PROGMEM
// A string cannot be passed in here
void realSetMessage(unsigned char *newMessage) {
  message = newMessage;
  messageCount = 0;
  donePrinting = false;
  byte messageByte;

  // We have to write our own strlen as PROGMEM strings are different
  messageLen = 0;
  while (true) {
    if (progmemMessage) messageByte = pgm_read_byte(message + messageLen);
    else messageByte = message[messageLen];
    if (messageByte == 0) {
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
    
    for (int i = 0; i < 8; i++) {
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
        if (progmemMessage) {
          letterPos = pgm_read_word(message) - 0x20;
        } else{ 
          letterPos = message[0] - 0x20;
        }
        donePrinting = true;
        // We subtrace 0x20 from the letter so our font index lines up
      } else {
        uint8_t pgmIndex = (messageCount + i) / 6;
        if (progmemMessage) {
          letterPos = pgm_read_word(message + pgmIndex) - 0x20;
        } else {
          letterPos = message[pgmIndex] - 0x20;
        }
      }

      // pos contians the actual row we are going to display
      uint8_t pos = (messageCount + i) % 6;
      byte letterChar = pgm_read_byte(font + (letterPos * 6) + pos);
      frameBuffer[i] = letterChar;
    }

    // messageCount is the row we're displaying, when we get to the end of the
    // string, we flip back to zero
    messageCount++;
    if (messageCount >= messageLen * 6) {
      messageCount = 0;
    }
  }

}

// Write all zeros to the framebuffer
// Basically, clear the screen
void clearFrameBuffer() {
  for (int i = 0; i < 8; i++) {
    frameBuffer[i] = 0;
  }
  // Reset the message to the beginning
  messageCount = 0;
}

void setFrameBuffer(uint8_t x, uint8_t y) {
  if (x < 8 && y < 8 && x >= 0 && y >= 0)
    frameBuffer[x] |= 1 << y;
}
