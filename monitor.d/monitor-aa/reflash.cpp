#include <Arduino.h>
/*
#include "CoreMutex.h"

#include "tusb.h"
#include "pico/time.h"
#include "pico/binary_info.h"
*/
#include "pico/bootrom.h"
/*
#include "hardware/irq.h"
#include "pico/mutex.h"
#include "hardware/watchdog.h"
#include "pico/unique_id.h"
*/

void reflash_firmware(void) {
    reset_usb_boot(0, 0);
}

