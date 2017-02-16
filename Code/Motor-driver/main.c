/*
 * main.c
 *
 */

#include <msp430.h>
#include <stdint.h>
#include "global.h"
#include "motor_ctrl.h"

// Note: INFOD has a length of 128 bytes
NVvar * fram = (NVvar *) 0x1800;
pi *pi_or = (pi *) 0x180A;
pi *pi_wc = (pi *) 0x180D;

void init(void) {

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    FRCTL0 = 0xA500 | ((1) << 4);           // Disable FRAM wait cycles to allow clock operation over 8MHz

    CSCTL0_H = CSKEY_H;                     // Unlock clock registers
    CSCTL1 = DCOFSEL_3;                     // Set DCO to 8MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__8 | DIVM__1;   // Set all dividers
    CSCTL0_H = 0;                           // Lock CS registers

    P1DIR |= BIT4;                          // Set P1.4 (AUX3) to output
    P1OUT &= ~BIT4;                         // Hold TCA9539 in reset (active low)

    P3DIR |= BIT4 | BIT5;                   // Set P3.4 and P3.5 (AUX1 and AUX2) to output
    P3OUT &= ~(BIT4 | BIT5);                // Disable both motor drivers
}

int main(void) {

    const uint8_t len = 10;
    const uint8_t inst_cmd[10] = {0x01, 0x03, 0x01, 0x03, 0x01, 0x03, 0x01, 0x03, 0x01, 0x03};
    const uint8_t inst_len[10] = {41, 1, 41, 1, 41, 1, 41, 1, 41, 1};

    init();
    if(!DEBUG)
        drv_init();

    while(1) {

        if(fram->cp_nr == 0x00) {
            fram->inst = 0;
            pi_or->cnt = 0;
            fram->cp_nr = 0x01;
        }

        while(fram->inst < len){
            prep_inst(inst_cmd[fram->inst], inst_len[fram->inst]);
            fram->inst++;
        }

        if(!DEBUG)
            dsbl_mot();

        fram->cp_nr = 0x00;
    }

    return 0;
}
