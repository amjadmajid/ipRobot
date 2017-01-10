/*
 * main.c
 *
 */

#include <msp430.h>
#include <stdint.h>

#define DATA_SIZE 2
#define SLAVE_ADDR 112 // TCA9538 address pins to GND
#define NUM_OF_STATES 4

void init(void) {

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    CSCTL0_H = CSKEY_H;                     // Unlock clock registers
    CSCTL1 = DCOFSEL_3;                     // Set DCO to 8MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__8 | DIVM__1;   // Set all dividers
    CSCTL0_H = 0;                           // Lock CS registers

    //P1DIR |= BIT0;                          // Set led2 pin to output
    //P1OUT &= ~BIT0;

    P1DIR |= BIT4;                          // Set P1.4 (AUX3) to output
    P1OUT &= ~BIT4;                         // Hold TCA9539 in reset (active low)

    P3DIR |= BIT4 | BIT5;                   // Set P3.4 and P3.5 (AUX1 and AUX2) to output
    P3OUT &= ~(BIT4 | BIT5);                // Disable both motor drivers
}

void i2c_init(){

    P1SEL1 |= BIT6 | BIT7;                  // configure I2C pins
    P1SEL0 &= ~(BIT6 | BIT6);               // configure I2C pins

    // I2C default uses SMCLK
    UCB0CTL1 |= UCSWRST;                    // put eUSCI_B in reset state
    UCB0CTLW0 |= UCMODE_3 | UCMST | UCSYNC; // I2C, master, sync
    UCB0BRW = 0x000A;                       // baud rate = SMCLK / 10 = 100khz
    UCB0CTLW1 = UCASTP_2;                   // automatic STOP assertion
    UCB0TBCNT = DATA_SIZE;                  // TX 2 bytes of data
    UCB0I2CSA = SLAVE_ADDR;                 // slave address
    UCB0CTL1 &= ~UCSWRST;                   // eUSCI_B in operational state
}

void i2c_transmit(uint8_t cmd, uint8_t data){

    UCB0CTLW0 |= UCTR | UCTXSTT;            // transmitter mode and START condition.

    while(!(UCB0IFG & UCTXIFG0));
    UCB0TXBUF = cmd;
    while(!(UCB0IFG & UCTXIFG0));
    UCB0TXBUF = data;
    while(UCB0CTLW0 & UCTXSTP);             // wait for stop
    _delay_cycles(800);
}

int main(void) {

    uint8_t states_left[NUM_OF_STATES] = {0x02, 0x10, 0x03, 0x11};  // P0 = DA, P1 = PA, P4 = DB, P3 = PB
    uint8_t states_right[NUM_OF_STATES] = {0x11, 0x03, 0x10, 0x02}; // P4 = DA, P5 = PA, P6 = DB, P7 = PB
    uint8_t next_state = 0;
    uint8_t steps_to_move = 20;

    init();
    i2c_init();

    // enable TCA9538
    P1OUT |= BIT4;

    //P1OUT |= BIT0;

    // make all outputs low
    i2c_transmit(0x01, 0x00);

    //configure output
    i2c_transmit(0x03, 0x00);

    while (steps_to_move > 0){
        if (next_state > (NUM_OF_STATES -1)){
            next_state = 0;
        }
        // enable both motor drivers
        P3OUT |= (BIT4 | BIT5);
        i2c_transmit(0x01, (states_left[next_state] | (states_right[next_state] << 4)));

        __delay_cycles(160000);

        // disable both motor drivers
        P3OUT &= ~(BIT4 | BIT5);
        i2c_transmit(0x01, 0x00);

        __delay_cycles(480000);

        next_state++;
        steps_to_move--;
    }
    //P1OUT &= ~BIT0;

    return 0;
}
