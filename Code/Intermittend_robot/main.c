#include <msp430.h> 
#include <stdint.h>
#include "eusci_b0_i2c.h"
#include "motor_ctrl.h"

/*
 * main.c
 */

#define DEBUG 0

typedef struct NVvar {
    uint8_t cp;
    uint8_t cnt_b;
    uint8_t cnt_a;
    //uint8_t lstate;
    //uint8_t rstate;
}NVvar;

#pragma PERSISTENT(fram);
NVvar fram = {0};
//#pragma PERSISTENT(fram_wc);
//NVvar fram_wc = {0};

void swap(NVvar *pa, NVvar *pb){
    NVvar temp = *pa;
    *pa = *pb;
    *pb = temp;
}

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

    //P4DIR |= BIT0;
    //P4OUT &= ~BIT0;
}

int main(void) {

    const uint8_t lstates[4] = {0x0C, 0x03, 0x08, 0x02};  // P0 = DA, P1 = PA, P2 = DB, P3 = PB
    const uint8_t rstates[4] = {0x02, 0x08, 0x03, 0x0C};  // P4 = DA, P1 = PA, P2 = DB, P3 = PB
    const uint8_t num_steps = 212;

    //uint8_t i;

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
            //fram.lstate = 0;
            //fram.rstate = 0;
            fram.cp = 0x01;
        }

        //P4OUT |= BIT0;
        /*for(i=0; i<4; i++){
            drv_mot(lstates[i] | (rstates[i] << 4));
            __delay_cycles(800000);
        }

        __delay_cycles(16000000); */

        if(!DEBUG){
            i2c_init();
            drv_init();
        }

        while(fram.cnt_a < num_steps){

            //fram_wc = fram;
            //fram_wc.cnt =
            fram.cnt_b++;

            if(!DEBUG)
                drv_mot(lstates[(fram.cnt_a % 4)] | (rstates[(fram.cnt_a % 4)] << 4));
                __delay_cycles(DELAY);

            //fram_wc = fram;
            //fram_wc.cnt =
            fram.cnt_a++;
            //fram_wc.lstate = fram.lstate + 1;
            //fram_wc.rstate = fram.rstate + 1;
            //swap(&fram, &fram_wc);
            //__no_operation();
        }
        if(!DEBUG)
            dsbl_mot();
        fram.cp = 0x00;
    }
	
	return 0;
}
