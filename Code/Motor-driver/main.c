/*
 * main.c
 *
 */

#include <msp430.h>
#include <stdint.h>
#include "global.h"
#include "eusci_b0_i2c.h"
#include "motor_ctrl.h"
#include "gyro_sens.h"
#include "prox_sens.h"
#include "particle_filter.h"

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
    CSCTL1 = DCOFSEL_6;                     // Set DCO to 8MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__8 | DIVM__1;   // Set all dividers
    CSCTL0_H = 0;                           // Lock CS registers

    P2DIR &= ~(BIT1);                       // Set P2.1 (UART_RX) to input
    P2OUT |= BIT1;                          // Set pull up resistor on input
    P2REN |= BIT1;                          // Enable pull up resistor for button to keep pin high until pressed
}

int main(void) {

    const uint8_t len = 1;
    const uint8_t inst_cmd[1] = {0x01}; // 0x03, 0x01, 0x03, 0x01, 0x03, 0x01, 0x03};
    const uint8_t inst_len[1] = {220}; //, 10, 50, 10, 50, 10, 50, 10};

    init();

    while(1) {

        if(fram->cp_nr == 0x00) {
            while(1) {
                if((P2IN & BIT1)==0){
                    __delay_cycles(16000000);
                    break;
                }
            }
            fram->inst = 0;
            pi_or->cnt = 0;
            fram->cp_nr = 0x01;
        }

        if(!DEBUG){
            i2c_init();
            drv_init();
            gyro_init();
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
