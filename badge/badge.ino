
/*
#include<avr/io.h>
#include <avr/interrupt.h>
#include<util/delay.h>
#define F_CPU 8000000UL
*/

#include "constants.h"



void setup() {

// 60Hz interrupt setup
TCCR1 = 0; // Stop timer
TCNT1 = 0; // Zero timer
TIMSK = _BV(TOIE1); // Interrupt on overflow
  
// Start timer in CTC mode; prescaler = 512;
// This controls the framerate
// 512 is 60FPS
// Lower FPS means more time to do things between frames
TCCR1 = _BV(CS13) | _BV(CS11);

/*
 * CS13 CS12 CS11 CS10  Clock divisor
 *   0    1    1    1    64
 *   1    0    0    0    128
 *   1    0    0    1    256
 *   1    0    1    0    512
 *   1    0    1    1    1024
 *   1    1    0    0    2048
 *   1    1    0    1    4096
 *   1    1    1    0    8192
 *   1    1    1    1    16384
 */

sei(); // Enable interrupts

//set pins to output so you can control the shift register
pinMode(latchPin, OUTPUT);
pinMode(clockPin, OUTPUT);
pinMode(dataPin1, OUTPUT);
pinMode(dataPin2, OUTPUT);
pinMode(buttonPin, INPUT);
setMessage(defaultMessage);

}

void loop() {
  showMessage();

  // If we see a new keypress, load the menu
  // The bitwise XOR is here because we don't want a
  // held down button to triger the menu, which happens when you
  // select back from the menu (or anything else that could have an exit)
  if (CUR_BUTTON && (CUR_BUTTON ^ OLD_BUTTON)) {
    OLD_BUTTON = CUR_BUTTON; // Prevent the menu detecting a new button press
    showMenu();
    setMessage(defaultMessage);
  }

  WAIT;
}
