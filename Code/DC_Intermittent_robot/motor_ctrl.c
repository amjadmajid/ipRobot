/*
 * motor_ctrl.c
 *
 *  Created on: 7 jul. 2017
 *      Author: Koen Schaper
 */

#include <msp430.h>
#include <stdint.h>
#include "motor_ctrl.h"
#include "eusci_b0_i2c.h"

void drv_init() {

    P1DIR |= BIT4;                      // Set P1.4 (AUX3) to output
    P1OUT |= BIT4;                      // Enable TCA9538

    P3DIR |= BIT4 | BIT5;                      // Only enable DRV2  // Set P3.4 and P3.5 (AUX1 and AUX2) to output
    P3OUT &= ~(BIT4 |BIT5);                   //                   // Disable both motor drivers

    // make all outputs low
    i2c_write(TCA_ADDR, 0x01, 0x00);   //0x02 for TCA9539

    //configure output
    i2c_write(TCA_ADDR, 0x03, 0x00);   //0x06 for TCA9539


    //configure motor ctrl(Timer0_A0)
    TA0CCTL0 = CCIE;                                // TACCR0 interrupt enabled
    TA0CCR0 = 2000;                                 // PWM frequency = 1kHz
    //configure motor ctrl (Timer0_A1)
    TA0CCTL1 = CCIE;
    TA0CCR1 = 400;                                 // Dutycycle = 50% (cannot be smaller than 400)

    TA0CTL = TASSEL__SMCLK | MC__UP | TACLR;        // SMCLK, up mode, clear TAR

    __bis_SR_register(GIE);            //enable interrupts */
}

void drv_mot() {

    // enable right motor driver
    P3OUT |= (BIT4 | BIT5);
    // backwards
    i2c_write(TCA_ADDR, 0x01, 0xAA);
    // forward
    //i2c_write(TCA_ADDR, 0x01, 0xFF);
}

void dsbl_mot() {

    i2c_write(TCA_ADDR, 0x01, 0x00);
    // disable right motor driver
    P3OUT &= ~(BIT4 | BIT5);
}

void dsbl_tim(){
    TA0CCTL0 &= ~CCIE;
    TA0CCTL1 &= ~CCIE;
    dsbl_mot();
}

// Timer0_A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer0_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    drv_mot();
}

// Timer0_A1 Interrupt Vector (TAIV) handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A1_VECTOR))) TIMER0_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(TA0IV, TA0IV_TAIFG))
  {
    case TA0IV_NONE:   break;               // No interrupt
    case TA0IV_TACCR1:
        dsbl_mot();
        break;
    case TA0IV_TACCR2: break;               // CCR2
    case TA0IV_3:      break;               // reserved
    case TA0IV_4:      break;               // reserved
    case TA0IV_5:      break;               // reserved
    case TA0IV_6:      break;               // reserved
    case TA0IV_TAIFG:  break;               // overflow
    default: break;
  }
}

