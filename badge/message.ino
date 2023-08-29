// Function that displays a message on the screen
void showMessage()
{
    // Scroll the display at a reasonable speed
  if (currentMillis - previousMillis >= messageDelay) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // Copy message data into framebuffer
    for (int i = 0; i < 8; i++) {
      // We will probably be reading two letters for this
      uint8_t letterPos;
      if (((messageCount + i) / 6) >= messageLen) {
        letterPos = message[(i / 6)] - 0x20;
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
