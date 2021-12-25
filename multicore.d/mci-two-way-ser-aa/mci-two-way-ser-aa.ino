// mci-two-way-ser-aa.ino

// Sat Dec 25 23:36:04 UTC 2021

/*
    [ https://github.com/earlephilhower/arduino-pico/discussions/378 ]
 */

#define newline(x) Serial.println(x)

bool led_state;

void toggle_led(void) {
    led_state = !led_state;
    digitalWrite(LED_BUILTIN, led_state);
}

// ENiD,

void setup() {
  Serial.begin(115200);
  delay(30000);
  Serial.printf("Connect %d (Serial1 TX) to %d (Serial2 RX)\n\n", PIN_SERIAL1_TX, PIN_SERIAL2_RX);
  Serial1.begin(115200);
  Serial2.begin(115200);

  pinMode(LED_BUILTIN, 1); // output
  for (int count = 7; count > 0; count--) {
      toggle_led();
    // digitalWrite(LED_BUILTIN, 1); // ON
    delay(70);
    toggle_led();
      // digitalWrite(LED_BUILTIN, 0); // OFF
    delay(800);
  }
}

void loop() {
  Serial.printf("S2 avail: %d\n", Serial2.available());
  Serial1.write('a');
  newline('4');
  newline(" four ");
  delay(1000);
  Serial.printf("S2 avail: %d\n", Serial2.available());
  Serial1.write('b');
  delay(1000);
  Serial.printf("S2 avail: %d\n", Serial2.available());
  Serial1.write('c');
  delay(1000);
  Serial.printf("S2 avail: %d\n", Serial2.available());
  Serial1.printf("defg");
  delay(1000);
  Serial.printf("S2 avail: %d\n", Serial2.available());
  delay(1000);
  while (Serial2.available()) {
    Serial.printf("S2 read: '%c'\n", Serial2.read());
  }
  delay(10000);
}

// END.
