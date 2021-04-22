Thu Apr 22 04:33:24 UTC 2021

Designed for and tested on Adafruit ItsyBitsy RP2040.

Likely to work on Feather RP2040 without modification.

  monitor-dd.ino - empty sketch
  n_monitor-dd_SKETCH.cpp - real sketch
  reflash.cpp - reflash routine

Different instruction set from monitor-bb? Certainly
from monitor-aa.

Using more defines (C preprocessor) to differentiate
between opcodes and inline data.

The present work (monitor-dd) demos LIT using both forms
(the C preprocessor define for it, and opcode 11 - they
are interchangeable, since cpp simply substitutes the
one for the other, prior to compilation).

# monitor

The monitor is named after traditional microcontroller
'monitor' programs.

It's job is somewhat primitive; it is a stand-in for
a more fully developed system (generally, with the
goal of reprogramming that system, in some way, or
at least interacting with it, possibly using a
command/control loop interface - usually via serial).

# NEWS

Simplfied instruction set - stack implemented.

Stack report now available, and collated in the same
order as typed.  In traditional forth:

 1 2 3 4  ok
 .s
 <4> 1 2 3 4  ok

With no work-around, it came out like this, instead:

 1 2 3 4  ok
 .s
 <4> 4 3 2 1  ok

That was not wanted, so some 'clever' work was put into it,
to make it the other way (reverse collated from what was
otherwise 'natural').

The 'clever' work itself needs work, to sort out why it
behaves as it does (it needs factoring).

Colorized intent - not sure where it'll go. ;)

Program waits for serial connection (indefinitely) and
blinks the onboard LED to indicate it is 'listening'
for a connection made to the target, on /dev/ttyACM0
(linux ref. for CDC/ACM USB).

Reflash

Program will automatically initiate a reflash (mount USB
drive where host PC can reflash by dropping a new version
of the firmware - in .UF2 format - on the flash as presented).

Between signon and reflash the program does all the work
it intends to. ;)

Right now that's just a repetitive message of sending an
'A' every second or so.

