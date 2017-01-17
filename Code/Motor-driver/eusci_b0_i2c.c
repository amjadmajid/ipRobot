/*
 * eusci_b0_i2c.c
 *
 *  Created on: 16 jan. 2017
 *      Author: Koen Schaper
 */

#include <msp430.h>
#include <stdint.h>
#include "eusci_b0_i2c.h"

void i2c_init(){

    P1SEL1 |= BIT6 | BIT7;                  // configure I2C pins
    P1SEL0 &= ~(BIT6 | BIT6);               // configure I2C pins

    // I2C default uses SMCLK
    UCB0CTL1 |= UCSWRST;                    // put eUSCI_B in reset state
    UCB0CTLW0 |= UCMODE_3 | UCMST | UCSYNC; // I2C, master, sync
    UCB0BRW = 0x000A;                       // baud rate = SMCLK / 10 = 100khz
    UCB0CTLW1 = UCASTP_2;                   // automatic STOP assertion
    UCB0TBCNT = DATA_SIZE;                  // TX 2 bytes of data
    UCB0I2CSA = SLAVE_ADDR;                 // slave address
    UCB0CTL1 &= ~UCSWRST;                   // eUSCI_B in operational state
}

void i2c_transmit(uint8_t cmd, uint8_t data){

    UCB0CTLW0 |= UCTR | UCTXSTT;            // transmitter mode and START condition.

    while(!(UCB0IFG & UCTXIFG0));
    UCB0TXBUF = cmd;
    while(!(UCB0IFG & UCTXIFG0));
    UCB0TXBUF = data;
    while(UCB0CTLW0 & UCTXSTP);             // wait for stop
}
