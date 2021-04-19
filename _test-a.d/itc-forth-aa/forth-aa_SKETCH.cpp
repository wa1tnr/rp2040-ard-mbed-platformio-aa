// ITC-Forth.ino
// Mon Apr 19 18:16:22 UTC 2021

/*
   from: https://github.com/CharleyShattuck/Steno-Keyboard-Arduino.git
*/

#include <Arduino.h> // mandatory for empty .ino file
#define REVISION_ITCF "0.1.0-a.6"

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

void blink (void) {
  // get LED blinking
}

void await_serial(void) {
  while(!Serial); // kludge
  blink ();
  delay(4000);
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
  digitalWrite(led, 1);
  delay(8000);
  digitalWrite(led, 0);
}

void reflash(void) { // too early?
  // statements
  reflash_firmware();
}

void setup () {
  Serial.begin (9600);
  pre_serial();
  hardware_setup();
  I = 0;
  S = S0;
  R = R0;
  runForth ();
  reflash();
}

void loop () {
  while (1) {
//    Serial.write ('A');
//    delay (1000);
  };
}

// End
