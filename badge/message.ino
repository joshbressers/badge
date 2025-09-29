// Function that displays a message on the screen

#include "constants.h"

bool progmemMessage = true;

// Turn the score into a string
// Using sprintf takes up 2K of program space
void printScore(unsigned int score) {
  uint8_t i = 0;
  unsigned int digit;
  unsigned char scoreString[10];

  printMessage(scoreMessage, true);

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

  printMessage(scoreString, false);
}

// Print a message then return
void printMessage(unsigned char *newMessage, bool progmemMessage) {

  setMessage(newMessage, progmemMessage);
  while (true) {
    LOOP(0);
    showMessage();
    if (donePrinting) return;
  }
}

// Set the message to a string in PROGMEM
// A string cannot be passed in here
void setMessage(unsigned char *newMessage, bool progMem) {

  progmemMessage = progMem;
  message = newMessage;
  messageCount = 0;
  donePrinting = false;
  byte messageByte;

  clearFrameBuffer();
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

    for (int i = 0; i < 7; i++) {
      frameBuffer[i] = frameBuffer[i+1];
    }
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
    if (((messageCount) / 6) >= messageLen) {
      // This the case where we hit the end of the message
      // and have to wrap back to 0
      if (progmemMessage) {
        letterPos = pgm_read_word(message) - 0x20;
      } else{ 
        letterPos = message[0] - 0x20;
      }
      // We subtrace 0x20 from the letter so our font index lines up
    } else {
      uint8_t pgmIndex = (messageCount) / 6;
      if (progmemMessage) {
        letterPos = pgm_read_word(message + pgmIndex) - 0x20;
      } else {
        letterPos = message[pgmIndex] - 0x20;
      }
    }

    // pos contians the actual row we are going to display
    uint8_t pos = (messageCount) % 6;
    if (pos == 5) {
      // Insert a gap between letters
      frameBuffer[7] = 0x00;
    } else {
      byte letterChar = pgm_read_byte(font + (letterPos * 5) + pos);
      frameBuffer[7] = letterChar;
    }


    // messageCount is the row we're displaying, when we get to the end of the
    // string, we flip back to zero
    messageCount++;
    if (messageCount >= messageLen * 6) {
      messageCount = 0;
      donePrinting = true;
    }
  }

}

// Write all zeros to the framebuffer
// Basically, clear the screen
void clearFrameBuffer() {
  memset(frameBuffer, 0, 8);
  messageCount = 0;
}

// Set one point on the framebuffer
void setFrameBuffer(uint8_t x, uint8_t y) {
  if (x < 8 && y < 8)
    frameBuffer[x] |= 1 << y;
}
