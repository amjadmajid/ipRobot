/*
 * motor_ctrl.c
 *
 *  Created on: 23 jan. 2017
 *      Author: Koen Schaper
 */

#include <msp430.h>
#include <stdint.h>
#include "motor_ctrl.h"
#include "eusci_b0_i2c.h"

/*void swap(pi **r, pi **s) {

   pi *tmp = *r;
   *r = *s;
   *s = tmp;
} */

void drv_init() {

    P1DIR |= BIT4;                     // Set P1.4 (AUX3) to output
    P1OUT |= BIT4;                     // Enable TCA9538

    P3DIR |= BIT4 | BIT5;              // Set P3.4 and P3.5 (AUX1 and AUX2) to output
    P3OUT &= ~(BIT4 | BIT5);           // Disable both motor drivers

    // make all outputs low
    i2c_write(TCA_ADDR, 0x01, 0x00);   //0x02 for TCA9539

    //configure output
    i2c_write(TCA_ADDR, 0x03, 0x00);   //0x06 for TCA9539
}

void drv_mot(uint8_t data) {

    // enable both motor drivers
    P3OUT |= (BIT4 | BIT5);
    i2c_write(TCA_ADDR, 0x01, data); //0x02 for TCA9539
}

void dsbl_mot() {

    i2c_write(TCA_ADDR, 0x01, 0x00); //0x02 for TCA9539
    // disable both motor drivers
    P3OUT &= ~(BIT4 | BIT5);
}
