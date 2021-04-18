/*
    Fade

    This example shows how to fade the onboard Raspberry Pi Pico LED

    This example code is in the public domain.

    http://www.arduino.cc/en/Tutorial/Fade
*/

/*

    18 April 2021 - following the consequences of:

    [ https://github.com/earlephilhower/arduino-pico#installing-via-git ]

    Sun Apr 18 21:58:58 UTC 2021

*/

int led = LED_BUILTIN; // the PWM pin the LED is attached to
int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

// the setup routine runs once when you press reset:
void setup() {
    // declare pin to be an output:
    pinMode(led, OUTPUT);
    Serial.begin(9600);
    while (!Serial);
    delay(5500);
    Serial.println("Hello from Arduino side RP2040 foo!");
    Serial.println("Reflash in 20 seconds.");

    // reflash
}

// the loop routine runs over and over again forever:
void loop() {
    // set the brightness
    analogWrite(led, brightness);

    // change the brightness for next time through the loop:
    brightness = brightness + fadeAmount;

    // reverse the direction of the fading at the ends of the fade:
    if (brightness <= 0 || brightness >= 255) {
        fadeAmount = -fadeAmount;
    }
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
}
