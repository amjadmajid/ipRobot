/*
 * motor_ctrl.c
 *
 *  Created on: 23 jan. 2017
 *      Author: Koen Schaper
 */

#include <msp430.h>
#include <stdint.h>
#include "extern.h"
#include "motor_ctrl.h"
#include "eusci_b0_i2c.h"

void drv_init() {

    i2c_init();

    // enable TCA9539
    P1OUT |= BIT4;

    // make all outputs low
    i2c_transmit(0x02, 0x00);   //0x01 for TCA9538
    _delay_cycles(800);         // 100us delay;

    //configure output
    i2c_transmit(0x06, 0x00);   //0x03 for TCA9538
    _delay_cycles(800);         // 100us delay;
}

void prep_inst(uint8_t cmd, uint8_t len) {

    const uint8_t lstates[4] = {0x02, 0x08, 0x03, 0x0C};  // P0 = DA, P1 = PA, P2 = DB, P3 = PB
    const uint8_t rstates[4] = {0x0C, 0x03, 0x08, 0x02};  // P4 = DA, P1 = PA, P2 = DB, P3 = PB
    uint8_t cnt = 0;
    int8_t lstate = 0;
    int8_t rstate = 0;
    int8_t ladd = 0;
    int8_t radd = 0;

    switch(cmd) {
       case 0x01 : // forward
           ladd = 1;
           radd = 1;
           break;
       case 0x02 : // left (hard)
           ladd = -1;
           radd = 1;
           break;
       case 0x03 : // right (hard)
           ladd = 1;
           radd = -1;
           break;
       case 0x04 : // reverse
           ladd = -1;
           radd = -1;
           break;
       /* case 0x05 : // left (soft forward)

           break;
       case 0x06 : // right (soft forward)

           break;
       case 0x07 : // left (soft reverse)

           break
       case 0x08 : // right (soft reverse)

           break;
       default : */

    }

    while(cnt < len) {
        lstate = lstate + ladd;
        rstate = rstate + radd;
        if (lstate > 3) {
            lstate = 0;
        } else if (lstate < 0) {
            lstate = 3;
        }
        if (rstate > 3) {
            rstate = 0;
        } else if (rstate < 0) {
            rstate = 3;
        }
        fram->inst[cnt] = (lstates[lstate] | (rstates[rstate] << 4));
        cnt++;
    }
    fram->len = len;
    fram->cp++;
}

void drive_motors() {

    while (fram->cnt < fram->len) {

        P4OUT |= BIT0;
        // enable both motor drivers
        P3OUT |= (BIT4 | BIT5);
        i2c_transmit(0x02, fram->inst[fram->cnt]); //0x01 for TCA9538

        __delay_cycles((MCF/FREQ) * DUTY);
        fram->cnt++;

        P4OUT &= ~BIT0;
        i2c_transmit(0x02, 0x00); //0x01 for TCA9538
        // disable both motor drivers
        P3OUT &= ~(BIT4 | BIT5);

        __delay_cycles((MCF/FREQ) * (1-DUTY));
    }
}
