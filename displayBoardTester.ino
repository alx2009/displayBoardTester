/*
 Test Keiyhley 197A display board with Arduino Nano

 (C) ALX2009
 Licensed under GPL-2.0 license, see file LICENSE

 Acknowledgments:
 This work wouldn't have been possible without eevblog user rastro reverse engineering work: https://www.eevblog.com/forum/projects/keithley-197-led-display-hack/msg493365/#msg493365

 Thanks goes also to Technogeeky, I have re-used part of this code (https://github.com/technogeeky/keithley-197), turning it around, so to speak, to simulate the motherboard
 
 See README.md for more information

 Arduino Nano connections (please see Fritzing diagram for the full circuit):
 RCL:   pin D2
 STO:   pin D3
 RCL:   pin D4
 dB:    pin D5
 RESET: pin D6
 C/D:   pin D7
 CS:    pin D8
 Clock: pin D9
 MOSI:  pin D11
 SCK:   pin D13
 
*/

// Useful Strings:
// 123456789A123456789B123456789C
// MIN BAT RCL AC dB STO REL AUTO
// mA k V u M mV
// RMT A OHM C
// 1-6abcdefgp

#include <string.h> 
#include <TimerOne.h>

// the sensor communicates using SPI, so include the library:
#include <SPI.h>

// pins used for the connection with the board
// the other you need are controlled by the SPI library):
#include "aButton.h"
#include "K197_set.h"

const int rclPin = 2;
const int stoPin = 3;
const int relPin = 4;
const int dbPin = 5;
const int chipResetPin = 6;
const int chipCdPin = 7;
const int chipSelectPin = 8;
const int clockPin = 9;
// SPI MOSI, D11
// SPI SCK,  D13

aButtonGroup buttons;

byte startupSeq[] = {};
byte initSeq[]    = { 0x10, 0x20, 0x30, 0x4A, 0x14, 0x00, 0x18, 0x11, 0x20 };
byte cmdSeq[]     = {       0x20, 0x30, 0x4A, 0x14, 0x00, 0x18, 0x11, 0x20 };
byte dataSeq[]    = {       0x01, 0x00, 0x00, 0xEB, 0x23, 0x00, 0x00, 0x02, 0x02 };

void myButtonCallback(uint8_t buttonPin, uint8_t buttonEvent) {
    short mbit=MODE_BIT_NONE;
    switch(buttonPin) {
        case dbPin:  Serial.print(F("BUTTON dB: "));  mbit=MODE_BIT_dB;  break;
        case relPin: Serial.print(F("BUTTON REL: ")); mbit=MODE_BIT_REL; break;
        case stoPin: Serial.print(F("BUTTON STO: ")); mbit=MODE_BIT_STO; break;
        case rclPin: Serial.print(F("BUTTON RCL: ")); mbit=MODE_BIT_RCL; break;
        default: Serial.print(F("")); Serial.print(buttonPin); Serial.print(F(": "));
    }
    aButtonGroup::Serial_printEventName(buttonEvent); Serial.println();
    if ( (buttonEvent == aButtonGroup::eventClick) && (mbit!=MODE_BIT_NONE) ) {
        toggle_mode_bit(dataSeq, mbit);
    }
}

/// --------------------------------------------------------------------------------
/// Serial Menu
/// --------------------------------------------------------------------------------
void printPrompt() {
  Serial.println(); Serial.print(F("> ")); Serial.flush();
}

void printHelp(void) {
  Serial.println();
  Serial.println(F(" ? - print this help text"));
  Serial.println(F(" MIN[US] | BAT | RCL | AC | dB | STO | REL | AUTO ==> toggle mode bit"));
  Serial.println(F(" mA | k | V | u | M | mV  ==> toggle unit bit"));
  Serial.println(F(" RMT | A | OHM | C        ==> toggle unit2 bit"));
  Serial.println(F(" n[-m]<seg> to turn on segments on display n[to m]"));
  Serial.println(F(" where n,m: 1-6, <seg> any combination of abcdefgp"));
  Serial.println(F(" bla  ==> add event"));

  printPrompt();
}
#define INPUT_BUFFER_SIZE 30 

inline boolean validDigit(char c) {
    return ( c >= '1') && (c <='6');
}

inline boolean validSegment(char c) {
    boolean result=false;
    switch(c) {
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'p':  result=true; break;
    }
    return result;
}

//7  6  5  4  3  2  1  0
//c, b, d, g, a,dp, e, f
inline uint8_t segment2bit_number(char c) {
    uint8_t result=0;
    switch(c) {
        case 'a': result=3; break;
        case 'b': result=6; break;
        case 'c': result=7; break;
        case 'd': result=5; break;
        case 'e': result=1; break;
        case 'f': result=0; break;
        case 'g': result=4; break;
        case 'p': result=2; break;
    }
    return result;
}

inline boolean isDash(char c) {
    return c == '-' ? true : false; 
}

void handleSegments(char*token, uint8_t seg1, uint8_t seg2) {
    uint8_t segdata=0x00;
    while (*token!=0x00) {
        if (!validSegment(*token)) {
            printError(token);
            return;
        }
        bitSet(segdata, segment2bit_number(*token));
        token++;
    }
    if (seg2<seg1) {
        Serial.println(F("Internal error in handleSegments")); 
        return;
    }
    for(int i=seg1; i<=seg2; i++) {
        dataSeq[digit2index(i)] = segdata;   
    }
}

void printError(char*buf) {
    Serial.print(F("Invalid command: ")); Serial.println(buf);  
    printHelp();
}

void handleSerial(void) {
    static char buf[INPUT_BUFFER_SIZE];
    size_t i =Serial.readBytesUntil(' ', buf, INPUT_BUFFER_SIZE);
    buf[i]=0;
    if (i==0) { // no characters read
       return;  
    }
    // Check help
    if (strcasecmp("?", buf) == 0 ) {
          printHelp();
          return;    
    }
    
    // Check MODE_BIT commands
    short xbit=MODE_BIT_NONE;
    if (        strcasecmp("AUTO", buf) == 0 ) {
        xbit = MODE_BIT_AUTO;
    } else if ( strcasecmp("REL", buf) == 0  ) {
        xbit = MODE_BIT_REL;
    } else if ( strcasecmp("STO", buf) == 0  ) {
        xbit = MODE_BIT_STO;
    } else if ( strcasecmp("dB", buf) == 0   ) {
        xbit = MODE_BIT_dB;
    } else if ( strcasecmp("AC", buf) == 0   ) {
        xbit = MODE_BIT_AC;
    } else if ( strcasecmp("RCL", buf) == 0  ) {
        xbit = MODE_BIT_RCL;
    } else if ( strcasecmp("BAT", buf) == 0  ) {
        xbit = MODE_BIT_BAT;
    } else if ( strcasecmp("MIN", buf) == 0  ) {
        xbit = MODE_BIT_MINUS;
    } else if ( strcasecmp("MINUS", buf) == 0  ) {
        xbit = MODE_BIT_MINUS;
    }
    if (xbit!=MODE_BIT_NONE) {
       toggle_mode_bit(dataSeq, xbit);
       return;
    }

    // Check UNITS_BIT commands
    xbit=UNITS_BIT_NONE;
    if (        strcasecmp("mV", buf) == 0 ) {
        xbit = UNITS_BIT_mV;
    } else if ( strcasecmp("M", buf) == 0  ) {
        xbit = UNITS_BIT_M;
    } else if ( strcasecmp("u", buf) == 0  ) {
        xbit = UNITS_BIT_micro;
    } else if ( strcasecmp("V", buf) == 0  ) {
        xbit = UNITS_BIT_V;
    } else if ( strcasecmp("K", buf) == 0  ) {
        xbit = UNITS_BIT_K;
    } else if ( strcasecmp("mA", buf) == 0 ) {
        xbit = UNITS_BIT_mA;
    } 
    if (xbit!=UNITS_BIT_NONE) {
       toggle_units_bit(dataSeq, xbit);
       return;
    }

    // Check UNIT2_BIT commands
    xbit=UNIT2_BIT_NONE;
    if (        strcasecmp("C", buf) == 0   ) {
        xbit = UNIT2_BIT_C;
    } else if ( strcasecmp("OHM", buf) == 0 ) {
        xbit = UNIT2_BIT_OHM;
    } else if ( strcasecmp("A", buf) == 0   ) {
        xbit = UNIT2_BIT_A;
    } else if ( strcasecmp("RMT", buf) == 0 ) {
        xbit = UNIT2_BIT_RMT;
    }    
    if (xbit!=UNIT2_BIT_NONE) {
       toggle_unit2_bit(dataSeq, xbit);
       return;
    }

    //check if 7 segment (+dp) specification
    char*token=buf;
    uint8_t seg1; uint8_t seg2;
    if ( validDigit(*token)  ) {
       seg1 = ((*token)-'0');
       seg2=seg1;
       token++;
       if (isDash(*token)) {
          token++;
          if (validDigit(*token)) {
              seg2 = ((*token)-'0');
              token++;
          } else {
              printError(token);
              return;
          }
       }
       handleSegments(token, seg1, seg2);
       return;
    }  else {
        printError(token);
        return;
    }
    
}

void setup() {
    Timer1.initialize(12);  // 12 us = 83.333 kHz instead of 81.92 (according to display controller data sheet 2-16 us is acceptable)
    Timer1.pwm(clockPin, (50.0 / 100) * 1023 );
  
    Serial.begin(115200);
    delay(2000);
    Serial.println(F(""));
    Serial.print(F("startupSeq:"));Serial.println(sizeof(startupSeq)/sizeof(byte));
    Serial.print(F("initSeq:"));Serial.println(sizeof(initSeq)/sizeof(byte));
    Serial.print(F("dataSeq:"));Serial.println(sizeof(dataSeq)/sizeof(byte));

    buttons.add(dbPin,  myButtonCallback);
    buttons.add(relPin, myButtonCallback);
    buttons.add(stoPin, myButtonCallback);
    buttons.add(rclPin, myButtonCallback);
  
    pinMode(11, OUTPUT);
    digitalWrite(11, LOW); 

    // initialize the  data ready and chip select pins:
    pinMode(chipSelectPin, OUTPUT);
    digitalWrite(chipSelectPin, HIGH); // This pin is active low

    pinMode(chipCdPin, OUTPUT);
    digitalWrite(chipCdPin, LOW); // Prepare for data

    // start the SPI library:
    SPI.begin();

    // Reset the LCD driver chip
    pinMode(chipResetPin, OUTPUT);
    digitalWrite(chipResetPin, LOW); 
    delay(2000);
    digitalWrite(chipResetPin, HIGH); 

    beginTransaction();
    writeInitialCommandSequence(startupSeq, sizeof(startupSeq)/sizeof(byte));
    writeCommandDataSequence(initSeq, sizeof(initSeq)/sizeof(byte), dataSeq, sizeof(dataSeq)/sizeof(byte));
    endTransaction();
    delay(1000);
}

static unsigned long display_seq_timer = 0L;

void loop() {
    unsigned long now = millis();
    if (now-display_seq_timer>300) {
         display_seq_timer=now;
         beginTransaction();
         writeCommandDataSequence(cmdSeq, sizeof(cmdSeq)/sizeof(byte), dataSeq, sizeof(dataSeq)/sizeof(byte));
         endTransaction();
    }
    buttons.check();
    if (Serial.available() ) {
        handleSerial();
    }

}

void beginTransaction(void) {  // All transactions begin as Command. Set chipCdPin to LOW when it's time to transfer data
  SPI.beginTransaction(SPISettings(50000, MSBFIRST, SPI_MODE0));
  // take the chip select low to select the device:
  digitalWrite(chipSelectPin, LOW);
  delayMicroseconds(5);
  digitalWrite(chipCdPin, HIGH);
  delayMicroseconds(1000);
}

void endTransaction(void) {
  delayMicroseconds(2500);
  digitalWrite(chipCdPin, LOW);
  digitalWrite(chipSelectPin, HIGH);
  SPI.endTransaction();
}

void writeInitialCommandSequence(const byte *cmd, short cmdlen) { //Note: this will overwrite cmd[]! Use with a dedicated buffer and only during setup!
  for (int i=0; i<cmdlen; i++) {
     SPI.transfer(cmd[i]);    // Send one byte at a time
     delayMicroseconds(368);  // Give it time... this is an old, slow display driver!!!
     if (i==7) {
        delayMicroseconds(810);
     }     
  }
  delayMicroseconds(1000);
}

void writeCommandDataSequence(const byte *cmd, short cmdlen, const byte *data, short datalen) {
  for (int i=0; i<cmdlen; i++) {
     SPI.transfer(cmd[i]);    // Send one byte at a time
     delayMicroseconds(368);  // Give it time... this is an old, slow display driver!!!
     if (i==6) {
        delayMicroseconds(810);
     }     
  }
  digitalWrite(chipCdPin, LOW);
  delayMicroseconds(230);
  for (int i=0; i<datalen; i++) {
     SPI.transfer(data[i]);    // Send one byte at a time
     delayMicroseconds(368);   // Give it time... this is an old, slow display driver!!!
  }

  delayMicroseconds(2500);
}
