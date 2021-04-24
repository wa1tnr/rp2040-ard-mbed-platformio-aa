// n_monitor-ee_SKETCH.cpp
// #define REVISION_ITCF "0.1.0-g.7a - alpha kiyuta iii np: h.e. aa"
#define REVISION_ITCF "0.1.0-g.7b - alpha kiyuta iii np: tst aa"


// Sat Apr 24 20:20:20 UTC 2021


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

int J = 0; // counter

int RAMSPACE [RAM_SIZE];

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
int mem_size = 0; // for sizeof<foo>

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

#define op_exc       0xced1
#define op_lit       0x74696C + _nop_hxlg // lit: 6C l  69 i  74 t  0x74696C
#define op_nop       0x706F6E + _nop_hxlg // n: 6e o: 6f p: 70
#define op_rba       0x616272 + _nop_hxlg
#define op_rmb       0x626D72 + _nop_hxlg // hex char b . char m . char r . 62 6D 72  ok

#define op_rfl       0x6C6672 + _nop_hxlg
#define op_dly       0x796C64 + _nop_hxlg
#define op_getch 3
#define op_push 4
#define op_pop 5
#define op_tib_init 6
#define op_branch 7
#define op_one_plus 9
#define op_dts       0x737464 + _nop_hxlg
#define op_pon       0x6E6F70 + _nop_hxlg
#define op_pof       0x666F70 + _nop_hxlg
#define op_dump      0x706D64 + _nop_hxlg
#define op_emit      0x746D65 + _nop_hxlg
#define op_romptr 16

// #define op_hxee 238
// #define op_hxee 0x3EEEEEE // 238
// char e . char e . char x . char h . 65 65 78 68  ok
// #define op_hxee 0x657868EE
#define op_hxee 0x646E45EE

#define n1_sec  999
#define n4_sec 3999
#define n8_sec 7999
#define c_newline 10
#define c_return 13

const int memory [] {

     op_nop, op_nop, op_nop, op_nop,
     op_nop, op_nop, op_nop, op_nop,

     op_lit, c_newline, op_lit, c_return, op_emit, op_emit,
     op_dts,

     op_lit, c_newline, op_lit, c_return, op_emit, op_emit,
     op_rba,
     op_dts,

     /* dump */

     op_lit, c_newline, op_lit, c_return, op_emit, op_emit,
     op_dump,
     op_lit, c_newline, op_lit, c_return, op_emit, op_emit,
     op_dts,

     /* dump */

     op_lit, c_newline, op_lit, c_return, op_emit, op_emit,
     op_dump,
     op_lit, c_newline, op_lit, c_return, op_emit, op_emit,
     op_dts,

     /* dump */

     op_lit, c_newline, op_lit, c_return, op_emit, op_emit,
     op_dump,
     op_lit, c_newline, op_lit, c_return, op_emit, op_emit,
     op_dts,

     /* dump */

     op_lit, c_newline, op_lit, c_return, op_emit, op_emit,
     op_dump,
     op_lit, c_newline, op_lit, c_return, op_emit, op_emit,
     op_dts,

     /* dump */

     op_lit, c_newline, op_lit, c_return, op_emit, op_emit,
     op_dump,
     op_lit, c_newline, op_lit, c_return, op_emit, op_emit,
     op_dts,

     op_lit, c_newline, op_lit, c_return, op_emit, op_emit,
     op_lit, c_newline, op_lit, c_return, op_emit, op_emit,

     op_lit, 4000, op_dly,


     /* blink */
     op_lit, led, op_pon,  op_lit,  40, op_dly,
     op_lit, led, op_pof, op_lit, n1_sec, op_dly,

     op_lit, n1_sec, op_dly,

     op_nop,
     op_lit, c_newline, op_lit, c_return, op_emit, op_emit,
     op_lit, 4400, op_dly,
     op_exc, // throw an exception
     op_rfl,

     op_lit, 0, op_lit, 7, op_lit, 14, op_lit, 21, op_lit, 28, op_lit, 35, op_lit, 42, op_lit, 49,
     op_dts,

     op_lit, n8_sec, op_dly,

     op_lit, 5, op_lit, 10, op_lit, 15, op_lit, 20, op_lit, 25, op_lit, 30, op_lit, 35, op_lit, 40,

     op_dts,

     op_lit, n8_sec, op_dly,

     op_lit, 0, op_lit, 7, op_lit, 14, op_lit, 21, op_lit, 28, op_lit, 35, op_lit, 42, op_lit, 49,
     op_dts,

     op_lit, n8_sec, op_dly,

     op_lit,  3, op_lit,  6, op_lit,  9, op_lit, 12, op_lit, 15, op_lit, 18, op_lit, 21, op_lit, 24,

     op_dts,
     op_lit, n8_sec, op_dly,

     op_dts,
     op_lit, n4_sec, op_dly,
     op_dts,
     op_lit, n4_sec, op_dly,
     op_dts,
     op_lit, n4_sec, op_dly,
     op_dts,
     op_lit, n4_sec, op_dly,
     op_nop,

     op_dts, //

     op_tib_init, //

     op_dts,

     op_rfl,

     op_nop,
     op_nop, op_nop, op_nop, op_nop,
     op_nop, op_nop, op_nop, op_nop,
     op_nop, op_nop, op_nop, op_nop,
     op_nop, op_nop, op_nop, op_nop,
     op_hxee, op_hxee, op_hxee, op_hxee,
     op_hxee, op_hxee, op_hxee, op_hxee,
     op_hxee, op_hxee, op_hxee, op_hxee,
     op_hxee, op_hxee, op_hxee, op_hxee,
     op_hxee, op_hxee, op_hxee, op_hxee,
     op_hxee, op_hxee, op_hxee, op_hxee,
     op_hxee, op_hxee, op_hxee, op_hxee,
     op_hxee, op_hxee, op_hxee, op_hxee, // compiler lets you keep your comma

     };

// uint32_t mem_rom = *memory;
uint32_t mem_rom = (uint32_t) &memory;
uint32_t mem_ram = (uint32_t) &RAMSPACE;

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

/* copy ROM-like space into RAM-like space: */
void copy_over(void) {
    int mem_size = sizeof(memory) / 4;
    int words = sizeof(RAMSPACE) / 4; // 0x1200 array subscripts
    // Serial.print("sizeof(RAMSPACE) is: ");
    // Serial.println(words); delay(18000);

    // Serial.print("sizeof(memory) is: ");
    // Serial.println(mem_size);
    for (int i=0; i<words; i++) {
        RAMSPACE[i] = 1; // nop fill
    }

    for (int i=0; i<mem_size; i++) {
        RAMSPACE[i] = memory [i];
    }

}

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

void handle_exception(void) {
    Serial.println(" EXCEPTION encountered. delay 2 seconds..");
    delay(2000);
    Serial.println("reflash(); called now.");
    reflash();
}

#define program_boundary 2100

void runForth () {
    char ch;
    int ESC_counter = 0;
    bool return_Flag = 0;
next:
    if (I < 0) I = program_boundary + 8;
    if (I > program_boundary) I = program_boundary + 8;

    Serial.write(' '); Serial.write(' '); Serial.write('[');
    Serial.print(I);
    Serial.write(']'); Serial.write(' '); Serial.write(' ');

    delay(200);

    if (I > program_boundary) handle_exception();

    // W = memory [I++];
    W = RAMSPACE [I++];
    switch (W) {
        case op_exc:
        _exc:
            I = I + program_boundary + 8;
            goto next;
        case op_nop:
        _nop:
            Serial.print(" op_nop");
            nopp();
            goto next;

        case op_dly:
        _delay:
            Serial.print(" op_dly");
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

        case op_rfl:
        _reflash:
            Serial.print(" op_rfl");
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

        // case op_stack_report:
        case op_dts:
        _stack_report:
            print_stack_report();
            goto next;

        case op_lit:
        _lit:
            push(memory[I]);
            I++;
            goto next;

        // case op_gpio_on:
        case op_pon:
        _gpio_on:
//          Serial.print(" op_gpio_on");
            Serial.print(" op_pon");
            L = pop();
            digitalWrite(L, 1);
            goto next;

        // case op_gpio_off:
        case op_pof:
        _gpio_off:
        //  Serial.print(" op_gpio_off");
            Serial.print(" op_pof");
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

        case op_rba: // 'ram base' e.g. 'rba' ram base
        _ramptr:
            Serial.print(" op_rba");
            push(mem_ram);
            goto next;

        case op_rmb: // 'rom base' 'rmb' rom base
        _romptr:
            Serial.print(" op_rmb");
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
    copy_over(); // where?
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
