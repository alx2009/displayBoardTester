// Alx2009 button interface
#ifndef __ABUTTON_H
#define __ABUTTON_H

#define MAX_BUTTONS 4
#define BUTTON_PRESSED_STATE HIGH
#define BUTTON_IDLE_STATE    LOW

class aButtonGroup {
    public:           
        aButtonGroup();
        typedef void (*buttonCallBack)(uint8_t buttonPin, uint8_t buttonEvent);

    protected:
        uint8_t buttonPin[MAX_BUTTONS];
        buttonCallBack callBack[MAX_BUTTONS];
        uint8_t buttonState[MAX_BUTTONS];            // the current reading from the button pin  
        uint8_t lastButtonState[MAX_BUTTONS];        // the previous reading from the button pin 
        unsigned long lastDebounceTime[MAX_BUTTONS]; // the last time the button pin was toggled
        unsigned long startPressed[MAX_BUTTONS];
        unsigned long lastReleased[MAX_BUTTONS];
        
        void check(uint8_t i);
        static const unsigned long debounceDelay = 50;     // the debounce time; increase if the output flickers
        static const unsigned long longPressTime = 800L;   // long press event when pressed more than longPressTime ms
        static const unsigned long doubleClicktime = 500L; // double click event when pressed within doubleClicktime ms from a previous release

    public:  // Define Button events, switches and callbacks
       static const uint8_t eventClick       = 0x01;  // detected after relase
       static const uint8_t eventDoubleClick = 0x02;  // detected after relase
       static const uint8_t eventLongPress   = 0x03;  // detected after relase
       static const uint8_t eventPress       = 0x11;  // detected imediately when pressed 
       static const uint8_t eventRelease     = 0x12;  // detected imediately when released
       
       boolean add(uint8_t pin, buttonCallBack pinCallBack);
       
       void check(void);

       static void Serial_printEventName(uint8_t event);

};

#endif //__ABUTTON_H
