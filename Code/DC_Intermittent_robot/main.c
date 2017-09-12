#include <msp430.h> 
#include <stdint.h>
#include "global.h"
#include "eusci_b0_i2c.h"
#include "ctrl_loop.h"

/*
 * main.c
 */

#pragma PERSISTENT(fram);
NVvar fram = {0};

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

    PJDIR |= BIT6;
    PJOUT &= ~BIT6;
}

int main(void) {


    init();
    i2c_init();
    ctrl_init();

    if(fram.cp == 0){
       fram.cnt = 0;
       fram.cp = 1;
    }

    set_tunings(0.3*0.6, (16/50)/2, (16/50)/8);
    set_setpoint(0);
    set_limits(355, -355);
    enbl_loop(100, 100);

    while(1){
        if(fram.cnt >= 400)
            break;
    }
    dsbl_loop();
    fram.cp = 0;

    //Enable LED to drain excess energy
    PJOUT |= BIT6;

    return 0;
}
