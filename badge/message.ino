// Function that displays a message on the screen
void showMessage()
{
    // Scroll the display at a reasonable speed
  if (currentLoop - lastLoop >= messageDelay) {
    // save the last time you blinked the LED
    lastLoop = currentLoop;

    // Copy message data into framebuffer
    for (int i = 0; i < 8; i++) {
      // There will always be 2 letters on the screen at a time. The letters
      // are 6 pixels wide, the screen is 8
      uint8_t letterPos;
      if (((messageCount + i) / 6) >= messageLen) {
        // This the case where we hit the end of the message
        // and have to wrap back to 0
        letterPos = message[0] - 0x20;
      } else {
        letterPos = message[((messageCount + i) / 6)] - 0x20;
      }
      //if (letterPos >= messageLen) letterPos = 0;
      uint8_t pos = (messageCount + i) % 6;
      byte letterChar = pgm_read_byte(font + (letterPos * 6) + pos);
      frameBuffer[i] = letterChar;
    }
    messageCount++;
    if (messageCount >= messageLen * 6) messageCount = 0;
  }

}
