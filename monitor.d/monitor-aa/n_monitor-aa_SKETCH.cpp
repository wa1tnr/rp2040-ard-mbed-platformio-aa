// n_monitor-aa_SKETCH.cpp
// Mon Apr 19 21:29:48 UTC 2021

// was: forth-aa_SKETCH.cpp
// was: ITC-Forth.ino

/*
    from: https://github.com/CharleyShattuck/Steno-Keyboard-Arduino.git
*/

#include <Arduino.h>
#define REVISION_ITCF "0.1.0-d.6"
#define SLOW_WAIT_AA 125

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

int reflash_timeout = 0xCFFF; // a good six minutes here 0xCFFF

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
    Serial.print("Press ESC (3x) to initiate a reflash.   ");
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

void print_newline(void) {
    Serial.write('\r');
    Serial.write('\n');
}

// new:

void runForth () {
    char ch;
    int ESC_counter;
    bool return_Flag = 0;
next:
    W = memory [I++];
    switch (W) {
        case 1:
        A:
            goto next;
        case 2:
        _delay:
            // delay (1000);
            goto next;
        case 3:
        _sm_new:
            ch = '\000';
            if (Serial.available() > 0) ch = Serial.read();
            goto next;
        case 4:
        _nop_a:
            if (ch != '\033') { if (ch != '\000') { ESC_counter = 0; } }
            if (ch == '\033') {
                ch = '\000';
                I--; I--;
                ESC_counter++;
                if (ESC_counter == 3) return_Flag = -1;
            }
            goto next;
        case 5:
        _nop_b:
            goto next;
        case 6:
        _nop_c:
            goto next;
        case 7:
        branch:
            I = memory [I];
            if (return_Flag) return;
            if ((ch > 31) && (ch < 127)) {
                Serial.write(ch);
            }
            if ((ch == '\012')) { // 10 decimal 0x0A
                print_newline();
            }
            if (ch == '\010') { // backspace
                Serial.write('\010');
            }
            goto next;
    }
}

// old:
void xxrunForth () {
    char ch;
    // = '\000';
next:
    W = memory [I++];
    switch (W) {
        case 1:
        A:
            // reflash_timeout--; // intent is to rescind the timeout
            goto next;
        case 2:
        _delay:
            // delay (1000);
            nopp();
            goto next;
        case 3:
        _sm_new:
            delay(7);
            ch = '\000';
            if (Serial.available() > 0) ch = Serial.read();
            // if ((ch > 31) && (ch < 127)) Serial.write(ch);
            goto next;

        case 4:
        _nop_a:
            nopp();
            goto next;

        case 5:
        _nop_b:
            nopp();
            goto next;

        case 6:
        _nop_c:
            nopp();
            goto next;

        case 7:
        branch:
            I = memory [I];
            if ((reflash_timeout == 0)
                    || (ch == '\033')
               ) return; // ESC
            if ((ch > 31) && (ch < 127)) Serial.write(ch);
            if (ch == '\012') print_newline(); // 10 decimal 0x0A
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
