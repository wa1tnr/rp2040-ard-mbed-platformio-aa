Sun Apr 25 16:07:02 UTC 2021

Designed for and tested on Adafruit Feather RP2040.

Likely to work on ItsyBitsy RP2040 without modification.

  monitor-hh-p.ino - empty sketch
  n_monitor-hh-p_SKETCH.cpp - real sketch
  reflash.cpp - reflash routine
  dump_ram.cpp - dump ram block 256 bytes increment addrs

Newest work (the -p branch) adds a sub-processor command
and interpret loop (Forth-like Interpreter) as scaffolding,
to be used to freely examine the RAM.

Older work maps so that the ram dump looks more like
the code (ex. 'dly' appears in the right columns for
'delay' and 'lit' for 'literal').

Using more defines (C preprocessor) to differentiate
between opcodes and inline data.  This has progressed.


# monitor

The monitor is named after traditional microcontroller
'monitor' programs.

It's job is somewhat primitive; it is a stand-in for
a more fully developed system (generally, with the
goal of reprogramming that system, in some way, or
at least interacting with it, possibly using a
command/control loop interface - usually via serial).

# command loop subprocessor

Traditional Forth - used to examine the (new) primary.
Meant as temporary scaffolding to construct the new one
more easily.

# NEWS

SRAM space (at it were) more fully investigated - seems
to do what is wanted (rewritable during program exec.)

A more true 'SRAM' stand-in is now used - the original
array was declared 'const int memory []' whereas the
new array drops the 'const' qualifier.

The old one is copied over to the new one, during
program setup.  The old one is still used to specify
the program (in its customary mnemonics).

The new buffer is where the switch/case construct looks
for its instructions (the 'program' that it follows
and tries to execute .. usually successfully).

The new buffer is

   int RAMSPACE [RAM_SIZE];

 $  egrep RAM_SIZE n_monitor-ff_SKETCH.cpp 

// #define RAM_SIZE 0x1200

   #define RAM_SIZE 0x2C8

   int RAMSPACE [RAM_SIZE];


# Older News

op_code table defines altered significantly, with the
advent of a ram dumper.

Now they read well in the right (translated to ASCII)
columns, for ease of decoding from a dump.

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

