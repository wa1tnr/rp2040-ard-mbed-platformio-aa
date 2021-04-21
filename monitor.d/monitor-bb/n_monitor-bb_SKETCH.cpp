// n_monitor-bb_SKETCH.cpp
// Wed Apr 21 03:03:52 UTC 2021

// was: n_monitor-aa_SKETCH.cpp
// Wed Apr 21 01:31:22 UTC 2021

// was: forth-aa_SKETCH.cpp
// was: ITC-Forth.ino

/*
    from: https://github.com/CharleyShattuck/Steno-Keyboard-Arduino.git
*/

#include <Arduino.h>
#define REVISION_ITCF "0.1.0-f.3 color - alpha"

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

/*
const int memory [] {
    1, // delay
    2, // read serial
    3, // escape detection
    4, // fill tib
    5, // exit handler
    6, // re-init tib
    7, // branch
    1, // to this address
};
*/

// 9 nocolor  8 green
// GOOD: const int memory [] { 6, 1, 2, 3, 4, 5, 6, 7, 1 };

// try 8, 4  subst for just the 4.

// so 8 modifies 4 .. 8, 4 is valid where 4 is an opcode not an address ;)

// try: 9, 5 to subst for just 5

// 9, 5 did kind of work but the buffer wasn't updating anymore.
// it'd keep its first reported value.

         const int memory [] { 9, 6, 7, 4, 2, 3, 8, 4, 5, 6, 7, 4 };

// addresses                 { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, a }

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

void nocolor(void) {  // terminal default
    Serial.write('\033'); // ESC
    Serial.write('[');
    Serial.write('0');
    Serial.write('m');
}



void green(void) {
    Serial.write('\033'); // ESC
    Serial.write('[');
    Serial.write('0');
    Serial.write(';');
    Serial.write('1');
    Serial.write(';');
    Serial.write('3');
    Serial.write('2');
    Serial.write(';');
    Serial.write('4');
    Serial.write('0');
    Serial.write('m');
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
            // delay(800);
            goto next;
        case 2: // xt:2
        _read_serial:
            ch = '\000';
            if (Serial.available() > 0) ch = Serial.read();
            goto next;
        case 3:
        _esc_det:
            if (ch != '\033') { if (ch != '\000') { ESC_counter = 0; } }
            if (ch == '\033') {
                ch = '\000';
                I--; I--; // point to 2nd previous instruction, xt:2 read_serial
                ESC_counter++;
                if (ESC_counter == 3) return_Flag = -1;
            }
            goto next;
        case 4:
        _fill_next_tib_byte:
            if ((ch > 31) && (ch < 127)) {
                Serial.write(ch);
                if (pos < (maxtib -1)) {
                    tib[pos++] = ch;
                    tib[pos] = '\000';
                }
            }
            if ((ch == '\012')) { // 10 decimal 0x0A
                Serial.write('\040'); // space
                Serial.print(tib);
                I++; // point to tib_initializer
                print_newline();
            }
            if (ch == '\010') { // backspace
                Serial.write('\010');
            }
            goto next;
        case 5:
        reflash_on_exit:
            I++; // skip over tib_initializer
            if (return_Flag) {
                // Serial.print(tib);
                nocolor(); // kludge don't know what else to do.
                Serial.println("That's all folks");
                return;
            }
            goto next;
        case 6:
        tib_initializer:
            pos = 0; tib[0] = 0;
            goto next;
        case 7:
        branch:
            I = memory [I];
            goto next;
        case 8:
        green:
            green();
            goto next;
        case 9:
        nocolor:
            nocolor();
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
