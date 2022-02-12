#include "compatibility.h"
#ifdef NOT_USING_MBED
#include <Arduino.h>
#include "pico/bootrom.h"

void reflash_firmware(void) {
    reset_usb_boot(0, 0);
}
#endif
