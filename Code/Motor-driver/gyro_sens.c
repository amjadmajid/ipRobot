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

    volatile uint8_t status;

    // enable BMG250
    //P2OUT |= BIT1; //P2.1 (UART_RX)

    //Set to normal mode (requires typically 55ms)
    i2c_write(BMG_ADDR, 0x7E, 0x15);

    __delay_cycles(440);

    //Read PMU_STATUS
    status = i2c_read(BMG_ADDR, 0x03);

    __no_operation();

    //Self test
    //i2c_write(BMG_ADDR, 0xD6, 0x10);

    //__delay_cycles(1000000);

    //Read STATUS
    //status = i2c_read(BMG_ADDR, 0x1B);

    __no_operation();

    //Start automatic offset compensation
    i2c_write(BMG_ADDR, 0x7E, 0x03);

    //Wait for completion
    while(!(i2c_read(BMG_ADDR, 0x1B) & 0x08))


}

uint16_t gyro_read() {

    uint8_t data[6];
    uint16_t ret;

    i2c_read_multi(BMG_ADDR, 0x12, 6, data);

    // Only return z-axis data!
    ret = data[5] << 8;
    return (ret | data[4]);
}
