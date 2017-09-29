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
    CSCTL3 = DIVA__1 | DIVS__8 | DIVM__1;   // Set all dividers (SMCLK @ 1MHz)
    CSCTL0_H = 0;                           // Lock CS registers

    PJDIR |= BIT6;
    PJOUT &= ~BIT6;
}

int main(void) {

    init();
    i2c_init();
    ctrl_init();

    int8_t len = 6;
    int8_t inst[6] = {STRAIGHT, 20, TURN_RIGHT, 90, STRAIGHT, 20}; //, STRAIGHT, 20};
    //int8_t len = 2;
    //int8_t inst[2] = {TURN_RIGHT, 90};

    __delay_cycles(8000000);

    while(fram.cp < len/2){

        move(inst[2*fram.cp], inst[2*fram.cp+1]);

        while(!fram.stop){

        }
        __delay_cycles(8000000);
        fram.stop = 0;
        fram.cp++;
        fram.once = 0;
    }
    //fram.cp = 0;

    //Enable LED to drain excess energy
    PJOUT |= BIT6;

    return 0;
}
