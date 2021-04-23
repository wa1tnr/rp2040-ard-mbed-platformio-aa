// n_monitor-ee_SKETCH.cpp
#define REVISION_ITCF "0.1.0-g.6j - alpha kiyuta ii np: lloc"

// ---------------------------------------
// LAST LOW OPCODES in recent development.
// ---------------------------------------

// Thu Apr 22 23:32:07 UTC 2021

// $ git branch
// * dvlp-aa-dump-a-

// was (most recent):
// #define REVISION_ITCF "0.1.0-g.6b - alpha kiyuta ii"
// Thu Apr 22 22:21:11 UTC 2021

// was: (or privately):
// #define REVISION_ITCF "0.1.0-g.6e - alpha kiyuta ii np: flagstone"
// was:
// Thu Apr 22 18:30:45 UTC 2021
// was:
// Thu Apr 22 18:02:54 UTC 2021

// $ git branch
// * dvlp-aa
// $ git checkout -b dvlp-aa-dump-a-
// Switched to a new branch 'dvlp-aa-dump-a-'
// $  git branch | egrep dvl
//  dvlp-aa
// * dvlp-aa-dump-a-

// was: n_monitor-cc_SKETCH.cpp
// Thu Apr 22 01:31:30 UTC 2021

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

/*
   dump
   from: https://github.com/wa1tnr/Metro-M4-Express-interpreter/blob/master/interpret_m4/interpret_m4.ino
*/
#include <Arduino.h>

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

#define _nop_hxpfx   0x111130
#define _nop_hxlg    0x03000000

#define op_rba       0x616272 + _nop_hxlg

#define op_nop 1
#define op_delay 2
#define op_getch 3
#define op_push 4
#define op_pop 5
#define op_tib_init 6
#define op_branch 7
#define op_reflash 8
#define op_one_plus 9
#define op_stack_report 10
#define op_lit 11
#define op_gpio_on 12
#define op_gpio_off 13
#define op_dump 14
#define op_emit 15
#define op_romptr 16

#define op_hxee 238

#define n1_sec  999
#define n4_sec 3999
#define n8_sec 7999
#define c_newline 10
#define c_return 13

const int memory [] {

     op_nop, op_nop, op_nop, op_nop,
     op_nop, op_nop, op_nop, op_nop,

     op_lit, c_newline, op_lit, c_return,

     op_rba,

     op_dump,
     op_lit, c_newline, op_lit, c_return,
     op_lit, c_newline, op_lit, c_return,

     op_stack_report,
     op_lit, c_newline, op_lit, c_return,
     op_lit, c_newline, op_lit, c_return,
     op_lit, 18000, op_delay,


     /* blink */
     op_lit, led, op_gpio_on,  op_lit,  40, op_delay,
     op_lit, led, op_gpio_off, op_lit, n1_sec, op_delay,

     op_lit, n1_sec, op_delay,

     op_nop, //

     op_lit, 0, op_lit, 7, op_lit, 14, op_lit, 21, op_lit, 28, op_lit, 35, op_lit, 42, op_lit, 49,
     op_stack_report, ///

     op_lit, n8_sec, op_delay,

     11, 5, 11, 10, 11, 15, 11, 20, 11, 25, 11, 30, 11, 35, 11, 40,

     op_stack_report, ///

     op_lit, n8_sec, op_delay,

     op_lit, 0, op_lit, 7, op_lit, 14, op_lit, 21, op_lit, 28, op_lit, 35, op_lit, 42, op_lit, 49,
     op_stack_report, //

     op_lit, n8_sec, op_delay,

     11, 3,
     11, 6,
     11, 9,
     11, 12,

     11, 15,
     11, 18,
     11, 21,
     11, 24,

     op_stack_report, //
     op_lit, n8_sec, op_delay,

     op_stack_report, //
     op_lit, n4_sec, op_delay,
     op_stack_report, //
     op_lit, n4_sec, op_delay,
     op_stack_report, //
     op_lit, n4_sec, op_delay,
     op_stack_report, //
     op_lit, n4_sec, op_delay,
     op_nop, //

     op_stack_report, //

     op_tib_init, //

     op_reflash

     };
/*
200012b8      1   0   0   0    1   0   0   0    1   0   0   0    1   0   0  0   ................
200012c8      1   0   0   0    1   0   0   0    1   0   0   0    1   0   0  0   ................
200012d8    [11]  0   0   0  [10]  0   0   0  [11]  0   0   0  [13]  0   0  0   ................
200012e8    [16]  0   0   0  [14]  0   0   0  [11]  0   0   0 [18,000]   0  0   ............PF..
200012f8      2   0   0   0  [11]  0   0   0  [11]  0   0   0  [12]  0   0  0   ................
20001308    [11]  0   0   0  [40]  0   0   0    2   0   0   0  [11]  0   0  0   ....(...........
20001318    [11]  0   0   0  [13]  0   0   0  [11]  0   0   0 [999]      0  0   ................
20001328      2   0   0   0  [11]  0   0   0 [999]      0   0    2   0   0  0   ................
20001338      1   0   0   0  [11]  0   0   0    0   0   0   0  [11]  0   0  0   ................
20001348      7   0   0   0  [11]  0   0   0  [14]  0   0   0  [11]  0   0  0   ................
20001358    [21]  0   0   0  [11]  0   0   0  [28]  0   0   0  [11]  0   0  0   ................
20001368    [35]  0   0   0  [11]  0   0   0  [42]  0   0   0  [11]  0   0  0   #.......*.......
20001378    [49]  0   0   0  [10]  0   0   0  [11]  0   0   0 [7,999]    0  0   1...........?...
20001388      2   0   0   0  [11]  0   0   0    5   0   0   0  [11]  0   0  0   ................
20001398    [10]  0   0   0  [11]  0   0   0  [15]  0   0   0  [11]  0   0  0   ................
200013a8    [20]  0   0   0  [11]  0   0   0  [25]  0   0   0  [11]  0   0  0   ................
*/

// uint32_t mem_rom = *memory;
uint32_t mem_rom = (uint32_t) &memory;

// https://github.com/CharleyShattuck/Feather-M0-interpreter/blob/master/Interpreter.ino

/* Terminal Input Buffer for interpreter */
const byte maxtib = 16;
char tib[maxtib];
byte pos = 0;

int faked = 0;
int L = 0; // place to pop into

extern void reflash_firmware(void); // prototype

extern void dumpRAM(void); // dump_ram.cpp
extern void rdumps(void);

/* push n to top of data stack */
void push(int n) {
  p = (p + 1)& STKMASK;
  TOS = n;
}

/* return top of stack */
int pop(void) {
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
    pinMode(led, OUTPUT);
    digitalWrite(led, 1);
    delay(700);
    digitalWrite(led, 0);
    delay(700);
    while (!Serial) {
        await_serial();
    }
    Serial.println(REVISION_ITCF);
    delay(3000);
}

void nopp(void) { } // no operation

void print_newline(void) {
    Serial.write('\r');
    Serial.write('\n');
}

void print_stack_report(void) {
    Serial.println();
    // Serial.println(" <4> 1 2 3 4 :: Reads left to right, old to new: ");
    for (int k = 0; k < (STKSIZE + 1); k++) L = pop();
    for (int i = 0; i < STKSIZE; i++) {
        for (int j = 0; j < ((STKSIZE - 1)) ; j++) {
            L = pop();
        }
        Serial.print(L);
        Serial.print(' ');
    }
    for (int l = 0; l < (STKSIZE - 1); l++) L = pop();
    Serial.println();
    // delay(4000);
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
            Serial.print(" op_nop");
            nopp();
            goto next;

        case op_delay:
        _delay:
            Serial.print(" op_delay");
            L = pop();
            delay(L);
            goto next;

        case op_getch:
        _getch:
            Serial.print(" op_getch");
            ch = '\000';
            if (Serial.available() > 0) ch = Serial.read();
            goto next;

        case op_push: // forbidden idiom - maybe - use LIT instead
        _push:
            Serial.print(" op_push");
            push(++faked);
            goto next;

        case op_pop:
        _pop:
            Serial.print(" op_pop");
            L = pop();
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
            Serial.print(" delay 2 seconds..");
            delay(2000);
            reflash();
            goto next;

        case op_one_plus:
        _one_plus:
            Serial.print(" op_one_plus");
            L = pop();
            L++;
            push(L);
            goto next;

        case op_stack_report:
        _stack_report:
            // Serial.println();
            print_stack_report();
            goto next;

        case op_lit:
        _lit:
            push(memory[I]);
            I++;
            goto next;

        case op_gpio_on:
        _gpio_on:
            Serial.print(" op_gpio_on");
            L = pop();
            digitalWrite(L, 1);
            goto next;

        case op_gpio_off:
        _gpio_off:
            Serial.print(" op_gpio_off");
            L = pop();
            digitalWrite(L, 0);
            goto next;

        case op_dump:
        _dump:
            Serial.print(" op_dump");
            rdumps();
            goto next;
        case op_emit:

        _emit:
            // Serial.print(" op_emit");
            Serial.write((pop()));
            goto next;

        case op_rba: // 'r base' aka 'rom base' e.g. 'rba' rom base
        _romptr:
            Serial.print(" op_rba");
            push(mem_rom);
            goto next;

        case op_hxee:
        _hxee:
            // nothing - is a marker instruction only
            goto next;
    }
}

void setup () {
    Serial.begin (9600);
    pre_serial();
    I = 0;
    S = S0;
    R = R0;
    for (int i = 0; i < STKSIZE; i++) push((13 * i));
    runForth (); // loop; not easily escaped
}

void loop () {
    while (1) {
        //    Serial.write ('A');
        //    delay (1000);
    };
}

// End
