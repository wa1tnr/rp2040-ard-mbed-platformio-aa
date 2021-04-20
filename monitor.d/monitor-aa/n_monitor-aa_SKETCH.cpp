// n_monitor-aa_SKETCH.cpp
// Mon Apr 19 21:29:48 UTC 2021

// was: forth-aa_SKETCH.cpp
// was: ITC-Forth.ino

/*
    from: https://github.com/CharleyShattuck/Steno-Keyboard-Arduino.git
*/

#include <Arduino.h>
#define REVISION_ITCF "0.1.0-d.0"
#define SLOW_WAIT_AA 125
#define DEBUG_NOP_PRINTS
#undef DEBUG_NOP_PRINTS
#define DEBUG_PRINTS
#undef DEBUG_PRINTS

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

int reflash_timeout = 611; // seconds

const int memory [] {
    1, // print A
    2, // delay 1 sec
    3, // do something new
    4, // nop a
    5, // nop b
    6, // nop c
    7, // branch
    0, // to this address
};

void full_blank(void) {
    digitalWrite(led, 0);
}

void blink (void) {
    digitalWrite(led, 1);
    delay(55);
    full_blank();
}

void fast_blank(void) {
    int counter = SLOW_WAIT_AA;
    do {
        if (Serial) return;
        delay(50);
        counter--;
    } while (counter > 0);
    full_blank();
}

void await_serial(void) {
    while (!Serial) {
        blink ();
        if (!Serial) fast_blank();
        full_blank();
    }
    Serial.print("Press ESC to initiate a reflash.   ");
    Serial.print("Rev. ");
    Serial.println(REVISION_ITCF);
}

void pre_serial(void) {
    while (!Serial) {
        await_serial();
    }
}

void hardware_setup(void) {
    pinMode(led, OUTPUT);
}

void reflash(void) {
    reflash_firmware();
}

void nopp(void) { } // no operation

void runForth () {
    char ch;
    // = '\000';
next:
    W = memory [I++];
    switch (W) {
        case 1:
A:
            reflash_timeout--;
#ifdef DEBUG_PRINTS
            Serial.write ('.'); // 'A'
#endif
            goto next;
        case 2:
_delay:
            delay (1000);
            goto next;
        case 3:
_sm_new:
            delay(7);
            ch = '\000';
            if (Serial.available() > 0) ch = Serial.read();
            // if ((ch > 31) && (ch < 127)) Serial.write(ch);
#ifdef DEBUG_PRINTS
            Serial.write('.'); // 'B'
#endif
            goto next;

        case 4:
_nop_a:
            nopp();
#ifdef DEBUG_NOP_PRINTS
            Serial.print("  NOP-A  ");
#endif
            goto next;

        case 5:
_nop_b:
            nopp();
#ifdef DEBUG_NOP_PRINTS
            Serial.print("  NOP-B  ");
#endif
            goto next;

        case 6:
_nop_c:
            nopp();
#ifdef DEBUG_NOP_PRINTS
            Serial.print("  NOP-C  ");
#endif
            goto next;

        case 7:
branch:
            I = memory [I];
            if ((reflash_timeout == 0)
                    || (ch == '\033')
               ) return; // ESC
            if ((ch > 31) && (ch < 127)) Serial.write(ch);
            if ((ch == '\012')    // 10 decimal 0x0A
                    // || (ch == '\012') // 10 decimal 0x0A
               ) {
                Serial.write('\r');
                Serial.write('\n');
            }
            goto next;
    }
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
