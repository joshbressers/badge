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

import board
import busio
import microcontroller
import microcontroller.pin

import adafruit_avrprog

#sck = microcontroller.pin.GPIO18
#mosi = microcontroller.pin.GPIO19
#miso = microcontroller.pin.GPIO16

sck = microcontroller.pin.GPIO10
mosi = microcontroller.pin.GPIO11
miso = microcontroller.pin.GPIO12

#spi = busio.SPI(board.SCK, board.MOSI, board.MISO)
spi = busio.SPI(sck, mosi, miso)
avrprog = adafruit_avrprog.AVRprog()
avrprog.init(spi, microcontroller.pin.GPIO15)

# Each chip has to have a definition so the script knows how to find it
attiny85 = avrprog.Boards.ATtiny85


def error(err):
    """Helper to print out errors for us and then halt"""
    print("ERROR: " + err)
    avrprog.end()
    while True:
        pass


#while input("Ready to GO, type 'G' here to start> ") != "G":
#    pass

if not avrprog.verify_sig(attiny85, verbose=True):
    error("Signature read failure")
print("Found", attiny85["name"])

fuses = avrprog.read_fuses(attiny85)

#print("Fuses")
#print(fuses)

avrprog.write_fuses(attiny85, low=0x62, high=0xD7, ext=0x07, lock=0x3F)
#if not avrprog.verify_fuses(attiny13, low=0x7A, high=0xFF):
#    error("Failure verifying fuses!")

print("Programming flash from file")
avrprog.program_file(attiny85, "badge.ino.hex", verbose=True, verify=True)

print("Done!")