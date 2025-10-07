## Developing for the badge

The code that makes everything go is in the badge directory. This is meant to be used with the Arduino development environment.

If you haven't done so, you should follow the instructions in the README about programming the ATTINY85 with an Arduino. Even if you're not using an Arduino to program the chip, there are instructions there for getting the Arduino environment configured. Both 1.x and 2.x Arduino IDEs work.

The files are hopefully well commented so what's happening in them is clear. Keep in mind that most decisions made were to make things as simple and efficient as possible. There's very little memory and not much program space. Sometimes using more memory was used to make things easier for humans. Like using floats in the pong game. Integers take up less room and would be faster, but would be harder to understand the code.

The best way to figure this thing out is to play around with the code. Look at the examples and ask questions (file all the issues you want).

## File layout

There are a few critical files for the operation of the badge.

### badge.ino
The [badge.ino](badge/badge.ino) file is where the setup and main loop exist. If you've ever programmed for an Arduino before, this should be familiar to you. If you haven't, you may want to look at some Arduino programming tutorials first.

### constants.h
[constants.h](badge/constants.h) is where all the constants are means to be held. The stuff you can modify is at the top.

These are the things used across the various functions that make the badge work. When possible you should use #define instead of declaring variables. There's not much memory, variables are expensive. This file should be very well commented. If something isn't clear please file an issue.

### font.h
[font.h](badge/font.h) is where the font to scroll a message across the screen exists. It's an 8x5 font that takes up 5 bytes per character.

For example the letter A is:
```
0x7C, 0x12, 0x11, 0x12, 0x7C,
```
If we turn that into binary, we can see what the letter looks like

```
01111100
00010010
00010001
00010010
01111100
```
or if we rotate it and use some friendlier characters
```
..#..
.#.#.
#...#
#...#
#####
#...#
#...#
.....
```
Every byte is a column in the font

The font characters are in ASCII order, so we can address them by converting a byte character into a uint8_t and subtracting 0x20.

If you want to add new characters, do it at the end of the array.

### shift_registers.ino

The [shift_registers.ino](badge/shift_registers.ino) file is where we read/write to the shift registers. This is how we update the screen and read the buttons. This is the file that actually makes all the hardware work.

The various millis() functions of the Arduino use up memory. Rather than try to use those, we have a variable called currentTick in this file that ticks up one every loop of the application and reads/writes the shift registers. Every subroutine has to call the runTick() function while it loops.

We could consider using a timer interrupt for this someday. How to handle a partially updated frameBuffer is why this hasn't happened yet (it's possible things update fast enough it won't matter). NOTE: I tried doing this in a [branch](https://github.com/joshbressers/badge/tree/interrupt), it's not very good.

The shiftRegisters() function is mostly unexciting but also drives everything. It pulses the latches and clocks then reads/writes the data. Whatever is in the frameBuffer variable gets written to the screen. Whatever buttons are pressed get written to CUR_BUTTON.

### message.ino
[message.ino](badge/message.ino) is the file that scrolls text on the screen. It has a lot of comments, go read them.

Most of the text that is displayed is stored in PROGMEM. This is a special variable in the Arduino universe that places the text in the 8K of program space instead of our 512 bytes of RAM. As such, PROGMEM needs to be read differently than a normal variable.

We have a variable in the code called a "progmemMessage". It's possible to print a string from memory, or a string from PROGMEM space. You just have to call either setMessage for PROGMEM strings or setMemMessage for memory strings.

This is all confusing, the printScore function is probably worth looking at. There are probably ways to make this simpler with some C++ magic.

### menu.ino
[menu.ino](badge/menu.ino) is where we drive the main menu from. If you want to add a program, you have to register it here. It's pretty easy to understand, go look at it.

### tests.ino
[tests.ino](badge/tests.ino) contain two tests. One to test the buttons and one to show a pattern on the screen. They don't take up very much space and help with debugging.

### move_dot.ino
[simple_games.ino](badge/simple_games.ino) is where the games are currently held. There's no reason to add games here, it was a way to avoid adding too many files.

### blocks.ino
[blocks.ino](badge/blocks.ino) is a tetris like clone. It has some bugs and isn't amazing. Patches welcome

### rain.ino
[rain.ino](badge/rain.ino) is a little tool that draws rain waves on the screen. It's interesting because of the circle drawing function
