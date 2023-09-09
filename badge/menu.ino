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
const char menu4[] PROGMEM = "Pong ";

typedef struct badgeStruct {
  const char *programName;
  void (*badgeFunction)(void);
};

badgeStruct thePrograms[] = {
  {
    menu0,
    NULL
  },
  {
    menu2,
    (*moveDot)
  },
  {
    menu4,
    (*pongGame)
  },
  {
    menu1,
    (*buttonTest)
  },

  {
    menu3,
    (*screenTest)
  }
  
};

void showMenu() {
  uint8_t menuIndex = 0;
  uint8_t menuMax = sizeof(thePrograms) / sizeof(thePrograms[0]);
  bool menuChanged = false;
  
  setMessage(thePrograms[0].programName);
  while (true) {
    LOOP(HOME_TIMEOUT);

    
    if (NEW_BUTTON(BTN_DOWN)) {

      if (menuIndex < (menuMax - 1)) {
        menuIndex++;
        menuChanged = true;
      }
    } else if (NEW_BUTTON(BTN_UP)) {
      if (menuIndex > 0) {
        menuIndex--;
        menuChanged = true;
      }
    } else if (NEW_BUTTON(BTN_A)) {
      if (thePrograms[menuIndex].badgeFunction != NULL) {
        (*thePrograms[menuIndex].badgeFunction)();
        menuIndex = 0;
        menuChanged = true;
      }
    }
    if (menuChanged) {
      menuChanged = false;
      setMessage(thePrograms[menuIndex].programName);
    }

    showMessage();
  }
}
