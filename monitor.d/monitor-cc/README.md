Mon Apr 19 21:38:41 UTC 2021

Just a copy from the earlier work - not edited (or not
edited much).

 - - -

Designed for and tested on Adafruit Feather RP2040.

Likely to work on ItsyBitsy RP2040 without modification.

  monitor-bb.ino - empty sketch
  n_monitor-bb_SKETCH.cpp - real sketch
  reflash.cpp - reflash routine

# monitor

The monitor is named after traditional microcontroller
'monitor' programs.

It's job is somewhat primitive; it is a stand-in for
a more fully developed system (generally, with the
goal of reprogramming that system, in some way, or
at least interacting with it, possibly using a
command/control loop interface - usually via serial).

# NEWS

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

