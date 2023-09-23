
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
TIMSK = _BV(TOIE1); // Interrupt on compare match with OCR1A
  
// Start timer in CTC mode; prescaler = 4096; 
TCCR1 = _BV(CS13) | _BV(CS10);
sei();

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
