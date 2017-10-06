#include <msp430.h> 
#include <stdint.h>
#include "global.h"
#include "eusci_b0_i2c.h"
#include "ctrl_loop.h"
#include "bor_timer.h"

/*
 * main.c
 */

#pragma PERSISTENT(fram);
NVvar fram = {0};
#pragma PERSISTENT(fram_wc);
NVvar fram_wc = {0};

void swap(NVvar *a, NVvar *b){

    NVvar tmp = *b;
    *b   = *a;
    *a   = tmp;
}

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

    __delay_cycles(8000000);

    i2c_init();
    ctrl_init();

    //int8_t len = 6;
    //int8_t inst[6] = {STRAIGHT, 20, TURN_RIGHT, 90, STRAIGHT, 20}; //, STRAIGHT, 20};
    int8_t len = 2;
    uint16_t inst[2] = {STRAIGHT, 50};

#if SPI
    // Create power interrupt after approx 1.1sec
    start_bor_timer(1100);
#endif

    if(fram.nonce){
        move(TURN_RIGHT, 4);
        while(!fram.stop){

        }
        //__delay_cycles(800000);     // 0.1sec delay between movements!
        fram.stop = 0;
    }
    fram.nonce = 1;

    while(fram.cp < len/2){

        move(inst[2*fram.cp], inst[2*fram.cp+1]);
        while(!fram.stop){

        }
        __delay_cycles(800000);     // 0.1sec delay between movements!

        // Double buffer to keep consistency
        fram_wc = fram;
        fram_wc.stop = 0;
        fram_wc.cp++;
        swap(&fram, &fram_wc);

    }
    // Double buffer to keep consistency
    fram_wc = fram;
    fram_wc.nonce = 0;
    fram_wc.cp = 0;
    swap(&fram, &fram_wc);

#if SPI
    stop_bor_timer();
#endif

    //Enable LED to drain excess energy
    PJOUT |= BIT6;

    return 0;
}
