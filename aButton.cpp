// Alx2009 button interface
#include <Arduino.h>
#include "aButton.h"

aButtonGroup::aButtonGroup() {
    for (int i=0; i <MAX_BUTTONS; i++) {
        buttonPin[i] = 0xff;
        callBack[i] = NULL; 
        buttonState[i] = BUTTON_IDLE_STATE; 
        lastButtonState[i] = BUTTON_IDLE_STATE; 
        lastDebounceTime[i] = 0L;
        startPressed[i] = 0L;
        lastReleased[i] = 0L;
    }
}

boolean aButtonGroup::add(uint8_t pin, buttonCallBack pinCallBack) {
    if (pinCallBack == NULL) {
        return false;  
    }
    for (int i=0; i <MAX_BUTTONS; i++) {
        if ( callBack[i] == NULL ) { // we found a free slot...
            buttonPin[i] = pin;
            callBack[i] = pinCallBack; 
            return true;
        }
    }
    return false;
}

void aButtonGroup::check(uint8_t i) {  // Check Index i (note: we assume the caller function checks that i<MAX_BUTTONS, callBack[i] != NULL, etc.
      unsigned long now = millis();
      int btnow = digitalRead(buttonPin[i]);
      if (btnow != lastButtonState[i]) {
         // reset the debouncing timer
         lastDebounceTime[i] = now;
         //Serial.print(F("pin ")); Serial.print(buttonPin[i]); Serial.print(F(" now="));Serial.println(btnow);
      }

      if ((now - lastDebounceTime[i]) > debounceDelay) {
          // whatever the reading is at, it's been there for longer than the debounce
          // delay, so take it as the actual current state:

          // if the button state has changed:
          if (btnow != buttonState[i]) {
              buttonState[i] = btnow;
              // The following actions are taken at Button release
              if (btnow == BUTTON_IDLE_STATE) {     // button was just released
                  callBack[i](buttonPin[i], eventRelease);
                  if ( (now - startPressed[i]) > longPressTime ) {
                      callBack[i](buttonPin[i], eventLongPress);
                  } else if (startPressed[i] - lastReleased[i] < doubleClicktime) {
                      callBack[i](buttonPin[i], eventDoubleClick);
                  } else { 
                      callBack[i](buttonPin[i], eventClick);
                  }
                  lastReleased[i] = now;                  
              } else {//btnow == BUTTON_PRESSED_STATE   // button was just pressed
                  callBack[i](buttonPin[i], eventPress);
                  startPressed[i] = now;
              }
          }
      }
      lastButtonState[i] = btnow;  
}

void aButtonGroup::check(void) {
    for (int i=0; i <MAX_BUTTONS; i++) {
        if ( callBack[i] != NULL ) { // we found a valid slot
            check(i);  
        }
    }        
}

void aButtonGroup::Serial_printEventName(uint8_t event) {
    switch(event) {
       case eventClick:       Serial.print(F("eventClick"));       break;
       case eventDoubleClick: Serial.print(F("eventDoubleClick")); break;
       case eventLongPress:   Serial.print(F("eventLongPress"));   break;
       case eventPress:       Serial.print(F("eventPress"));   break;
       case eventRelease:     Serial.print(F("eventRelease"));   break;
       default: Serial.print(F("unknown ev. "));  Serial.print(event);  break;
    }
}
