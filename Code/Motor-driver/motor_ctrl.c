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
    i2c_transmit(0x01, 0x00);   //0x02 for TCA9539
    _delay_cycles(800);         // 100us delay;

    //configure output
    i2c_transmit(0x03, 0x00);   //0x06 for TCA9539
    _delay_cycles(800);         // 100us delay;
}

void prep_inst(uint8_t cmd, uint8_t len) {

    const uint8_t lstates[4] = {0x0C, 0x03, 0x08, 0x02};  // P0 = DA, P1 = PA, P2 = DB, P3 = PB
    const uint8_t rstates[4] = {0x02, 0x08, 0x03, 0x0C};  // P4 = DA, P1 = PA, P2 = DB, P3 = PB
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

void drv_mot() {

    while (fram->cnt < fram->len) {

        // enable both motor drivers
        P3OUT |= (BIT4 | BIT5);
        i2c_transmit(0x01, fram->inst[fram->cnt]); //0x02 for TCA9539

        __delay_cycles(DELAY);
        fram->cnt++;
    }
}

void dsbl_mot() {

    i2c_transmit(0x01, 0x00); //0x02 for TCA9539
    // disable both motor drivers
    P3OUT &= ~(BIT4 | BIT5);

}
