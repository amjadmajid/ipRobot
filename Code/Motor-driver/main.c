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

struct NVvar {
    uint8_t started;
    uint16_t cnt;
    uint8_t next_state;
};

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
}

int main(void) {

    struct NVvar * fram = (struct NVvar *) 0x1800;
    uint8_t states_right[NUM_OF_STATES] = {0x90, 0xC0, 0x98, 0xE0}; // P13 = DA, P14 = PA, P15 = DB, P16 = PB, P17 = SL
    uint8_t states_left[NUM_OF_STATES] = {0x24, 0x30, 0x26, 0x38};  // P1 = DA, P2 = PA, P3 = DB, P4 = PB, P5 = SL
    uint16_t steps_to_move = 20;
    uint16_t cnt;
    uint8_t next_state;

    if( fram->started != 0x02 ){
        cnt = 0;
        next_state = 0;
        fram->started = 0x02;
    } else{
        cnt = fram->cnt;
        next_state = fram->next_state;
    }

    init();
    i2c_init();

    // enable TCA9539
    P1OUT |= BIT4;

    // make all outputs low
    i2c_transmit(0x02, 0x00);
    i2c_transmit(0x03, 0x00);

    //configure output
    i2c_transmit(0x06, 0xC1); // ~0x1E
    i2c_transmit(0x07, 0x07); // ~0xF0

    while (cnt < steps_to_move) {
        if (next_state > (NUM_OF_STATES -1)){
            next_state = 0;
        }
        P4OUT |= BIT0;
        i2c_transmit(0x02, states_left[next_state]);
        i2c_transmit(0x03, states_right[next_state]);

        __delay_cycles((MCF/FREQ) * DUTY);

        // Make all outputs low
        P4OUT &= ~BIT0;
        i2c_transmit(0x02, 0x00);
        i2c_transmit(0x03, 0x00);

        next_state++;
        fram->next_state = next_state;
        cnt++;
        fram->cnt = cnt;

        __delay_cycles((MCF/FREQ) * (1-DUTY));

    }
    fram->started = 0x00;

    return 0;
}
