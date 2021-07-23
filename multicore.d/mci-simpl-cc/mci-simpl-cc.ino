// mci-simpl-cc.ino

// Fri Jul 23 07:25:25 UTC 2021

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
            for (volatile int i = 77; i>0; i--) { }
    }
}


void rdel(void) {
    for (volatile uint64_t waste_cycles = 0x3E;
        waste_cycles >0; waste_cycles--) {
            for (volatile int i = 77; i>0; i--) { }
    }
}

bool led_state;

void toggle_led(void) {
    led_state = !led_state;
    digitalWrite(LED_BUILTIN, led_state);
}

void setup() {
  delay(3000);
  pinMode(LED_BUILTIN, 1); // output
  digitalWrite(LED_BUILTIN, 1); // ON
  Serial.begin(115200);
  delay(1000);
  newline();
  Serial.println("On 64-bit boundary");
  newline();
  Serial.print("THIS ONE begins here: ");
  Serial.println("Fri Jul 23 07:25:25 UTC 2021\r\n");
  delay(500);
}

void setup1() {
  delay (4500);
}

uint8_t halting = 3; // is only 8 bits so it wraps around

#define CEILING 53
void loop() {
  halting++; if (halting >= CEILING) { halting = 3; }
  toggle_led();
  uint64_t a = rp2040.getCycleCount64();
  rdel();
  uint64_t b = rp2040.getCycleCount64();
  Serial.print("Core _ cycles:      ");
  uint64_t difference = (b - a) + 0;
  Serial.println(difference);
  jdel();
}


void loop1() {
  halting++; if (halting >= CEILING) { halting = 3; }
  for (volatile uint64_t rwaste = 0x7FFFF ; rwaste > 0; rwaste--) { }
  if ((halting == 27) || (halting == 28) ) {
      newline(' ');
      newline('4');
      newline('5');
      newline('6');
      newline('7');
      newline();
      newline();
      Serial.println("     GOT IT     ");
      newline();
      newline();
      delay(19999);
      newline();
      newline();
      Serial.println("             so now what              ");
      newline();
      newline();
      delay(19999);
  }
  rp2040.idleOtherCore();
  uint64_t c = rp2040.getCycleCount64();
  rp2040.resumeOtherCore();
  for (volatile int waste = 0x39FEFF ; waste > 0; waste--) { }
  rp2040.idleOtherCore();
  uint64_t d = rp2040.getCycleCount64();
  rp2040.resumeOtherCore();
  Serial.print("Core 1 cycles: ");
  uint64_t difference = (d - c) + 1024;

  if (difference == 1024) { Serial.print("        no cash value: "); }
  if (difference != 1024) { Serial.print("              "); }

  Serial.println(difference);

  newline("                                                      +");
  jdel();
}

// END.
