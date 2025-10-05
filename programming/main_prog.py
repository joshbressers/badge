# SPDX-FileCopyrightText: 2021 ladyada for Adafruit Industries
# SPDX-License-Identifier: MIT

"""
ATtiny13a programming example, be sure you have the '13a wired up so:
  ATtiny13a GND to CircuitPython GND
  ATtiny13a VCC to CircuitPython USB
  Pin 2 -> CircuitPython SCK
  Pin 1 -> CircuitPython MISO
  Pin 0 -> CircuitPython MOSI
  RESET  -> CircuitPython D5 (or change the init() below to change it!)
Drag "attiny13a_blink.hex" onto the CircuitPython disk drive, then open REPL!
"""

import sys
import time
import board
import busio
import microcontroller
import microcontroller.pin
import digitalio

import adafruit_avrprog

#sck = microcontroller.pin.GPIO18
#mosi = microcontroller.pin.GPIO19
#miso = microcontroller.pin.GPIO16

sck = microcontroller.pin.GPIO18
mosi = microcontroller.pin.GPIO19
miso = microcontroller.pin.GPIO16
reset = microcontroller.pin.GPIO10

spi = busio.SPI(sck, mosi, miso)
avrprog = adafruit_avrprog.AVRprog()
avrprog.init(spi, reset)
# Each chip has to have a definition so the script knows how to find it
attiny85 = avrprog.Boards.ATtiny85

switch = digitalio.DigitalInOut(board.GP0)
switch.switch_to_input(pull=digitalio.Pull.UP)

redLight = digitalio.DigitalInOut(board.GP8)
redLight.direction = digitalio.Direction.OUTPUT
yelLight = digitalio.DigitalInOut(board.GP7)
yelLight.direction = digitalio.Direction.OUTPUT
grnLight = digitalio.DigitalInOut(board.GP9)
grnLight.direction = digitalio.Direction.OUTPUT

grnLight.value = True
yelLight.value = False
redLight.value = False

def error(err):
    """Helper to print out errors for us and then halt"""
    print("ERROR: " + err)
    avrprog.end()

    grnLight.value = False
    yelLight.value = False
    redLight.value = True
    
    raise Exception(err)


# Main
while True:

    end_state = None

    # False is pressed
    if switch.value != False:
        time.sleep(0.5)
        continue

    time.sleep(1) # Debounce

    grnLight.value = False
    yelLight.value = True
    redLight.value = False
    try:
        
        if not avrprog.verify_sig(attiny85, verbose=True):
            error("Signature read failure")

        print("Found", attiny85["name"])

        fuses = avrprog.read_fuses(attiny85)

        print("Fuses")
        print(fuses)

        avrprog.write_fuses(attiny85, low=0x62, high=0xD7, ext=0x07, lock=0x3F)

        print("Programming flash from file")

        if not avrprog.program_file(attiny85, "badge.ino.hex", verbose=True, verify=True):
            error("Failure programming flash")
        avrprog.end()
    except Exception as e:
        print("Failure programming flash")
        print(e)
        avrprog.end()
        grnLight.value = False
        yelLight.value = False
        redLight.value = True
        end_state = False

    else:
        print("Done!")

        grnLight.value = True
        yelLight.value = False
        redLight.value = False
        end_state = True
    
    # wait for the switch to be opened
    while switch.value == False:
        time.sleep(0.5)
        if end_state == True:
            grnLight.value = not grnLight.value
        else:
            redLight.value = not redLight.value
        
    time.sleep(2) # This is our debouncer
    grnLight.value = True
    yelLight.value = False
    redLight.value = False
