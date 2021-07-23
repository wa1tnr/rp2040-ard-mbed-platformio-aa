// mci-simpl-bb.ino

// first instance without repeat counts from either core - 03:59z 23 Jul 2021

#define newline(x) Serial.println(x)

void pdel(void) {
    for (volatile uint64_t waste_cycles = 0x3E12;
        waste_cycles >0; waste_cycles--) {
            for (volatile int i = 77; i>0; i--) { } // slower nop
    }
}

void jdel(void) {
    for (volatile uint64_t waste_cycles = 0x3E123;
        waste_cycles >0; waste_cycles--) {
            for (volatile int i = 77; i>0; i--) { } // slower nop
    }
}


void rdel(void) { // smaller than sdel
    for (volatile uint64_t waste_cycles = 0x3E;
        waste_cycles >0; waste_cycles--) {
            for (volatile int i = 77; i>0; i--) { }
    }
}

/*
void sdel(void) {
    for (volatile uint64_t waste_cycles = 0x3E1234;
        waste_cycles >0; waste_cycles--) {
            for (volatile int i = 77; i>0; i--) { }
    }
}
*/

void setup() {
  delay(4000);
  Serial.begin(9600);
  delay(1000);
  newline();
  Serial.println("On 64-bit boundary");
  newline();
  Serial.print("THIS ONE begins here: ");
  Serial.println("Fri Jul 23 04:06:11 UTC 2021\r\n");
  delay(1000);
}

void setup1() {
  delay (600);
}

uint8_t halting = 3; // is only 8 bits so it wraps around

void loop() {
  halting++;
  uint64_t a = rp2040.getCycleCount64();
  rdel();
  uint64_t b = rp2040.getCycleCount64();
  Serial.print("Core _ cycles: ");
  Serial.println(b - a);
  jdel();
}


void loop1() {
  halting++;
  for (volatile uint64_t rwaste = 0x7FFFF ; rwaste > 0; rwaste--) { }
  if ((halting == 77) || (halting == 78) ) {
      newline('7');
      newline('7');
      newline('7');
      newline('7');
      Serial.println();
      Serial.println();
      Serial.println("     GOT IT     ");
      Serial.println();
      Serial.println();
      delay(19999);
      Serial.println();
      Serial.println();
      Serial.println("             so now what              ");
      Serial.println();
      Serial.println();
      delay(19999);
  }
  rp2040.idleOtherCore();
  uint64_t c = rp2040.getCycleCount64();
  rp2040.resumeOtherCore();
  for (volatile int waste = 0x7FEFF ; waste > 0; waste--) { }
  rp2040.idleOtherCore();
  uint64_t d = rp2040.getCycleCount64();
  rp2040.resumeOtherCore();
  Serial.print("Core 1 cycles:                ");
  Serial.println(d - c);
  rdel();
  rdel();
  rdel();
  rdel();
  rdel();
  rdel();
  rdel();
  rdel();
  rdel();
  rdel();
  rdel();
  rdel();
  rdel();
  rdel();
  rdel();
  rdel();
}

// END.
