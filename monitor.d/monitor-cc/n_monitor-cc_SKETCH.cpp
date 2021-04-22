// n_monitor-cc_SKETCH.cpp
// Thu Apr 22 00:41:04 UTC 2021

// was: n_monitor-bb_SKETCH.cpp
// was: forth-aa_SKETCH.cpp
// was: ITC-Forth.ino

/*
    from: https://github.com/CharleyShattuck/Steno-Keyboard-Arduino.git
*/

/*
  stack
  from: https://github.com/CharleyShattuck/Feather-M0-interpreter
*/

#include <Arduino.h>
#define REVISION_ITCF "0.1.0-f.6 - new minimal alpha"

#undef ADAFRUIT_ITSY_RP2040_ITCF
#define ADAFRUIT_ITSY_RP2040_ITCF

#ifndef ADAFRUIT_ITSY_RP2040_ITCF
    #define UNKNOWN_TARGET_RP2040_ITCF
#endif

#define SLOW_WAIT_AA 125

#define RAM_SIZE 0x1200
#define S0 0x1000
#define R0 0x0f00

/* Data stack for parameter passing
   This "stack" is circular,
   like a Green Arrays F18A data stack,
   so overflow and underflow are not possible
   Number of items must be a power of 2 */

const int STKSIZE = 8;
const int STKMASK = 7;
int stack[STKSIZE];
int p = 0;
int reflash_timeout = 0xCFFF; // a good six minutes here 0xCFFF

/* TOS is Top Of Stack */
#define TOS stack[p]


// hardwara GPIO
#ifdef UNKNOWN_TARGET_RP2040_ITCF
  #undef ADAFRUIT_ITSY_RP2040_ITCF
int led = LED_BUILTIN;
#endif

#ifdef ADAFRUIT_ITSY_RP2040_ITCF
int led = 11;
#endif

// "registers"
int S = S0; // data stack pointer
int R = R0; // return stack pointer
int I = 0; // instruction pointer
int W = 0; // working register

#define op_nop 1
#define op_delay 2
#define op_getch 3
#define op_push 4
#define op_pop 5
#define op_tib_init 6
#define op_branch 7
#define op_reflash 8

const int memory [] {

     op_nop, //
     op_delay, //
     op_getch, //
     op_push, //
     op_pop, //

     op_nop, //
     op_delay, //
     op_getch, //
     op_push, //
     op_pop, //

     op_nop, //
     op_delay, //
     op_getch, //
     op_push, //
     op_pop, //

     op_nop, //
     op_delay, //
     op_getch, //
     op_push, //
     op_pop, //

     op_nop, //
     op_delay, //
     op_getch, //
     op_push, //
     op_pop, //

     op_nop, //
     op_delay, //
     op_getch, //
     op_push, //
     op_pop, //

     op_nop, //
     op_delay, //
     op_getch, //
     op_push, //
     op_pop, //

     op_nop, //
     op_delay, //
     op_getch, //
     op_push, //
     op_pop, //

     op_tib_init, //

     op_reflash

     };

// https://github.com/CharleyShattuck/Feather-M0-interpreter/blob/master/Interpreter.ino

/* Terminal Input Buffer for interpreter */
const byte maxtib = 16;
char tib[maxtib];
byte pos = 0;

extern void reflash_firmware(void); // prototype

/* push n to top of data stack */
void push(int n) {
  p = (p + 1)& STKMASK;
  TOS = n;
}

/* return top of stack */
int pop() {
  int n = TOS;
  p = (p - 1)& STKMASK;
  return n;
}

void reflash(void) {
    reflash_firmware();
}

void await_serial(void) {
}

void pre_serial(void) {
    delay(700);
    while (!Serial) {
        await_serial();
    }
}

void nopp(void) { } // no operation

void print_newline(void) {
    Serial.write('\r');
    Serial.write('\n');
}

int faked = 0;
int L = 0; // place to pop to

void runForth () {
    char ch;
    int ESC_counter = 0;
    bool return_Flag = 0;
next:
    W = memory [I++];
    switch (W) {
        case op_nop:
        _nop:
            Serial.print(" op_nop");
            nopp();
            goto next;

        case op_delay: // xt:2
        _delay:
            Serial.print(" op_delay");
            delay(999);
            goto next;

        case op_getch:
        _getch:
            Serial.print(" op_getch");
            ch = '\000';
            if (Serial.available() > 0) ch = Serial.read();
            goto next;

        case op_push:
        _push:
            Serial.print(" op_push");
            push(++faked);
            goto next;

        case op_pop:
            Serial.print(" op_pop");
            L = pop();
        _pop:
            goto next;

        case op_tib_init:
        _tib_init:
            Serial.print(" op_tib_init");
            pos = 0; tib[0] = 0;
            goto next;

        case op_branch:
        _branch:
            Serial.print(" op_branch");
            I = memory [I];
            goto next;

        case op_reflash:
        _reflash:
            Serial.print(" op_reflash");
            Serial.print(" delay 12 seconds..");
            delay(12000);
            reflash();
            goto next;
    }
}

void setup () {
    Serial.begin (9600);
    pre_serial();
    I = 0;
    S = S0;
    R = R0;
    runForth (); // loop; not easily escaped
}

void loop () {
    while (1) {
        //    Serial.write ('A');
        //    delay (1000);
    };
}

// End
