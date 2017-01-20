/*
 * main.c
 *
 */

#include <msp430.h>
#include <stdint.h>
#include "eusci_b0_i2c.h"

#define NUM_OF_STATES 4
#define MCF 8000000 // Hz
#define FREQ 20
#define DUTY 0.25

// Note: INFOD has a length of 128 bytes
struct NVvar {
    uint8_t cp;
    uint8_t state;
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

    const uint8_t states_left[NUM_OF_STATES] = {0x02, 0x08, 0x03, 0x0C};  // P0 = DA, P1 = PA, P4 = DB, P3 = PB
    const uint8_t states_right[NUM_OF_STATES] = {0x0C, 0x03, 0x08, 0x02}; // P4 = DA, P5 = PA, P6 = DB, P7 = PB
    const uint16_t stm = 20;

    if( fram->cp == 0x00 ){
        fram->cnt = 0;
        fram->state = 0;
        fram->cp = 0x01;
    }

    init();
    i2c_init();

    // enable TCA9539
    P1OUT |= BIT4;

    // make all outputs low
    i2c_transmit(0x02, 0x00);   //0x01 for TCA9538
    _delay_cycles(800);         // 100us delay;

    //configure output
    i2c_transmit(0x06, 0x00);   //0x03 for TCA9538
    _delay_cycles(800);         // 100us delay;

    //prepaire array

    if( fram->cp == 0x01 ) {
        fram->len = stm;
        while(fram->cnt < fram->len) {
            if (fram->state > (NUM_OF_STATES -1)){
                fram->state = 0;
            }
            fram->inst[fram->cnt] = (states_left[fram->state] | (states_right[fram->state] << 4));
            fram->state++;
            fram->cnt++;
        }
        fram->cp++;
        fram->cnt = 0;
    }

    drive_motors();

    fram->cp = 0x00;

    return 0;
}
