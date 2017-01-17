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

    P3DIR |= BIT4 | BIT5;                   // Set P3.4 and P3.5 (AUX1 and AUX2) to output
    P3OUT &= ~(BIT4 | BIT5);                // Disable both motor drivers
}

int main(void) {

    struct NVvar * fram = (struct NVvar *) 0x1800;
    const uint8_t states_left[NUM_OF_STATES] = {0x02, 0x08, 0x03, 0x0C};  // P0 = DA, P1 = PA, P4 = DB, P3 = PB
    const uint8_t states_right[NUM_OF_STATES] = {0x0C, 0x03, 0x08, 0x02}; // P4 = DA, P5 = PA, P6 = DB, P7 = PB
    const uint16_t steps_to_move = 20;
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
    i2c_transmit(0x02, 0x00);   //0x01 for TCA9538
    _delay_cycles(800);         // 100us delay;

    //configure output
    i2c_transmit(0x06, 0x00);   //0x03 for TCA9538
    _delay_cycles(800);         // 100us delay;

    while (cnt < steps_to_move) {
        if (next_state > (NUM_OF_STATES -1)){
            next_state = 0;
        }
        P4OUT |= BIT0;
        // enable both motor drivers
        P3OUT |= (BIT4 | BIT5);
        i2c_transmit(0x02, (states_left[next_state] | (states_right[next_state] << 4))); //0x01 for TCA9538

        __delay_cycles((MCF/FREQ) * DUTY);

        P4OUT &= ~BIT0;
        i2c_transmit(0x02, 0x00); //0x01 for TCA9538
        // disable both motor drivers
        P3OUT &= ~(BIT4 | BIT5);

        fram->next_state = ++next_state;
        fram->cnt = ++cnt;

        __delay_cycles((MCF/FREQ) * (1-DUTY));
    }
    fram->started = 0x00;

    return 0;
}
