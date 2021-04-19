// ITC-Forth.ino
// Mon Apr 19 17:25:39 UTC 2021

//    TESTED locally - runs unmodified, no need to #include anything at all.

/*
   from: https://github.com/CharleyShattuck/Steno-Keyboard-Arduino.git
*/

#include <Arduino.h>

#define RAM_SIZE 0x1200
#define S0 0x1000
#define R0 0x0f00

// "registers"
int S = S0; // data stack pointer
int R = R0; // return stack pointer
int I = 0; // instruction pointer
int W = 0; // working register

const int memory [] {
  1, // print A
  2, // delay 1 sec
  3, // branch
  0, // to this address
};

void runForth () {
next:
  W = memory [I++];
  switch (W) {
    case 1:
    A:
      Serial.write ('A');
      goto next;
    case 2:
    _delay:
      delay (1000);
      goto next;
    case 3:
    branch:
      I = memory [I];
      goto next;
  }
}

void setup () {
  Serial.begin (9600);
  I = 0;
  S = S0;
  R = R0;
  runForth ();
}

void loop () {
  while (1) {
//    Serial.write ('A');
//    delay (1000);
  };
}

// End
