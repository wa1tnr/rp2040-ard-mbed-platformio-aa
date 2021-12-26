// mci-two-way-ser-aa.ino

// Sun Dec 26 12:21:05 UTC 2021

/*
    [ https://github.com/earlephilhower/arduino-pico/discussions/378 ]
 */

#include <Arduino.h>
#include "program.h"

#define newline(x) Serial.println(x)

bool led_state;

void toggle_led(void) {
    led_state = !led_state;
    digitalWrite(LED_BUILTIN, led_state);
}

void setup(void) {
    // reflash_firmware();
    Serial.begin(115200); // USB
    delay(4000);
    // GPIO0  PIN_SERIAL1_TX
    // GPIO9  PIN_SERIAL2_RX
    newline();
    newline("Sun 26 Dec 12:21:59 UTC 2021");
    Serial.printf("Connect %d (Serial1 TX) to %d (Serial2 RX)\n\n",
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
    for (int iterations = 3; iterations > 0; iterations--) { // do it three times then reflash
        Serial.printf("S2 avail: %d\n", Serial2.available());
        Serial1.write('a');
        // newline('4');  // test code for the 'newline()' cpp macro
        // newline(" four ");
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
        delay(4000);
    }
    reflash_firmware();
}

// END.
