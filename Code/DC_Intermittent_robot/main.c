#include <msp430.h> 
#include <stdint.h>
#include "global.h"
#include "eusci_b0_i2c.h"
#include "motor_ctrl.h"

/*
 * main.c
 */

#pragma PERSISTENT(fram);
NVvar fram = {0};
uint8_t running;

void init(void) {

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    FRCTL0 = 0xA500 | ((1) << 4);           // Disable FRAM wait cycles to allow clock operation over 8MHz

    CSCTL0_H = CSKEY_H;                     // Unlock clock registers
    CSCTL1 = DCOFSEL_6;                     // Set DCO to 8MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__4 | DIVM__1;   // Set all dividers
    CSCTL0_H = 0;                           // Lock CS registers

    P4DIR |= BIT0;
    P4OUT &= ~(BIT0);
}

int main(void) {

    init();
    drv_init();

    /*if(CNT_AFTER){
        fram.cnt--;
    }*/

    enbl_mot();
    forward();
    __delay_cycles(8000000);
    reverse();
    __delay_cycles(8000000);
    dsbl_mot();

    /*running=1;
    while(running) {
        // DO NOTHING

    }*/

    return 0;
}
