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
const char menu1[] PROGMEM = "Test Buttons ";
const char menu2[] PROGMEM = "Dot ";
const char menu3[] PROGMEM = "Test Screen ";
const char menu4[] PROGMEM = "Pong ";
const char menu5[] PROGMEM = "Space ";
const char menu6[] PROGMEM = "Rain";

typedef struct badgeStruct {
  const char *programName;
  void (*badgeFunction)(void);
};

badgeStruct thePrograms[] = {
  {
    menu0,
    NULL
  },
#ifdef RainGame
  {
    menu6,
    (*showRain)
  },
#endif
#ifdef DotGame
  {
    menu2,
    (*moveDot)
  },
#endif
#ifdef PongGame
  {
    menu4,
    (*pongGame)
  },
#endif
#ifdef SpaceGame
  {
    menu5,
    (*spaceGame)
  },
#endif
#ifdef EnableTests
  {
    menu1,
    (*buttonTest)
  },

  {
    menu3,
    (*screenTest)
  }
#endif
  
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
