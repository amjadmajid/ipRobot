/*
 * gyro_sens.c
 *
 *  Created on: 1 mei 2017
 *      Author: Koen Schaper
 */


#include <msp430.h>
#include <stdint.h>
#include "gyro_sens.h"
#include "eusci_b0_i2c.h"

void gyro_init(){

    // enable BMG250
    //P2OUT |= BIT1; //P2.1 (UART_RX)

    //Software reset
    //i2c_write(BMG_ADDR, 0x7E, 0xB6);

    //__delay_cycles(100000);

    //Set to normal mode
    i2c_write(BMG_ADDR, 0x7E, 0x15);

    //Wait for normal mode (requires typically 55ms)
    __delay_cycles(400000);

    //while(!(i2c_read(BMG_ADDR, 0x03) & 0x04));

    //Set angular rate measurement range to 250deg/s
    i2c_write(BMG_ADDR, 0x43, 0x03);

    //Start automatic offset compensation
    i2c_write(BMG_ADDR, 0x7E, 0x03);

    //Wait for completion (max 250ms)
    while(!(i2c_read(BMG_ADDR, 0x1B) & 0x08));

    //Enable FOC?
    i2c_write(BMG_ADDR, 0x77, (i2c_read(BMG_ADDR, 0x77) | 0x80));

    //Remove stall data, clear data ready bit
    gyro_read();

    __delay_cycles(80000);
}

int16_t gyro_read() {

    uint8_t data[6];
    int16_t ret;

    //Check if data ready bit is set
    while(!(i2c_read(BMG_ADDR, 0x1B) & 0x40));

    i2c_read_multi(BMG_ADDR, 0x12, 6, data);

    // Only return z-axis data!
    ret = data[5] << 8;
    ret |= data[4];
    return ret;
}
