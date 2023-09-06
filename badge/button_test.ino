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

    showDefaultMessage();
    // Clear the frame buffer
    clearFrameBuffer();
    
    if (inputBits & 0x04) frameBuffer[6] = 0x08; // A
    if (inputBits & 0x08) frameBuffer[4] = 0x08; // B
    if (inputBits & 0x10) frameBuffer[2] = 0x08; // Right
    if (inputBits & 0x20) frameBuffer[1] = frameBuffer[1] | 0x10; // Down
    if (inputBits & 0x80) frameBuffer[1] = frameBuffer[1] | 0x04; // Up
    if (inputBits & 0x40) frameBuffer[0] = 0x08; // Left

}
