#include <Arduino.h>
#include "program.h"
/* Tiny interpreter,
   similar to myforth's Standalone Interpreter
   This example code is in the public domain */

/* Structure of a dictionary entry */
typedef struct {
  const char *name;
  void (*function)();
} entry;

/* Data stack for parameter passing
   This "stack" is circular,
   like a Green Arrays F18A data stack,
   so overflow and underflow are not possible
   Number of items must be a power of 2 */
const int STKSIZE_B = 8;
const int STKMASK_B = 7;
int stack_B[STKSIZE_B];
int p_B = 0;

/* TOS is Top Of Stack */
#define TOS stack_B[p_B]
/* NAMED creates a string in flash */
#define NAMED(x, y) const char x[]=y

/* Terminal Input Buffer for interpreter */
const byte maxtib_B = 16;
char tib_B[maxtib_B];
/* buffer required for strings read from flash */
char namebuf[maxtib_B];
byte pos_B;

/* push n to top of data stack */
void push_B(int n) {
  p_B = (p_B + 1)& STKMASK_B;
  TOS = n;
}

/* return top of stack */
int pop_B() {
  int n = TOS;
  p_B = (p_B - 1)& STKMASK_B;
  return n;
}

/* Global delay timer */
int spd = 15;

/* top of stack becomes current spd */
NAMED(_speed, "speed");
void speed() {
  spd = pop_B();
}

/* discard top of stack */
NAMED(_drop, "drop");
void drop() {
  pop_B();
}

/* recover dropped stack item */
NAMED(_back, "back");
void back() {
  for (int i = 1; i < STKSIZE_B; i++) drop();
}

/* copy top of stack */
NAMED(_dup, "dup");
void dup() {
  push_B(TOS);
}

/* exchange top two stack items */
NAMED(_swap, "swap");
void swap() {
  int a;
  int b;
  a = pop_B();
  b = pop_B();
  push_B(a);
  push_B(b);
}

/* copy second on stack to top */
NAMED(_over, "over");
void over() {
  int a;
  int b;
  a = pop_B();
  b = pop_B();
  push_B(b);
  push_B(a);
  push_B(b);
}
/* add top two items */
NAMED(_add, "+");
void add() {
  int a = pop_B();
  TOS = a + TOS;
}

/* bitwise and top two items */
NAMED(_and, "and");
void and_() {
  int a = pop_B();
  TOS = a & TOS;
}

/* inclusive or top two items */
NAMED(_or, "or");
void or_() {
  int a = pop_B();
  TOS = a | TOS;
}

/* exclusive or top two items */
NAMED(_xor, "xor");
void xor_() {
  int a = pop_B();
  TOS = a ^ TOS;
}

/* invert all bits in top of stack */
NAMED(_invert, "invert");
void invert() {
  TOS = ~(TOS);
}

/* negate top of stack */
NAMED(_negate, "negate");
void negate() {
  TOS = -(TOS);
}

/* destructively display top of stack, decimal */
NAMED(_dot, ".");
void dot() {
  Serial.print(pop_B());
  Serial.print(" ");
}

/* destructively display top of stack, hex */
NAMED(_dotHEX, ".h");
void dotHEX() {
  Serial.print(0xffff & pop_B(), HEX);
  Serial.print(" ");
}

/* display whole stack, hex */
NAMED(_dotShex, ".sh");
void dotShex() {
  for (int i = 0; i < STKSIZE_B; i++) dotHEX();
}

/* display whole stack, decimal */
NAMED(_dotS, ".s");
void dotS() {
  for (int i = 0; i < STKSIZE_B; i++) dot();
}

/* delay TOS # of milliseconds */
NAMED(_delay, "delay");
void del() {
  delay(pop_B());
}

/* Toggle pin at TOS and delay(spd), repeat... */
NAMED(_wiggle, "wiggle");
void wiggle() {
  int a = pop_B();
  pinMode(a, OUTPUT);
  for (int i = 0; i < 20; i++) {
    digitalWrite(a, HIGH);
    delay(100);
    digitalWrite(a, LOW);
    delay(100);
  }
}

/* TOS is pin number, set it HIGH */
NAMED(_high, "high");
void high() {
  digitalWrite(pop_B(), HIGH);
}

/* set TOS pin LOW */
NAMED(_low, "low");
void low() {
  digitalWrite(pop_B(), LOW);
}

/* read TOS pin */
NAMED(_in, "in");
void in() {
  TOS = digitalRead(TOS);
}

/* make TOS pin an input */
NAMED(_input, "input");
void input() {
  pinMode(pop_B(), INPUT);
}

/* make TOS pin an output */
NAMED(_output, "output");
void output() {
  pinMode(pop_B(), OUTPUT);
}

/* make TOS pin an input with weak pullup */
NAMED(_input_pullup, "input_pullup");
void input_pullup() {
  pinMode(pop_B(), INPUT_PULLUP);
}

/* dump 16 bytes of RAM in hex with ascii on the side */
void dumpRAM_B() {
  char buffer[5] = "";
  char *ram;
  int p_B = pop_B();
  ram = (char*)p_B;
  sprintf(buffer, "%4x", p_B);
  Serial.print(buffer);
  Serial.print("   ");
  for (int i = 0; i < 16; i++) {
    char c = *ram++;
    sprintf(buffer, " %2x", (c & 0xff));
    Serial.print(buffer);
  }
  ram = (char*)p_B;
  Serial.print("   ");
  for (int i = 0; i < 16; i++) {
    buffer[0] = *ram++;
    if (buffer[0] > 0x7f || buffer[0] < ' ') buffer[0] = '.';
    buffer[1] = '\0';
    Serial.print(buffer);
  }
  push_B(p_B + 16);
}

/* dump 256 bytes of RAM */
NAMED(_dumpr, "dump");
void rdumps_B() {
  for (int i = 0; i < 16; i++) {
    Serial.println();
    dumpRAM_B();
  }
}

boolean quit_flag = 0;

NAMED(_quit, "quit");
void quit() {
    quit_flag = -1; // true
}

/* End of Forth interpreter words */
/* ******************************************** */
/* Beginning of application words */




/* End of application words */
/* ******************************************** */
/* Now build the dictionary */

/* empty words don't cause an error */
NAMED(_nop, " ");
void nop() { }

/* Forward declaration required here */
NAMED(_words, "words");
void words(void);

/* table of names and function addresses in flash */
const entry dictionary[] = {
  {_nop, nop},
  {_words, words},
  {_quit, quit},
  {_dup, dup},
  {_drop, drop},
  {_back, back},
  {_swap, swap},
  {_over, over},
  {_add, add},
  {_and, and_},
  {_or, or_},
  {_xor, xor_},
  {_invert, invert},
  {_negate, negate},
  {_dotS, dotS},
  {_dotShex, dotShex},
  {_dot, dot},
  {_dotHEX, dotHEX},
  {_delay, del},
  {_high, high},
  {_low, low},
  {_in, in},
  {_input, input},
  {_output, output},
  {_input_pullup, input_pullup},
  {_wiggle, wiggle},
  {_dumpr, rdumps_B},
  {_speed, speed}
};

/* Number of words in the dictionary */
const int entries = sizeof dictionary / sizeof dictionary[0];

/* Display all words in dictionary */
void words(void) {
  for (int i = entries - 1; i >= 0; i--) {
    strcpy(namebuf, dictionary[i].name);
    Serial.print(namebuf);
    Serial.print(" ");
  }
}

/* Find a word in the dictionary, returning its position */
int locate() {
  // Serial.println("DEBUG: locate() was called.");
  // Serial.print(" Number of entries is: ");
  // Serial.println(entries);
  // delay(3000);
  for (int i = (entries - 1); i >= 0; i--) { // 25 april was not entries -1 just entries
    // Serial.println("DEBUG inside loop locate");
    strcpy(namebuf, dictionary[i].name);
    // if (!strcmp(tib_B, namebuf)) return i;
    if (!strcmp(tib_B, namebuf)) return i; // index 'i' sketchy 25 April 2021
  }
  return 0;
}

/* Is the word in tib_B a number? */
int isNumber() {
  char *endptr;
  strtol(tib_B, &endptr, 0);
  if (endptr == tib_B) return 0;
  if (*endptr != '\0') return 0;
  return 1;
}

/* Convert number in tib_B */
int number() {
  char *endptr;
  return (int) strtol(tib_B, &endptr, 0);
}

char ch;

void ok() {
  if (ch == '\r') Serial.println("ok");
}

/* Incrementally read command line from serial port */
byte reading() {
  if (!Serial.available()) return 1;
  ch = Serial.read();
  Serial.write(ch); // KLUDGE 25 April 2021
  if (ch == '\n') return 0; // terminal pref 25 apr 2021
  // if (ch == '\r') return 0;
  // if (ch == '\r') return 1; // not terminal pref 25 apr 2021
  if (ch == ' ') return 0;
  if (pos_B < maxtib_B) {
    tib_B[pos_B++] = ch;
    tib_B[pos_B] = 0;
  }
  return 1;
}

/* Block on reading the command line from serial port */
/* then echo each word */
void readword() {
  pos_B = 0;
  tib_B[0] = 0;
  while (reading());
  Serial.print('\000'); // Serial.print(tib_B); // KLUDGE 25 April 2021 - was enabled in upstream
  Serial.print(" ");
}

/* Run a word via its name */
void runword() {
  int place = locate();
  if (place != 0) {
    dictionary[place].function();
    // Serial.println("DEBUG 25 apr: dictionary thing in 'runword' happened already.");
    ok();
    return;
  }
  if (isNumber()) {
    push_B(number());
    ok();
    return;
  }
  Serial.println("?");
}

/* Arduino main loop */

void blink_three(void) {
  for (int i=3;i>0;i--) {
      digitalWrite(13, 1); delay(500); digitalWrite(13, 0); delay(900);
  }
}

void setup_second_interpreter(void) {
  pinMode(13, 1); // Feather RP2040 LED
  blink_three();

  // Serial.begin(9600);
  // while (!Serial);
  // Serial.println ("Forth-like interpreter:");
  // words();
  // Serial.println();
}

byte run_secondForth(void) {
  readword();
  runword();
  if (quit_flag) {
      Serial.println(" SEEN: quit_flag = -1");
      quit_flag = 0; // reset it
      return 0; // exit secondary loop
  }
  return 1;
}

void secondary_Forth_loop(void) {
    while(run_secondForth());
    Serial.println("QUIT detected!");
    Serial.println();
    Serial.println("RESTARTing.");
}

// Sun Apr 25 17:23:54 UTC 2021


// extern void setup_second_interpreter(void);
// extern void secondary_Forth_loop(void);



