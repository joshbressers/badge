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
const char menu0[] PROGMEM = "Menu ";
const char menu1[] PROGMEM = "Button Test ";
const char menu2[] PROGMEM = "Move Dot ";
const char menu3[] PROGMEM = "Screen Test ";
const char menu99[] PROGMEM = "Back ";

const char* const menuStrings[] PROGMEM = {
  menu0,
  menu1,
  menu2,
  menu3,
  menu99
};

// The compiler will probably make these incrementing integers
// But let's set them just in case
// M_MENU should always be first and M_BACK always last
enum menuStates {
  M_MENU = 0,
  M_BUTTON = 1,
  M_DOT = 2,
  M_SCREEN = 3,
  M_BACK = 4
 
} menuState;
bool menuChanged = false;

void showMenu() {

  // The first time we show the menu, change the text
  if (currentState == MENU) {
    currentState = MENU2;
    menuState = M_MENU;
    setMessage(&menuStrings[menuState]);
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
    else if (menuState == M_SCREEN) currentState = SCREEN;
    else if (menuState == M_BACK) {
      currentState = HOME;
      OLD_BUTTON = CUR_BUTTON;
      setMessage(&homeStrings[0]);
    }
    menuState = M_MENU;
    menuChanged = false;
  }
  if (menuChanged) {
    menuChanged = false;
    setMessage(&menuStrings[menuState]);
  }
  showMessage();
}
