/*
 * motor_ctrl.c
 *
 *  Created on: 7 jul. 2017
 *      Author: Koen Schaper
 */

#include <msp430.h>
#include <stdint.h>
#include "global.h"
#include "motor_ctrl.h"

void drv_init() {

    P2DIR |= BIT1;                            // Set P2.1 (UART_RX) to output
    P2OUT &= ~(BIT1);                         // Disable DRV8836

    //Motor driver config
    P1DIR |= BIT4;                            // Setup AIN1 (TB01) (M1 BLUE)
    P1SEL0 |= BIT4;                           // Option select timer output

    P3DIR |= BIT4;                            // Setup AIN2 (TB03) (M1 RED)
    P3SEL0 |= BIT4;                           // Option select timer output

    P3DIR |= BIT5;                            // Setup BIN1 (TB04) (M2 RED)
    P3SEL0 |= BIT5;                           // Option select timer output

    P2DIR |= BIT0;                            // Setup BIN2 (TB06) (M2 BLUE)
    P2SEL0 |= BIT0;                           // Option select timer output

    TB0CCR0 = 1000;                           // PWM Period (2khz)
    TB0CTL = TBSSEL__SMCLK;                   // SMCLK, up mode, clear TBR
}


void enbl_mot() {
    P2OUT |= BIT1;                            // Enable DRV8836
}

void dsbl_mot(){
    P2OUT &= ~(BIT1);
    TB0CTL |= MC__STOP;
}

void forward(){

    TB0CTL |= MC__STOP;

    //Set the non pwm output to zero
    TB0CCTL3 = 0x00;
    TB0CCTL6 = 0x00;

    //Left motor
    TB0CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TB0CCR1 = 200;
    //Right motor
    TB0CCTL4 = OUTMOD_7;                      // CCR4 reset/set
    TB0CCR4 = 270;

    TB0CTL |= MC__UP | TBCLR;
}

void reverse(){

    TB0CTL |= MC__STOP;

    //Set the non pwm output to zero
    TB0CCTL1 = 0x00;
    TB0CCTL4 = 0x00;

    //Left motor
    TB0CCTL3 = OUTMOD_7;                      // CCR3 reset/set
    TB0CCR3 = 200;                            // CCR3 PWM duty cycle left motor
    //Right motor
    TB0CCTL6 = OUTMOD_7;                      // CCR6 reset/set
    TB0CCR6 = 270;

    TB0CTL |= MC__UP | TBCLR;
}
