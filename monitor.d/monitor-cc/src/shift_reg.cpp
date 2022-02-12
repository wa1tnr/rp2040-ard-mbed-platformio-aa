#include <Arduino.h>

const int latchPin = 2; /* STCP */
const int clockPin = 3; /* SHCP */
const int dataPin = 4; /* DS */

byte leds = 0;
byte uleds = 0;
byte position = 15; // rightmost

uint8_t ledval = 0;

void setup_sr(void) {
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin,  OUTPUT);
}

void _digitSelect(void) {
    uleds = position ;
    shiftOut(dataPin, clockPin, MSBFIRST, uleds);
}

void updateShiftRegister(void) {
    digitalWrite(latchPin, LOW);
    _digitSelect();
    uleds = leds;
    shiftOut(dataPin, clockPin, MSBFIRST, uleds); // paint glyph
    digitalWrite(latchPin, HIGH);
}

void blankleds(void) {
    leds = 0;
    updateShiftRegister();
}

void setleds(void) {
    leds = ledval;
    updateShiftRegister();
}

void flash_digit(void) { // paint a single digit brightly, then immediately blank all LEDs
    setleds();
    // blankleds(); // waste no time in doing so!
}

void clear_shift_reg(void) {
    setup_sr();
    // ledval = 63;
    // ledval = 1 + 2 + 4 + 0 +  0 + 32 + 64 +   0; // '9'
    ledval = 0 + 2 + 4 + 0 +  0 + 32 + 64 +   0; // '4'
    position = 15 ; flash_digit();
    delay(350);

    // ledval = 1 + 2 + 4 + 8 + 16 + 32 + 64 +   0; // '8'
    ledval = 1 + 2 + 4 + 8 +  0 +  0 + 64 +   0; // '3'
    position = 22 ; flash_digit();
    delay(350);

    // ledval = 1 + 2 + 4 + 0 +  0 +  0 +  0 +   0; // '7'
    ledval = 1 + 2 + 0 + 8 + 16 +  0 + 64 +   0; // '2'
    position = 27 ; flash_digit();
    delay(350);

    // ledval = 1 + 0 + 4 + 8 + 16 + 32 + 64 +   0; // '6'
    ledval = 0 + 2 + 4 + 0 +  0 +  0 +  0 +   0; // '1'
    position = 29 ; flash_digit();
    delay(350);
}

// END.
