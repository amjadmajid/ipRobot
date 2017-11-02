#include <msp430.h> 
#include <stdint.h>
#include "global.h"
#include "eusci_b0_i2c.h"
#include "ctrl_loop.h"
#include "bor_timer.h"

/*
 * main.c
 */

#if PERSISTENT==1
#pragma PERSISTENT(fram);
#endif
nv_var fram = {0};
#if PERSISTENT==1
#pragma PERSISTENT(fram_wc);
#endif
nv_var fram_wc = {0};

#if ROBOT==1
robot_conf conf = {
        {0.13*0.6, (20/100)/2, (20/100)/8},     //tunings straight
        {0.20, 0, 0},                           //tunings turn_left
        {0.07, 0, 0},                           //tunings turn_right
        {80, 65, 75}                            //lmin, rmin, smax
};
#elif ROBOT==2
robot_conf conf = {
        {0.18*0.6, (20/100)/2, (20/100)/8},     //tunings straight
        {0.25, 0, 0},                           //tunings turn_left
        {0.07, 0, 0},                           //tunings turn_right
        {70, 110, 90}                           // lmin, rmin, smax
};
#elif ROBOT==3
robot_conf conf = {
        {0.13*0.6, (20/100)/2, (20/100)/8},     //tunings straight
        {0.3, 0, 0},                            //tunings turn_left
        {0.3, 0, 0},                            //tunings turn_right
        {90, 80, 85}                           // lmin, rmin, smax
};
#else
    //ERROR
#endif

void swap(nv_var *a, nv_var *b){

    nv_var tmp = *b;
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
    ctrl_init(conf);

    int8_t len = 12;
    uint16_t inst[12] = {STRAIGHT, 50, TURN_RIGHT, 180, STRAIGHT, 50, TURN_RIGHT, 180, STRAIGHT, 50, TURN_RIGHT, 180};
    //int8_t len = 2;
    //uint16_t inst[2] = {TURN_LEFT, 360};

#if SPI
    // Create power interrupt after approx 1.1sec
    start_bor_timer(1100);
#endif

    while(fram.cp < len/2){

        move(inst[2*fram.cp], inst[2*fram.cp+1]);

        while(!fram.stop){

        }
        __delay_cycles(16000000);     // 0.1sec delay between movements!
        // Double buffer to keep consistency
        fram_wc = fram;
        fram_wc.stop = 0;
        fram_wc.cp++;
        swap(&fram, &fram_wc);
    }
#if !DEBUG
    fram.cp = 0;
#endif

#if SPI
    stop_bor_timer();
#endif

    //Enable LED to drain excess energy
    PJOUT |= BIT6;

    return 0;
}
