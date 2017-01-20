/*
 * main.c
 *
 */

#include <msp430.h>
#include <stdint.h>
#include "eusci_b0_i2c.h"

#define MCF 8000000 // Hz
#define FREQ 20
#define DUTY 0.25

// Note: INFOD has a length of 128 bytes
struct NVvar {
    uint8_t cp;
    uint8_t cnt;
    uint8_t len;
    uint8_t inst[100];
};

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

void drv_init() {

    i2c_init();

    // enable TCA9539
    P1OUT |= BIT4;

    // make all outputs low
    i2c_transmit(0x02, 0x00);   //0x01 for TCA9538
    _delay_cycles(800);         // 100us delay;

    //configure output
    i2c_transmit(0x06, 0x00);   //0x03 for TCA9538
    _delay_cycles(800);         // 100us delay;
}

void prep_inst(uint8_t cmd, uint8_t len) {

    const uint8_t lstates[4] = {0x02, 0x08, 0x03, 0x0C};  // P0 = DA, P1 = PA, P2 = DB, P3 = PB
    const uint8_t rstates[4] = {0x0C, 0x03, 0x08, 0x02};  // P4 = DA, P1 = PA, P2 = DB, P3 = PB
    uint8_t cnt = 0;
    int8_t lstate = 0;
    int8_t rstate = 0;
    int8_t ladd = 0;
    int8_t radd = 0;


    switch(cmd) {
       case 0x01 : // forward
           ladd = 1;
           radd = 1;
           break;
       case 0x02 : // left (hard)
           ladd = -1;
           radd = 1;
           break;
       case 0x03 : // right (hard)
           ladd = 1;
           radd = -1;
           break;
       case 0x04 : // reverse
           ladd = -1;
           radd = -1;
           break;
       /* case 0x05 : // left (soft forward)

           break;
       case 0x06 : // right (soft forward)

           break;
       case 0x07 : // left (soft reverse)

           break
       case 0x08 : // right (soft reverse)

           break;
       default : */

    }

    while(cnt < len) {
        lstate = lstate + ladd;
        rstate = rstate + radd;
        if (lstate > 3) {
            lstate = 0;
        } else if (lstate < 0) {
            lstate = 3;
        }
        if (rstate > 3) {
            rstate = 0;
        } else if (rstate < 0) {
            rstate = 3;
        }
        fram->inst[cnt] = (lstates[lstate] | (rstates[rstate] << 4));
        cnt++;
    }
    fram->len = len;
    fram->cp++;
}

void drive_motors() {

    while (fram->cnt < fram->len) {

        P4OUT |= BIT0;
        // enable both motor drivers
        P3OUT |= (BIT4 | BIT5);
        i2c_transmit(0x02, fram->inst[fram->cnt]); //0x01 for TCA9538

        __delay_cycles((MCF/FREQ) * DUTY);
        fram->cnt++;

        P4OUT &= ~BIT0;
        i2c_transmit(0x02, 0x00); //0x01 for TCA9538
        // disable both motor drivers
        P3OUT &= ~(BIT4 | BIT5);

        __delay_cycles((MCF/FREQ) * (1-DUTY));
    }
}

int main(void) {

    const uint16_t stm = 20;

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
