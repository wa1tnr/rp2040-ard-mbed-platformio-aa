// mci-two-way-ser-aa.ino

// Sun 26 Dec 16:59:05 UTC 2021

/*
    [ https://github.com/earlephilhower/arduino-pico/discussions/378 ]
 */

#include <Arduino.h>
#include "program.h"

#define newline(x) Serial.println(x)


#define REQUIRE_SERIAL_CONNECTION
#undef  REQUIRE_SERIAL_CONNECTION

#ifdef REQUIRE_SERIAL_CONNECTION
  #define SERIAL_REQUIRED -1
#endif

#ifndef REQUIRE_SERIAL_CONNECTION
  #define SERIAL_REQUIRED  0
#endif

void toggle_led(void) {
    bool led_state;
    led_state = !led_state;
    digitalWrite(LED_BUILTIN, led_state);
}

void setup(void) {
    // reflash_firmware();
    Serial.begin(115200); // USB
    delay(4000);
    if (SERIAL_REQUIRED) { while (!Serial); }
    newline();
    newline("Sun 26 Dec 16:59:05 UTC 2021");
    // GPIO0  PIN_SERIAL1_TX
    // GPIO9  PIN_SERIAL2_RX
    Serial.printf("Connect %d (Serial1 TX) to %d (Serial2 RX)\n",
                  PIN_SERIAL1_TX, PIN_SERIAL2_RX); // decodes these symbols and prints to the user
    Serial1.begin(115200); // USART
    Serial2.begin(115200); // USART - that's *three* Serial.begin()'s .. wow.

    pinMode(LED_BUILTIN, 1); // output
    for (int count = 7; count > 0; count--) {
        toggle_led();
        delay(70);
        toggle_led();
        delay(800);
    }
    // reflash_firmware(); // early escape when needed - held as a comment
    // while(-1); // HOLD FOREVER - when loop() is not needed
}

void loop() {
    int loop_count = 0;
    for (int iterations = 3; iterations > 0; iterations--) { // do it three times then reflash
        loop_count++;
        Serial.printf("\nloop count: %d   ", loop_count);
        Serial.printf("\n");
        Serial.printf("S2 avail: %d   ", Serial2.available());
        Serial1.write('a');
        // newline('4');  // test code for the 'newline()' cpp macro
        // newline(" four ");
        delay(1000);
        Serial.printf("S2 avail: %d   ", Serial2.available());
        Serial1.write('b');
        delay(1000);
        Serial.printf("S2 avail: %d   ", Serial2.available());
        Serial1.write('c');
        delay(1000);
        Serial.printf("S2 avail: %d   ", Serial2.available());
        Serial1.printf("defg");
        delay(1000);
        Serial.printf("S2 avail: %d   ", Serial2.available());
        delay(1000);
        Serial.printf("\n");
        while (Serial2.available()) {
            Serial.printf("S2 read: '%c'   ", Serial2.read());
        }
        Serial.printf("\n");
        delay(4000);
    }
    reflash_firmware();
}

// END.
