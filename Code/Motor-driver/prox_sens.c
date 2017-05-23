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

    //Read the interrupt Status register
    i2c_read(MAX_ADDR, 0x00);

    //Required because four msb are 0 on startup.
    i2c_write(MAX_ADDR, 0x02, 0xF3);

    //Set led drive current to 110 mA
    i2c_write(MAX_ADDR, 0x03, 0x0F);

    //Set main configuration register to ALS + PROX
    i2c_write(MAX_ADDR, 0x01, 0x30);

}

int16_t als_read() {

    uint8_t data[2];
    uint16_t als_data;

    i2c_read_multi(MAX_ADDR, 0x04, 2, data);   // read Als data high register followed by the Als data low register

    if (data[0] & 0x40) {                      // if the overflow bit is set
        return -1;
    }

    als_data = (data[0] << 8);
    als_data |= data[1];

    __no_operation();

    return als_data;
}

uint8_t prox_read() {

    return i2c_read(MAX_ADDR, 0x16);
}
