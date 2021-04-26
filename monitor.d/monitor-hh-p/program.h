// program.h
// Mon Apr 26 01:06:59 UTC 2021
#define crlf(__a__); {Serial.print("\r\n");}

extern void reflash_firmware(void); // prototype
extern void dumpRAM(void); // dump_ram.cpp
extern void rdumps(void);
extern void words(void);
extern void setup_second_interpreter(void);
extern void secondary_Forth_loop(void);
