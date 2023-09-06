void buttonTest() {

/*
 * The button layout is
 * 
 *    U
 *  L   R    B A
 *    D
 *    
 *    Bit
 *    0x01 - unused
 *    0x02 - unused
 *    0x04 - A
 *    0x08 - B
 *    0x10 - Right
 *    0x20 - Down
 *    0x40 - Left
 *    0x80 - Up
 */

    // Clear the frame buffer
    clearFrameBuffer();
    
    if (inputBits & BTN_A) frameBuffer[6] = 0x08; // A
    if (inputBits & BTN_B) frameBuffer[4] = 0x08; // B
    if (inputBits & BTN_RIGHT) frameBuffer[2] = 0x08; // Right
    if (inputBits & BTN_DOWN) frameBuffer[1] = frameBuffer[1] | 0x10; // Down
    if (inputBits & BTN_UP) frameBuffer[1] = frameBuffer[1] | 0x04; // Up
    if (inputBits & BTN_LEFT) frameBuffer[0] = 0x08; // Left

}
