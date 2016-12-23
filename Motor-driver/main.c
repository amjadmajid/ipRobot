/*
 * main.c
 *
 */

#include <msp430.h>
#include <stdint.h>

#define NUM_OF_STATES 4
#define DUTY_CYCLE 4
#define FREQUENCY 40


void init(void) {

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    CSCTL0_H = CSKEY_H;                     // Unlock clock registers
    CSCTL1 = DCOFSEL_3 | DCORSEL;           // Set DCO to 8MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;   // Set all dividers
    CSCTL0_H = 0;                           // Lock CS registers

    P1DIR |= BIT0;							// Set led2 pin to output
    //P4DIR |= BIT6;						// Set led1 pin to output

    P1SEL0 &= ~0x78;						// Set right drv8836 pins to general i/o (pin 3, 4, 5, 6)
    P1DIR |= 0x78;							// Set right drv8836 pins to output

    P3SEL0 &= ~0x71;						// Set left drv8836 pins to general i/o (pin 0, 4, 5, 6)
    P3DIR |= 0x71;							// Set left drv8836 pins to output
}

int main(void) {

	char states_right[NUM_OF_STATES] = {0x11, 0x40, 0x19, 0x60}; // 00010001, 01000000, 00011001, 01100000 (PH/EN) + blink led2
	char states_left[NUM_OF_STATES] = {0x10, 0x40, 0x11, 0x60}; // 00010000, 01000000, 00010001, 01100000
	char next_state = 0;
	int steps_to_move = 20;

	init();

	while (steps_to_move > 0){
		if (next_state > (NUM_OF_STATES -1)){
			next_state = 0;
		}
		P1OUT = states_right[next_state];
		P3OUT = states_left[next_state];

		__delay_cycles(160000);
		// Make all outputs low
		P1OUT &= ~0x78;
		P3OUT &= ~0x71;

		__delay_cycles(480000);

		next_state++;
		steps_to_move--;
	}

	return 0;
}
