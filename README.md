# LPD8806 Strip Wrapper Library

Classes to abstract LPD8806 LED flex strips, allowing you to easily write to them as a grid (A strip, which has been cut and oriented so that it zig-zags creating a two dimensional grid of pixels).

Based off of the Adafruid library (https://github.com/adafruit/LPD8806)

## To get past the 2k heap size issue

In the mpide.app IDE application, go to `Contents > Resources > Java > Hardware > pic32 > platforms.txt` and edit `platforms.txt`.

Find this line:

    pic32.compiler.c.elf.flags=-Os::-Wl,--gc-sections::-mdebugger

Change it to:

    pic32.compiler.c.elf.flags=-Os::-Wl,--gc-sections,--defsym,_min_heap_size=32768::-mdebugger

This sets the heap size to a minimum of 32k.  Adjust this value as needed.
