/*
 * bor_timer.c
 *
 *  Created on: 4 okt. 2017
 *      Author: Koen Schaper
 */

#include <msp430.h>
#include <stdint.h>
#include "bor_timer.h"

// From http://processors.wiki.ti.com/index.php/MSP430_FAQ
#define SW_RESET()      PMMCTL0 = PMMPW + PMMSWBOR + (PMMCTL0 & 0x0003);  // software BOR reset

void start_bor_timer(uint16_t time_ms){

    TA2CCR0 = 9.4 * time_ms;         // Set timer frequency
    TA2CCTL0 = CCIE;                 // TACCR0 interrupt enabled
    TA2CTL = TASSEL__ACLK | MC__UP;  // SMCLK, UP mode

    __bis_SR_register(GIE);          // Enable interrupt
}

void stop_bor_timer(){

    TA2CCTL0 &= ~CCIE;
}

// Timer2_A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER2_A0_VECTOR
__interrupt void Timer2_A0_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER2_A0_VECTOR))) Timer2_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    SW_RESET();
}
