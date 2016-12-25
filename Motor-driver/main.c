/*
 * main.c
 *
 */

#include <msp430.h>
#include <stdint.h>

#define DATA_SIZE 2
#define SLAVE_ADDR 116 // TCA9539 address pins to GND
#define NUM_OF_STATES 4

void init(void) {

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    CSCTL0_H = CSKEY_H;                     // Unlock clock registers
    CSCTL1 = DCOFSEL_3 | DCORSEL;           // Set DCO to 8MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__8 | DIVM__1;   // Set all dividers
    CSCTL0_H = 0;                           // Lock CS registers

    P1DIR |= BIT0;                          // Set led2 pin to output

    P1DIR |= BIT4;                          // Set P1.4 (AUX3) to output
    P1OUT &= ~BIT4;                         // Hold TCA9539 in reset (active low)
}

void i2c_init(){

    // I2C default uses SMCLK
    UCB0CTL1 |= UCSWRST;                    // put eUSCI_B in reset state
    UCB0CTLW0 |= UCMODE_3 | UCMST | UCSYNC; // I2C, master, sync
    UCB0BRW = 0x000A;                       // baud rate = SMCLK / 10 = 100khz
    UCB0CTLW1 = UCASTP_2;                   // automatic STOP assertion
    UCB0TBCNT = DATA_SIZE;                  // TX 2 bytes of data
    UCB0I2CSA = SLAVE_ADDR;                 // slave address

    P1SEL1 |= BIT6 | BIT7;                  // configure I2C pins
    P1SEL0 &= ~(BIT6 | BIT6);               // configure I2C pins

    UCB0CTL1 &= ~UCSWRST;                   // eUSCI_B in operational state
}

void i2c_transmit(uint8_t cmd, uint8_t data){

    UCB0CTLW0 |= UCTR + UCTXSTT;            // transmitter mode and START condition.

    while(!(UCB0IFG & UCTXIFG0));
    UCB0TXBUF = cmd;
    while(!(UCB0IFG & UCTXIFG0));
    UCB0TXBUF = data;
    while(UCB0CTLW0 & UCTXSTP);             // wait for stop
}

int main(void) {

    uint8_t states_right[NUM_OF_STATES] = {0x88, 0xA0, 0x89, 0xE0}; // P13 = PA, P14 = DA, P15 = PB, P16 = DB, P17 = SL
    uint8_t states_left[NUM_OF_STATES] = {0x22, 0x28, 0x26, 0x38};  // P1 = PA, P2 = DA, P3 = PB, P4 = DB, P5 = SL
    uint8_t next_state = 0;
    uint8_t steps_to_move = 20;

    init();
    i2c_init();

    // enable TCA9539
    P1OUT |= BIT4;

    // make all outputs low
    i2c_transmit(0x02, 0x00);
    i2c_transmit(0x03, 0x00);

    //configure output
    i2c_transmit(0x05, 0xE1); // ~0x1E
    i2c_transmit(0x06, 0x0F); // ~0xF0

    while (steps_to_move > 0){
        if (next_state > (NUM_OF_STATES -1)){
            next_state = 0;
        }
        i2c_transmit(0x02, states_left[next_state]);
        i2c_transmit(0x03, states_right[next_state]);

        __delay_cycles(160000);

        // Make all outputs low
        i2c_transmit(0x02, 0x00);
        i2c_transmit(0x03, 0x00);

        __delay_cycles(480000);

        next_state++;
        steps_to_move--;
    }

    return 0;
}
