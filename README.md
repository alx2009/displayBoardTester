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

ALX2009


