# Open Badge Project

![](<images/badge.gif>)

This is the Open Badge Project. The idea behind this is to have a very low cost, low power electronic badge. These could be built just for fun, or if you're throwing a conference, feel free to use the PCB as the badge, maybe build some, maybe let the folks at the conference build them.

Electronic badges have gotten pretty hip, but they seem to suffer from two primary problems.
1) They are very expensive
1) They need a lot of batteries

The goal of the Open Badge Project was to build a badge that costs $5 or less and can run for a day or more on one 2032 battery.

The badge as it stands right now will only cost $5 if you build A LOT (1000 or more). The cost is closer to $6 or $7 for smaller batches at the moment, but that's still pretty reasonable.

### Types
There are two versions of the badge. One is a through hole badge that's very easy to solder. It was designed with human fabricators in mind. The kicad files in the main branch are for the through hole model.

There is also a surface mount version in the [surface-mount](https://github.com/joshbressers/badge/tree/surface-mount) branch. This version of the badge can be sent off to a fabricator to be mass produced.

Both badge versions use the same code.

## How does it work?
The badge itself runs on a microprocessor called an ATTINY85. It has 8K of program space, 512 bytes (yes, bytes) of memory. That microprocessor then drives 3 shift registers. One for the button input, and two for the display output. Shift registers shouldn't be used to drive LEDs, but it works, and doing it this way keeps the cost down.

The PCB can be shipped off to one of the countless PCB manufacturers that exist. There is room on the front for a logo. There is also plenty of room on the back for graphics. Instructions for swapping in your own logo can be found below.

The PCB was designed using KiCad, version 7.0 to be precise. Your mileage may very if you try to use other versions.

![](<images/PCB.png>)

## Parts List
You can generate a bill of materials from KiCad, but for the sake of simplicity here is a list of all the parts.

- 1 [ATTINY85](https://www.digikey.com/en/products/detail/microchip-technology/ATTINY85-20PU/735469)
- 1 PCB (you have to get this made, you can't buy them)
- 2 [CD74HC595E](https://www.digikey.com/en/products/detail/texas-instruments/CD74HC595E/1507396) shift registers
- 1 [CD4021BE](https://www.digikey.com/en/products/detail/texas-instruments/CD4021BE/67261) shift register
- 1 [2032 battery holder](https://www.digikey.com/en/products/detail/adam-tech/BH-25C-1/13537703)
- 8 [100 Ohm resistors](https://www.digikey.com/en/products/detail/stackpole-electronics-inc/CF14JT100R/1741261)
- 8 [100K Ohm resistors](https://www.digikey.com/en/products/detail/stackpole-electronics-inc/CF14JT10K0/1741265)
- 6 [Buttons](https://www.amazon.com/gp/product/B07VSNN9S2/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&th=1)
- 1 [8x8 LED Display](https://www.aliexpress.us/item/3256805713726832.html?spm=a2g0o.order_list.order_list_main.11.64611802KkD7xH&gatewayAdapt=glo2usa)

There may be better places to purchase these things than these links. Please submit a PR if you find something better.

## How to change the logo
KiCad has a tool called "Image Converter". This application lets you turn an image into a footprint that can be added to the PCB layout.

When you run the tool, there is a button on the left called "Load Source Image". If you have a black and white image to start with that's the best. You can use any image but things get weird.

Once you load an image you will need to watch the size. The "Output Size" field is what matters here. It will take a few tries to get it right. There's about 3 inches by 1.7 inches of space to work with on the front.

Use the "Front Solder Mask" layer. Make sure you use Footprint for the output layer.

Click Export to file. Save the file in the badge-footprints.pretty directory.

Now open the "PCB Editor" application. Make sure the "F. Silkscreen" layer is selected on the right.

You will have to click on and delete the existing logo. Now from the menu Place -> Add Footprint

If you search for "badge" in the text box you should see your logo show up in the list. Double click it, then place it where it fits. If the logo isn't sized right, go back to the Image Converter and try again.

## How to order PCBs
If you want to have PCBs made, you need to export the Gerber files from the PCB Editor. The Gerber files are what you send the PCB manufacturer.

Run the PCB Editor, then File -> Fabrication Outputs -> Gerbers

Create a new directory to hold the files (there will be more than one). For example let's call it badge_gerber. Leave the default settings alone, it should do the right thing.

Then click Plot. Then click "Generate Drill Files" and "Generate Drill File" on the window that pops up. Your directory now has the files need to create the PCB in it.

The PCB manufacturer will want that directory turned into a zip file and uploaded. You're on your own to figure that part out.

## How to program the chip

In order to program the ATTINY85 you will need a programmer. There is a lovely page that explains how to use an arduino as your [programmer](https://www.instructables.com/How-to-Program-an-Attiny85-From-an-Arduino-Uno/)

The reset pin on this chip is special. We are purposely not using that pin (so we really only have 5 IO pins now) because if you use the reset pin, you now require a high voltage programmer. Since we want people to program this thing using only an arduino, we'll keep the reset pin free.

## How to develop for the badge
See [DEVELOPING.md](DEVELOPING.md)
