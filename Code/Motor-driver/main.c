/*
 * main.c
 *
 */

#include <msp430.h>
#include <stdint.h>
#include <stdlib.h>
#include "global.h"

#ifndef __BSD_VISIBLE
#define __BSD_VISIBLE
#endif
#include <math.h>

#include "eusci_b0_i2c.h"
#include "motor_ctrl.h"
#include "prox_sens.h"
#include "const_map.h"
#include "particle_filter.h"
#include "msp_lib.h"

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

    /* inst len should be <= 4 */
    /*const uint8_t len = 16;
    const uint8_t inst_cmd[16] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}; // 0x03, 0x01, 0x03, 0x01, 0x03, 0x01, 0x03};
    const uint8_t inst_len[16] = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4}; //, 10, 50, 10, 50, 10, 50, 10}; */

    float temp = 1;
    float ang = 0;
    uint8_t len = 25;
    uint8_t cnt;
    uint8_t i;
    uint8_t prox;
    const uint8_t num_part = 100;
    const uint8_t prox_thres = 50;
    const uint8_t step_size  = 4;

    init();

    while(1) {

        if(fram->cp_nr == 0x00) {
            while(1) {
                if((P2IN & BIT1)==0){
                    __delay_cycles(8000000);
                    break;
                }
            }
            const_map_init();
            part_init(num_part);
            fram->inst = 0;
            //pi_or->cnt = 0;
            cnt = 0;
            fram->cp_nr = 0x01;
        }

        if(!DEBUG){
            i2c_init();
            prox_init();
            drv_init();
        }

        for(cnt=0; cnt < len; cnt++){

            prox = prox_read();

            if(prox < prox_thres){
                for(i=0; i < step_size; i++){
                    prep_inst(0x01); // forward
                }
                ang = 0;
            }
            else{
                for(i=0; i < 13; i++){
                    prep_inst(0x03); // turn right
                }
                for(i=0; i < step_size; i++){
                    prep_inst(0x01); // forward
                }
                ang = 0.5 *M_PI;
            }

            temp = move(0.2355 * 2 * step_size, ang);
            temp = update(temp);
            if(temp < 0.5*num_part){
                resample();
            }
            //fram->inst++;
        }

        if(!DEBUG)
            dsbl_mot();

        fram->cp_nr = 0x00;
    }

    return 0;
}
