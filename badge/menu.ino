/*
 * menu
 * 
 * Display the main menu. When a key is pressed on the
 * default message screen, the menu is loaded. We then
 * launch other subroutines from here, or go back to the
 * message screen
 */

#include "constants.h"

// Menu static text
static const unsigned char menu0[] PROGMEM = "Menu ";
static const unsigned char menu1[] PROGMEM = "Button Test ";
static const unsigned char menu2[] PROGMEM = "Move Dot ";
static const unsigned char menu99[] PROGMEM = "Back ";

// The compiler will probably make these incrementing integers
// But let's set them just in case
// M_MENU should always be first and M_BACK always last
enum menuStates {
  M_MENU = 0,
  M_BUTTON = 1,
  M_DOT = 2,
  M_BACK = 3
  
} menuState;
bool menuChanged = false;

void showMenu() {

  // The first time we show the menu, change the text
  if (currentState == MENU) {
    currentState = MENU2;
    menuState = M_MENU;
    setMessage(menu0);
  }

  // Button handling
  // Change the message if up adn down are pressed
//  if ((CUR_BUTTON & BTN_DOWN) && !(OLD_BUTTON & BTN_DOWN)) {
  if (NEW_BUTTON(BTN_DOWN)) {

    if (menuState < M_BACK) {
      menuState = menuState + 1;
      menuChanged = true;
    }
  } else if (NEW_BUTTON(BTN_UP)) {
    if (menuState > M_MENU) {
      menuState = menuState - 1;
      menuChanged = true;
    }
  } else if (NEW_BUTTON(BTN_A)) {
    if (menuState == M_BUTTON) currentState = BUTTON;
    else if (menuState == M_DOT) currentState = DOT;
    else if (menuState == M_BACK) {
      currentState = HOME;
      OLD_BUTTON = CUR_BUTTON;
      setMessage(defaultMessage);
    }
    menuState = M_MENU;
    menuChanged = false;
  }
  if (menuChanged) {
    menuChanged = false;
    switch(menuState) {
      case M_MENU:
        setMessage(menu0);
        break;
      case M_BUTTON:
        setMessage(menu1);
        break;
      case M_DOT:
        setMessage(menu2);
        break;
      case M_BACK:
        setMessage(menu99);
        break;
    }
  }
  showMessage();
}
