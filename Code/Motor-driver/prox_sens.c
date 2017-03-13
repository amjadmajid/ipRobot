/*
 * prox_sens.c
 *
 *  Created on: 13 mrt. 2017
 *      Author: Koen Schaper
 */

#include <msp430.h>
#include <stdint.h>
#include "prox_sens.h"
#include "eusci_b0_i2c.h"


void prox_init(){

    // enable MAX44000
    //P1OUT |= BIT4;

    //Set led drive current to 100 mA
    i2c_write(MAX_ADDR, 0x03, 0x0E);
    //Set main configuration register to PROX only
    i2c_write(MAX_ADDR, 0x01, 0x34);
}

uint8_t prox_read() {

    return i2c_read(MAX_ADDR, 0x16);
}
