#include <msp430.h> 
#include <stdint.h>
#include "eusci_b0_i2c.h"
#include "motor_ctrl.h"

/*
 * main.c
 */

#define DEBUG 0

#define DELAY 400000

typedef struct NVvar {
    uint8_t cp;
    uint8_t cnt_b;
    uint8_t cnt_a;
}NVvar;

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
    CSCTL3 = DIVA__1 | DIVS__8 | DIVM__1;   // Set all dividers
    CSCTL0_H = 0;                           // Lock CS registers

    P2DIR &= ~(BIT1);                       // Set P2.1 (UART_RX) to input
    P2OUT |= BIT1;                          // Set pull up resistor on input
    P2REN |= BIT1;                          // Enable pull up resistor for button to keep pin high until pressed
}

int main(void) {

    const uint8_t num_steps = 136; // 32cm
    const uint8_t num_cp = 22;
    uint8_t cnt = 0;

    init();

    while(1) {

        if(fram.cp == 0x00) {
            while(1) {
                if((P2IN & BIT1)==0){
                    __delay_cycles(8000000);
                    break;
                }
            }
            fram.cnt_b = 0;
            fram.cnt_a = 0;
            fram.cp = 0x01;
        }

        if(!DEBUG){
            i2c_init();
            drv_init();
        }

        while(fram.cnt_a < num_cp){
            fram.cnt_b++;
            /* Begin "atomic" operation */
            for(cnt=0; cnt < (num_steps / num_cp); cnt++){

                if(!DEBUG){
                    drv_mot();
                    __delay_cycles(DELAY);
                    dsbl_mot();
                }
            }
            /* End "atomic" operation */
            fram.cnt_a++;
        }
        if(!DEBUG)
            dsbl_mot();
        fram.cp = 0x00;
    }

    return 0;
}
