/*
 * defs.h
 *
 * Created: 01/03/2025 10:07:08
 *  Author: Simon
 */
#include <Arduino.h>

#ifndef DEFS_H_
#define DEFS_H_

//~~~~~~~~~ UNIT SELECT ~~~~~~~~~~~~~~~~~
#define UNIT_TEST 0
#define UNIT_PATIO 1
#define UNIT_ASTRAL 2
#define UNIT_OFFICE 3
#define UNIT_KITCHEN 4
#define UNIT_LOUNGE 5
#define UNIT_BEDROOM 6

#define UNIT UNIT_BEDROOM

#if UNIT==UNIT_TEST
    #include <test-network.h>
    #define IDSTR "PAT"
    #define NODE_IDENT PATIO_NODEID
    #undef IS_RFM69HW_HCW
    #define IS_RFM69HW_HCW false
    #define RADIO_TX_POWER 29
    #define USE_POWER_SWITCH 0
    #define SWAP_SPI 1
    #define SWAP_SERIAL 1
#elif UNIT==UNIT_PATIO
    #include <study-network.h>
    #define IDSTR "PAT"
    #define NODE_IDENT PATIO_NODEID
    #undef IS_RFM69HW_HCW
    #define IS_RFM69HW_HCW false
    #define RADIO_TX_POWER 29
    #define USE_POWER_SWITCH 0
    #define SWAP_SPI 1
    #define SWAP_SERIAL 1
#elif UNIT==UNIT_ASTRAL
    #include <study-network.h>
    #define IDSTR "AST"
    #define NODE_IDENT ASTRAL_NODEID
    #define RADIO_TX_POWER 29
    #define USE_POWER_SWITCH 1
    #define SWAP_SPI 0
    #define SWAP_SERIAL 0
    #define TEMP_OFFSET 0
#elif UNIT==UNIT_OFFICE
    #include <study-network.h>
    #define IDSTR "OFF"
    #define NODE_IDENT OFFICE_NODEID
    #define RADIO_TX_POWER 29
    #define USE_POWER_SWITCH 1
    #define SWAP_SPI 0
    #define SWAP_SERIAL 0
    #define TEMP_OFFSET -1.7
#elif UNIT==UNIT_KITCHEN
    #include <study-network.h>
    #define IDSTR "KIT"
    #define NODE_IDENT KITCHEN_NODEID
    #define RADIO_TX_POWER 29
    #define USE_POWER_SWITCH 1
    #define SWAP_SPI 0
    #define SWAP_SERIAL 0
    #define TEMP_OFFSET 0
#elif UNIT==UNIT_LOUNGE
    #include <study-network.h>
    #define IDSTR "LNG"
    #define NODE_IDENT LOUNGE_NODEID
    #define RADIO_TX_POWER 29
    #define USE_POWER_SWITCH 1
    #define SWAP_SPI 0
    #define SWAP_SERIAL 0
    #define TEMP_OFFSET 0
#elif UNIT==UNIT_BEDROOM
    #include <study-network.h>
    #define IDSTR "BED"
    #define NODE_IDENT BEDROOM_NODEID
    #define RADIO_TX_POWER 29
    #define USE_POWER_SWITCH 1
    #define SWAP_SPI 0
    #define SWAP_SERIAL 0
    #define TEMP_OFFSET 0
#endif
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define WAKE_DELAY (121+3*NODE_IDENT)




#if 1
/**
 * Pins for V3.2 board (BME280)
 */

 
// On the Arduino board, digital pins are also used
// for the analog output (software PWM).  Analog input
// pins are a separate set.

// ATtiny1614 / ARDUINO
//                          _____
//                  VDD   1|*    |14  GND
// (nSS)  (AIN4) PA4  0~  2|     |13  10~ PA3 (AIN3)(SCK)(EXTCLK)
//        (AIN5) PA5  1~  3|     |12  9   PA2 (AIN2)(MISO)
// (DAC)  (AIN6) PA6  2   4|     |11  8   PA1 (AIN1)(MOSI)
//        (AIN7) PA7  3   5|     |10  11  PA0 (nRESET/UPDI)
// (RXD) (TOSC1) PB3  4   6|     |9   7~  PB0 (AIN11)(SCL)
// (TXD) (TOSC2) PB2  5~  7|_____|8   6~  PB1 (AIN10)(SDA)
//                    ^               ^
//                    |               |
//                    --------------------- USE THESE NUMBERS....
//

// See https://github.com/SpenceKonde/megaTinyCore/blob/master/megaavr/extras/ATtiny_x14.md
// ...which does not agree
//
// I/O Usage
// NOTE: You must use the wacky Arduino like ids... :(

struct Pins
{
    // PIN_PA0 is UDPI
    static const uint8_t MOSI       = PIN_PA1;
    static const uint8_t MISO       = PIN_PA2;
    static const uint8_t SCK        = PIN_PA3;
    static const uint8_t LDR        = PIN_PA4;
    static const uint8_t VBATT      = PIN_PA5;
    static const uint8_t POWER_SW   = PIN_PA6;
    static const uint8_t BME280_CS  = PIN_PA7;

    static const uint8_t RADIO_CS   = PIN_PB0;
    static const uint8_t RADIO_IRQ  = PIN_PB1;
    static const uint8_t TX         = PIN_PB2;
    static const uint8_t RX         = PIN_PB3;

};

#else

// ATtiny1616 / ARDUINO
//                          _____
//                  VDD   1|*    |20  GND
// (nSS)  (AIN4) PA4  0~  2|     |19  16~ PA3 (AIN3)(SCK)(EXTCLK)
//        (AIN5) PA5  1~  3|     |18  15  PA2 (AIN2)(MISO)
// (DAC)  (AIN6) PA6  2   4|     |17  14  PA1 (AIN1)(MOSI)
//        (AIN7) PA7  3   5|     |16  17  PA0 (AIN0/nRESET/UPDI)
//        (AIN8) PB5  4   6|     |15  13  PC3
//        (AIN9) PB4  5   7|     |14  12  PC2
// (RXD) (TOSC1) PB3  6   8|     |13  11~ PC1 (PWM only on 1-series)
// (TXD) (TOSC2) PB2  7~  9|     |12  10~ PC0 (PWM only on 1-series)
// (SDA) (AIN10) PB1  8~ 10|_____|11   9~ PB0 (AIN11)(SCL)
//                    ^               ^
//                    |               |
//                    --------------------- USE THESE NUMBERS....

/**
 * Pins for V3A board [HS1101]
 */
struct Pins
{
    // PIN_PA0 is UDPI
    static const uint8_t TX = PIN_PA1;
    static const uint8_t RX = PIN_PA2;
    static const uint8_t LED = PIN_PA3;
    static const uint8_t RADIO_IRQ = PIN_PA4;
    static const uint8_t RADIO_CS = PIN_PA5;
    // PA6 spare
    // PA7 spare

    static const uint8_t THERMISTOR = PIN_PB0;
    static const uint8_t LDR = PIN_PB1;
    // PB2 XTAL
    // PB3 XTAL
    static const uint8_t VBATT = PIN_PB4;
    static const uint8_t POWER_SWITCH = PIN_PB5;

    static const uint8_t SCK = PIN_PC0;
    static const uint8_t MISO = PIN_PC1;
    static const uint8_t MOSI = PIN_PC2;
    static const uint8_t HUMID = PIN_PC3; // freq input

};
#endif

const unsigned IOT_BUFFSIZE=60;

#endif /* DEFS_H_ */