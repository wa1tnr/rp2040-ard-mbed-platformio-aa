// x

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

/*
void rdel(void) { // smaller than sdel
    for (volatile uint64_t waste_cycles = 0x3E;
        waste_cycles >0; waste_cycles--) {
            for (volatile int i = 77; i>0; i--) { }
    }
}

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
  Serial.print("THIS ONE begins here: ");
  Serial.println("Fri Jul 23 03:09:47 UTC 2021\r\n");
  delay(1000);
}

void setup1() {
  delay (6000);
}

void loop() {
  jdel();
  uint32_t a = rp2040.getCycleCount();
  jdel();
  uint32_t b = rp2040.getCycleCount();
  Serial.print("Core 1 cycles: ");
  Serial.println(b - a);
  jdel();
}

// loop1 seems determined to issue getCycleCount() returns in standard chunks
void loop1() {
  jdel();
  uint32_t c = rp2040.getCycleCount();
  pdel();
  // jdel();
  uint32_t d = rp2040.getCycleCount();
  Serial.print("Core 2 cycles:                ");

  if ( d - c < 7777777 ) { Serial.println("Deresolution event. "); }

  Serial.println(d - c);
  jdel();
}

// END.
