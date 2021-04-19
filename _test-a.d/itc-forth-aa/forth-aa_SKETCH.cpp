// forth-aa_SKETCH.cpp
// Mon Apr 19 20:11:47 UTC 2021

// was: ITC-Forth.ino

/*
   from: https://github.com/CharleyShattuck/Steno-Keyboard-Arduino.git
*/

#include <Arduino.h>
#define REVISION_ITCF "0.1.0-b.1"

#define RAM_SIZE 0x1200
#define S0 0x1000
#define R0 0x0f00

extern void reflash_firmware(void); // prototype

// hardwara GPIO
int led = LED_BUILTIN;

// "registers"
int S = S0; // data stack pointer
int R = R0; // return stack pointer
int I = 0; // instruction pointer
int W = 0; // working register

int reflash_timeout = 7; // seconds

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
      reflash_timeout--;
      Serial.write ('A');
      goto next;
    case 2:
    _delay:
      delay (1000);
      goto next;
    case 3:
    branch:
      I = memory [I];
      if (reflash_timeout == 0) return;
      goto next;
  }
}

void blink (void) {
  digitalWrite(led, 1);
  delay(55);
}

void full_blank(void) {
  digitalWrite(led, 0);
}

void fast_blank(void) {
  int counter = 3;
  do {
    if (Serial) return;
    delay(30);
    counter--;
  } while (counter > 0);
  full_blank();
}

void await_serial(void) {
  while(!Serial) {
    blink ();
    if(!Serial) fast_blank();
    full_blank();
    delay(4000);
  }
  Serial.print("Rev. ");
  Serial.println(REVISION_ITCF);
}

void pre_serial(void) {
  while(!Serial) {
    await_serial();
  }
}

void hardware_setup(void) {
  pinMode(led, OUTPUT);
}

void reflash(void) {
  reflash_firmware();
}

void setup () {
  Serial.begin (9600);
  hardware_setup();
  pre_serial();
  I = 0;
  S = S0;
  R = R0;
  runForth (); // loop; not easily escaped
  reflash();
}

void loop () {
  while (1) {
//    Serial.write ('A');
//    delay (1000);
  };
}

// End
