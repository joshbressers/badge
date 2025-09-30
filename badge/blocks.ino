
#include "constants.h"

void badgeBlock() {
  // Let's use a game board instead of just the framebuffer
  byte screen[8]= {0x00};
  unsigned int score = 0;
  bool down = false;

  // All the blocks are a 4x4 array turned into an integer
  // The bits represent the blocks
  short blocks[7] = {
    0x0660, // square
    0x0F00, // line
    0x4620, // S
    0x0264, // Z
    0x0E80, // L
    0x08E0, // backwards L
    0x2620 // T
  };

  // Random first block, but we always use the same position
  short block = blocks[RANDOM(7)];
  int8_t block_x = 3;
  int8_t block_y = -4;
  byte line;

  // This is used to determine if a block needs to be set down

  while(true) {
    LOOP(0);
    clearFrameBuffer(); // We don't technically need this

    // Only move the piece every now and then, or if the down button is pressed
    if (TICK(150) || down) {
      down = false;
      // Check if the piece is hitting anything
      for (int i = 0; i < 4; i++) {
        // Get the 4 bytes for the col we care about
        line = (block >> ((3-i) * 4)) & 0x0F;
        if (block_y < 0) {
          // Negative bitshifts aren't a thing
          line = line >> block_y * -1;
        } else {
          line = line << block_y;
        }
        if (line & 0x80 || (line << 1) & screen[block_x+i]) {
          // Do we have a game over?
          if (block_y == -2) {
            printScore(score);
            return;
          }
          // Basically the same loop as above, but this time, write the piece into the board
          for (int i = 0; i < 4; i++) {
            byte line = (block >> ((3-i) * 4)) & 0x0F;
            if (block_y < 0) {
              line = line >> block_y * -1;
            } else {
              line = line << block_y;
            }
            screen[block_x+i] = screen[block_x+i] | line;
          }
          // Get the next piece
          block_x = 3;
          block_y = -4;
          block = blocks[RANDOM(7)];
          goto out; // Only bad people use goto
        }
      }
      block_y++;
      // Don't increment y if we have to place
    }
out:
    if (NEW_BUTTON(BTN_A)) {
      short new_block = 0x0000;
      // Rotate
      // 0123456789ABCDEF
      // 37BF26AE159D048C

      // I will never remember how this works. Good luck
      for (int i = 3; i >= 0; i--) {
        new_block = (new_block << 1) | ((block & (0x0001 << i)) ? 0x0001 : 0x0000);
        new_block = (new_block << 1) | ((block & (0x0001 << (i + 4))) ? 0x0001 : 0x0000);
        new_block = (new_block << 1) | ((block & (0x0001 << (i + 8))) ? 0x0001 : 0x0000);
        new_block = (new_block << 1) | ((block & (0x0001 << (i + 12))) ? 0x0001 : 0x0000);
      }
      block = new_block;
    } else if (NEW_BUTTON(BTN_B)) {
      short new_block = 0x0000;
      // Rotate
      // 0123456789ABCDEF
      // C840D951EA62FB73

      // This is just the opposite of the above
      for (int i = 0; i < 4; i++) {
        new_block = (new_block << 1) | ((block & (0x0001 << (i + 12))) ? 0x0001 : 0x0000);
        new_block = (new_block << 1) | ((block & (0x0001 << (i + 8))) ? 0x0001 : 0x0000);
        new_block = (new_block << 1) | ((block & (0x0001 << (i + 4))) ? 0x0001 : 0x0000);
        new_block = (new_block << 1) | ((block & (0x0001 << i)) ? 0x0001 : 0x0000);
      }
      block = new_block;
    }

    if (NEW_BUTTON(BTN_LEFT)) {
      block_x--;
    } else if (NEW_BUTTON(BTN_RIGHT)) {
      block_x++;
    }

    // Remove this after testing is done
    // Or not, whatever
    if (NEW_BUTTON(BTN_DOWN)) {
      //block_y++;
      down = true;
    } //else if (NEW_BUTTON(BTN_UP)) {
      //block_y--;
    //}

    // Print the screen
    for (int i = 0; i < 8; i++) {
      frameBuffer[i] = screen[i];
    }

    // Print the block onto the screen, but not the board
    // It's a 4x4 array
    for (int i = 0; i < 4; i++) {
      line = (block >> ((3-i) * 4)) & 0x0F;
      // Now shift the piece into the place it belongs
      if (block_y < 0) {
        // Negative bitshifts aren't a thing
        line = line >> block_y * -1;
      } else {
        line = line << block_y;
      }
      // Don't allow the pieces to move off the side of the screen
      if (line == 0) continue;
      
      if (block_x+i < 0) block_x++;
      else if (block_x+i > 7) block_x--;
      else if (NEW_BUTTON(BTN_LEFT)) {
        //If we push left, and aren't already at the leftmost column check the line to the left
        if (line & frameBuffer[block_x+i])
          block_x++;
      } else if (NEW_BUTTON(BTN_RIGHT)) {
        if (line & frameBuffer[block_x+i])
          block_x--;
      }

      // Write the piece into the frame buffer
      frameBuffer[block_x+i] = frameBuffer[block_x+i] | line;
    }


    // Do we have a copmlete line? delete it
    byte row_mask = 0x00;
    byte row_mask_neg = 0xFF;
    // We loop once per row
    for (int i = 0; i < 8; i++) {
      // We use the row mask and a negative row mask to do some quick bit operations later
      row_mask = (row_mask >> 1) | 0x80;
      row_mask_neg = row_mask_neg >> 1;
      byte cb = 0x80 >> i; // current bit

      // This is easier unrolled
      bool full = (screen[0] & cb) &&
                  (screen[1] & cb) &&
                  (screen[2] & cb) &&
                  (screen[3] & cb) &&
                  (screen[4] & cb) &&
                  (screen[5] & cb) &&
                  (screen[6] & cb) &&
                  (screen[7] & cb);
      if (full) {
        // We have a full line
        score++;
        for (int j = 0; j < 8; j++) {
          // save off the bottom lines
          byte to_save = screen[j] & row_mask << 1;
          
          // save the lines above our complete row
          byte to_shift = screen[j] & row_mask_neg;
          // Shift the lines above down one place

          to_shift = to_shift << 1;
          // Recombine the lines
          screen[j] = to_save | to_shift;
        }
      }
    }
  }
}
