/*
 * motor_ctrl.c
 *
 *  Created on: 23 jan. 2017
 *      Author: Koen Schaper
 */

#include <msp430.h>
#include <stdint.h>
#include "global.h"
#include "motor_ctrl.h"
#include "eusci_b0_i2c.h"
#include "particle_filter.h"

void swap(pi **r, pi **s) {

   pi *tmp = *r;
   *r = *s;
   *s = tmp;
}

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

void prep_inst(uint8_t cmd, uint8_t len) {

    const uint8_t lstates[4] = {0x0C, 0x03, 0x08, 0x02};  // P0 = DA, P1 = PA, P2 = DB, P3 = PB
    const uint8_t rstates[4] = {0x02, 0x08, 0x03, 0x0C};  // P4 = DA, P1 = PA, P2 = DB, P3 = PB

    float temp = 1;

    uint8_t cnt = pi_or->cnt;
    int8_t lstate = pi_or->lstate;
    int8_t rstate = pi_or->rstate;

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
        if(!DEBUG)
            drv_mot(lstates[lstate] | (rstates[rstate] << 4));
        *pi_wc = *pi_or;
        pi_wc->lstate = lstate;
        pi_wc->rstate = rstate;
        pi_wc->cnt = ++cnt;
        swap(&pi_or, &pi_wc);

        temp = move(0.2355*2, 0);
        temp = update(temp);
        if(temp < 0.5*100){
            resample();
        }
    }
    pi_or->cnt = 0;
}

void drv_mot(uint8_t data) {

    // enable both motor drivers
    P3OUT |= (BIT4 | BIT5);
    i2c_write(TCA_ADDR, 0x01, data); //0x02 for TCA9539
    __delay_cycles(DELAY);
}

void dsbl_mot() {

    i2c_write(TCA_ADDR, 0x01, 0x00); //0x02 for TCA9539
    // disable both motor drivers
    P3OUT &= ~(BIT4 | BIT5);
}
