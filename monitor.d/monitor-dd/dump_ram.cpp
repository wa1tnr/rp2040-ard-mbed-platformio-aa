/* dump_ram.cpp */
// #define REVISION_ITCF "0.1.0-g.6 - alpha kiyuta ii"
// Thu Apr 22 18:02:54 UTC 2021
#include <Arduino.h>

/*
  from: https://github.com/wa1tnr/Metro-M4-Express-interpreter
  from: https://github.com/CharleyShattuck/Feather-M0-interpreter
*/

extern void push(int n);
extern int pop(void);

/* dump 16 bytes of RAM in hex with ascii on the side */

void dumpRAM(void) {
  char buffer[5] = "";
  char *ram;
  int p = pop();
  ram = (char*)p;
  sprintf(buffer, "%4x", p);
  Serial.print(buffer);
  Serial.print("   ");
  for (int i = 0; i < 16; i++) {
    char c = *ram++;
    sprintf(buffer, " %2x", (c & 0xff));
    Serial.print(buffer);
  }
  ram = (char*)p;
  Serial.print("   ");
  for (int i = 0; i < 16; i++) {
    buffer[0] = *ram++;
    if (buffer[0] > 0x7f || buffer[0] < ' ') buffer[0] = '.';
    buffer[1] = '\0';
    Serial.print(buffer);
  }
  push(p + 16);
}

/* dump 256 bytes of RAM */
void rdumps(void) {
  for (int i = 0; i < 16; i++) {
    Serial.println();
    dumpRAM();
  }
}

