// n_monitor-aa_SKETCH.cpp
// Tue Apr 20 21:05:08 UTC 2021

// was: forth-aa_SKETCH.cpp
// was: ITC-Forth.ino

/*
    from: https://github.com/CharleyShattuck/Steno-Keyboard-Arduino.git
*/

#include <Arduino.h>
#define REVISION_ITCF "0.1.0-e.4 chevron"

#undef ADAFRUIT_ITSY_RP2040_ITCF
#define ADAFRUIT_ITSY_RP2040_ITCF

#ifndef ADAFRUIT_ITSY_RP2040_ITCF
    #define UNKNOWN_TARGET_RP2040_ITCF
#endif

#define SLOW_WAIT_AA 125

#define RAM_SIZE 0x1200
#define S0 0x1000
#define R0 0x0f00

extern void reflash_firmware(void); // prototype

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

int reflash_timeout = 0xCFFF; // a good six minutes here 0xCFFF

// TODO: label these better.  There are 7 instrux, but labels are out of date.
/*
const int memory [] {
    1, // nop - was delay
    2, // read serial
    3, // escape detection
    4, // branch
    0, // to this address
};
*/

// ^^ so this memory is going to be how/where a 'program' is written.

// const int memory [] { 7, 1, 2, 3, 4, 5, 6, 7, 8, 1 }; // is similar to the program already written.
   const int memory [] { 7, 1, 2, 3, 4, 5, 6, 7, 8, 1 }; // is similar to the program already written.

// https://github.com/CharleyShattuck/Feather-M0-interpreter/blob/master/Interpreter.ino

/* Terminal Input Buffer for interpreter */
const byte maxtib = 16;
char tib[maxtib];
byte pos = 0;

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
    delay(700);
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

void runForth () {
    char ch;
    int ESC_counter = 0;
    bool return_Flag = 0;
next:
    W = memory [I++];
    switch (W) {
        case 1:
        _delay:
            Serial.print("  Seen branch 1"); delay(20);
            delay (500);
            goto next;
        case 2: // xt:2
        _read_serial:
            Serial.print("  Seen branch 2"); delay(20);
            ch = '\000';
            if (Serial.available() > 0) ch = Serial.read();
            goto next;
        case 3:
        _esc_det:
            Serial.print("  Seen branch 3"); delay(20);
            if (ch != '\033') { if (ch != '\000') { ESC_counter = 0; } }
            if (ch == '\033') {
                ch = '\000';
                I--; I--; // relative jump back to xt:3
                ESC_counter++;
                if (ESC_counter == 3) return_Flag = -1;
            }
            goto next;
        case 4:
        _do_this_aa:
            Serial.print("  Seen branch 4"); delay(20);
            goto next;
        case 5:
        _fill_next_tib_byte:
            Serial.print("  Seen branch 5"); delay(20);
            if ((ch > 31) && (ch < 127)) {
                Serial.write(ch);
                // DEBUG: Serial.write('\055');
                if (pos < (maxtib -1)) {
                    tib[pos++] = ch;
                    tib[pos] = '\000';
                    // DEBUG: Serial.write('\056');
                }
            }
            if ((ch == '\012')) { // 10 decimal 0x0A
                Serial.write('\040'); // space
                Serial.print(tib);
                Serial.print(" where TIB was printed. ");
                I++;
                // I = 7; // hum
                // Serial.print("Sent to I = 6");
                // pos = 0; tib[pos] = '\000';
                print_newline();
            }
            if (ch == '\010') { // backspace
                Serial.write('\010');
            }
            goto next;

        case 6:
        branch:
            // Serial.print("encounter branch case 6");
            I++;
            Serial.print("  Seen branch 6"); delay(500);
            if (return_Flag) {
                Serial.print(tib); Serial.print(" "); Serial.println(" ");
                Serial.println("That's all folks");
                return;
            }
            goto next;

        case 7:
        initializer:
            // I = memory [I];
            Serial.print("  Seen branch 7"); delay(500);
            // Serial.print("the great initializer on 7");
            pos = 0; tib[0] = 0;
            goto next;
        case 8:
        branch_b:
            I = memory [I];
            Serial.print("  Seen branch 8"); delay(500);
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
