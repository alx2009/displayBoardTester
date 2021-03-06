# displayBoardTester
Tester for Keithley 197/197A display board

This is a Arduino sketch to test a Keithley 197/197A display board.

The Keithley 197 / 197A is an old 5.5 digits bench multimeter. You can read about the multimeter here: https://www.eevblog.com/forum/testgear/keithley-197a-owners_-corner/

The meter is quite good but often the LCD display starts to fade, due to the zebra strip contacts failing. A user reversed engineer the display protocol (see  https://www.eevblog.com/forum/projects/keithley-197-led-display-hack/msg493365/#msg493365), in order to develop a replacement boards using a microcontroller.

This inspired me to do the same for my own instrument. However developing a new board using the instrument itself is not ideal. A mistake can damage the instrument. Not to mention the hazard posed by the exposed mains and the fact the ground of the board under development is shared with the device under test (if you do not understand the implication trust me. You do not want that).

As a first step designing my own replacement board (links will be included when available) I have developed a simple simulator based on an Arduino nano.

The Arduino simulates all the signals and voltage levels available on the Keithley main board. The timing are not exacly the same, but are close enough that the original display board works with the simulator.

![20220215_221436](https://user-images.githubusercontent.com/100997527/173181555-853b9f6d-0e15-4a9f-8c42-10a190db829e.jpg)

I am using this simulator to breadboard my replacement board and write the software. The idea is to reduce the amount of test on the target.

I am sharing the project as a thank you to the community. I hope that it will help and inspire others to bring back to life old test and measurement gear. And do it in a safer way! 

The Fritzing folder includes a Fritzing project with schematic and breadboard view (with PNG pictures if you do not have Fritzing), designed for a 197A, with a 16 pin DIL socket. Pin 1 and 16 are used by the backlight.

The 197 has a 14 pin socket, as it does not have the backlight. Everything else is the same. Connect it so that pin 1 and 16 of the simulator DIL socket are left unconnected, maintaining the same orientation (Pin 1 of the display connects to pin 2 of the simulator, etc.). 

INSTALLATION: 
- Connect the Arduino as shown in the Fritzing project (197A, see above for 197). Connect the display board to the DIL socket of the simulator as you would to the real board. 
- Download the entire content of this repository from gitHub. You should have displayBoardTester.ino in a folder called displayBoardTester, together with all the .h and .c files. 
- Open displayBoardTester.ino with the Arduino IDE (I used 1.8.19 so I can only attest that it will work there). 
- Select Arduino Nano (included in Arduino AVR board)
- Install library TimerOne if not already installed (see here for more information https://playground.arduino.cc/Code/Timer1/)
- Done! You can now compile and download the SW onto the Arduino. You control the SW via Serial monitor (see below)  

Principles of operation:
- A voltage diveder made with three 10K resistors generates the 3.3V and 1.66V needed by the display board. This is the same method used in the Keithley main board
- The timerOne library is used to generate a 83.333 kHz (not exactly the same as the original board, but close enough and within specs of the driver chip)
- The standard SPI library is used to generate the customary SPI signals. the SPI clock has been selected to be as close as possible to the original, some delayMicroseconds added as needed to replicate the original timing.
- RESET and C/D signals are generated via digitalWrite(). A 4.7K pull down resistor is used on the RESET pin as this is what is used in the original board.
- Arduino pins D2-D5 read the button presses (22K pull-down resistors are used as in the original board)
- The simulator is controlled via Serial Monitor. You can turn on and off any segment in this way.

Serial commands
---------------
? print a summary of all the recognized commands

The following commands toggle the corresponding segment on the display on and off: 
MIN BAT RCL AC dB STO REL AUTO mA k V u M mV RMT A OHM C
show text> ==> show the string "text" (> is the optioptional separator)  
clear ==> clears the display
Entering n[-m]<seg> controls the numeric display at the segment level. Where:
  <seg> identifies the segment(s) (abcdefgp)
  n,m are numbers from one to 6 that identify the digit
    
The show command supports the following characters: &'*-/0123456789=?@ACDEFGHIJKLMNPQRTUVWXY\^_abcdefghijnoqrtuy
unsupported characters turn off al segments.

example 1:
the following sequence:
show 123.456>

will show 123.456 on the display

example 2:
if the display is completely off, the following sequence:
    k OHM 3abcdefg
    
Will show "8 k Ohm" on the display
  
ALX2009


