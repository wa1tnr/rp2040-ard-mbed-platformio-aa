// n_monitor-cc_SKETCH.cpp
// Thu Apr 22 00:41:04 UTC 2021

// was: n_monitor-bb_SKETCH.cpp
// was: forth-aa_SKETCH.cpp
// was: ITC-Forth.ino

/*
    from: https://github.com/CharleyShattuck/Steno-Keyboard-Arduino.git
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

const int memory [] {
     op_nop, //
     op_delay, //
     op_getch, //
     op_push, //
     op_pop, //
     op_tib_init, //
     op_branch, // branch
     0 }; // to address

// https://github.com/CharleyShattuck/Feather-M0-interpreter/blob/master/Interpreter.ino

/* Terminal Input Buffer for interpreter */
const byte maxtib = 16;
char tib[maxtib];
byte pos = 0;

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

void runForth () {
    char ch;
    int ESC_counter = 0;
    bool return_Flag = 0;
next:
    W = memory [I++];
    switch (W) {
        case op_nop:
        _nop:
            goto next;

        case op_delay: // xt:2
        _delay:
            delay(800);
            goto next;

        case op_getch:
        _getch:
            ch = '\000';
            if (Serial.available() > 0) ch = Serial.read();
            goto next;

        case op_push:
        _push:
            goto next;

        case op_pop:
        _pop:
            goto next;

        case op_tib_init:
        _tib_init:
            pos = 0; tib[0] = 0;
            goto next;

        case op_branch:
        _branch:
            I = memory [I];
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
