/*
 * menu
 * 
 * Display the main menu. When a key is pressed on the
 * default message screen, the menu is loaded. We then
 * launch other subroutines from here, or go back to the
 * message screen
 */

// Menu static text
static const unsigned char menu0[] PROGMEM = "Menu ";
static const unsigned char menu1[] PROGMEM = "Button Test ";
static const unsigned char menu2[] PROGMEM = "Back ";

uint8_t menuState = 0;
bool menuChanged = false;

void showMenu() {

  // The first time we show the menu, change the text
  if (currentState == MENU) {
    currentState = MENU2;
    setMessage(menu0);
  }

  // Button handling
  // Change the message if up adn down are pressed
//  if ((CUR_BUTTON & BTN_DOWN) && !(OLD_BUTTON & BTN_DOWN)) {
  if (NEW_BUTTON(BTN_DOWN)) {

    if (menuState < 2) {
      menuState++;
      menuChanged = true;
    }
//  } else if ((CUR_BUTTON & BTN_UP) && !(OLD_BUTTON & BTN_UP)) {
    } else if (NEW_BUTTON(BTN_UP)) {
    if (menuState > 0) {
      menuState--;
      menuChanged = true;
    }
//  } else if ((CUR_BUTTON & BTN_A) && !(OLD_BUTTON & BTN_A)) {
  } else if (NEW_BUTTON(BTN_A)) {
    if (menuState == 1) currentState = BUTTON;
    if (menuState == 2) {
      currentState = HOME;
      OLD_BUTTON = CUR_BUTTON;
      setMessage(defaultMessage);
    }
    menuState = 0;
    menuChanged = false;
  }
  if (menuChanged) {
    menuChanged = false;
    switch(menuState) {
      case 0:
        setMessage(menu0);
        break;
      case 1:
        setMessage(menu1);
        break;
      case 2:
        setMessage(menu2);
        break;
    }
  }
  showMessage();
}
