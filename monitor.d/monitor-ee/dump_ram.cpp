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
  sprintf(buffer, "%4X", p);
  Serial.print(buffer);
  Serial.print(":");
  for (int i = 0; i < 4; i++) {
  for (int j = 0; j < 4; j++) {

    char c = *ram++;
    if (c == 0) {
        sprintf(buffer, " %s", "00");
    }
    if (c != 0) {
        sprintf(buffer, " %2X", (c & 0xff));
    }
    Serial.print(buffer);

    }
  Serial.write(' ');
// 200012B8: 39 11 11 00  39 11 11 00  39 11 11 00  39 11 11 00  9...9...9...9...
// 200012B8: 39 11 11 00  39 11 11 00  39 11 11 00  39 11 11 00  9...9...9...9...
// 12345678901234567890123456789012345678901234567890123456789012345678901234567890

  }


  ram = (char*)p;
  Serial.print(" ");
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

