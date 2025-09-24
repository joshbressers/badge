/*
 * menu
 * 
 * Display the main menu. When a key is pressed on the
 * default message screen, the menu is loaded. We then
 * launch other subroutines from here, or go back to the
 * message screen
 */

#include "constants.h"

void showMenu(badgeStruct *thePrograms, uint8_t menuMax, uint16_t menuTimeout) {
  uint8_t menuIndex = 0;
  bool menuChanged = false;
  
  setMessage(thePrograms[0].programName);
  while (true) {
    LOOP(menuTimeout);

    
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
