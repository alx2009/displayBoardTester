# displayBoardTester
Tester for Keithley 197/197A display board

This is a Arduino sketch to test a Keithley 197/197A display board.

The Keithley 197 / 197A is an old (but still very good) 5.5 digits bench multimeter. You can read about the multimeter here: https://www.eevblog.com/forum/testgear/keithley-197a-owners_-corner/

As said the meter is quite good but over the years the LCD display starts to fade, due to the zebra strip contacts failing. A user reversed engineer the display protocol (see  https://www.eevblog.com/forum/projects/keithley-197-led-display-hack/msg493365/#msg493365), in order to develop a replacement boards using a microcontroller.

This inspired me to do the same for my own instrument. However developing a new board using the instrument itself is not ideal. A mistake can damage the instrument. Not to mention the hazard posed by the exposed mains and the fact the ground of the board under development is shared with the device under test (if you do not understand the implication trust me. You do not want that).

As a first step designing my own replacement board (links will be included when available) I have developed a simple simulator based on an Arduino nano.

The Arduino simulates all the signals and voltage levels available on the Keithley main board. The timing are not exacly the same, but are close enough that the original display board works with the simulator.

![20220215_221436](https://user-images.githubusercontent.com/100997527/173181555-853b9f6d-0e15-4a9f-8c42-10a190db829e.jpg)

I am using this simulator to breadboard my replacement board and write the software. The idea is to reduce the amount of test on the real target, at least until the HW is stable enough to close the instrument.

I am sharing the project under a MIT license as a thanks to the community. I hope that it will help and inspire others to bring back to life old test and measurement gear. And do it in a safer way! 

The Fritzing folder includes a Fritzing project with schematic and breadboard view (with PNG pictures if you do not have Fritzing). 

INSTALLATION of the sketch: 
- Download the entire content of this repository from gitHub. you should have displayBoardTester.ino in a folder called displayBoardTester, together with all the .h and .c files. 
- Open displayBoardTester.ino with the Arduino IDE (I used 1.8.19 so I can only attest that it will work there). 
- Select Arduino Nano (included in Arduino AVR board)
- Install library TimerOne if not already installed (see here for more information https://playground.arduino.cc/Code/Timer1/)
- done! You can now compile and download the SW onto the Arduino. You control the SW via Serial monitor (see below)  

Principles of operation:
- A voltage diveder made with three 10K resistors generates the 3.3V and 1.66V needed by the display board. This is the same method used in the Keithley main board
- The timerOne library is used to generate a 83.333 kHz (not exactly the same as the original board, but close enough and within specs of the drivr chip)
- The standard SPI library is used to generate the customary SPI signals. the SPI clock has been selected to be as close as possible to the original, some delayMicroseconds added as needed to replicate the original timing.
- RESET and C/D signals are generated via digitalWrite(). A 4.7K pull down resistor is used on the RESET pin as this is what is used in the original board.
- Arduino pins D2-D5 read the button presses (22K pull-down resistors are used as in the original board) 

ALX2009


