// adjust to your compiler environment

// philhower:
#define NOT_USING_MBED

// mbed, two variants:
#ifdef ARDUINO_ARCH_MBED
    #undef NOT_USING_MBED // yes, using MBED
    #ifndef ARDUINO_ARCH_MBED_RP2040 // platformio will trigger this clause, probably
        #define USING_PLATFORMIO_MBED
    #endif
#endif
