/*
 * main.c
 *
 */

#include <msp430.h>
#include <stdint.h>
#include "extern.h"
#include "motor_ctrl.h"

struct NVvar * fram = (struct NVvar *) 0x1800;

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

    P4DIR |= BIT0;                          // Set led2 pin to output
    P4OUT &= ~BIT0;

    P1DIR |= BIT4;                          // Set P1.4 (AUX3) to output
    P1OUT &= ~BIT4;                         // Hold TCA9539 in reset (active low)

    P3DIR |= BIT4 | BIT5;                   // Set P3.4 and P3.5 (AUX1 and AUX2) to output
    P3OUT &= ~(BIT4 | BIT5);                // Disable both motor drivers
}

int main(void) {

    const uint16_t stm = 40;

    if( fram->cp == 0x00 ){
        fram->cnt = 0;
        fram->cp = 0x01;
    }

    init();
    drv_init();

    if( fram->cp == 0x01 ) {
        prep_inst(0x01, stm);
    }

    drive_motors();

    fram->cp = 0x00;

    return 0;
}
