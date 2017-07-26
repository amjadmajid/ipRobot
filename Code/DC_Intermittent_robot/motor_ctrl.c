/*
 * motor_ctrl.c
 *
 *  Created on: 7 jul. 2017
 *      Author: Koen Schaper
 */

#include <msp430.h>
#include <stdint.h>
#include "global.h"
#include "eusci_b0_i2c.h"
#include "motor_ctrl.h"
//#include "gyro_sens.h"

uint8_t cnt = 0;

void drv_init() {

    P1DIR |= BIT4;                          // Set P1.4 (AUX3) to output
    P1OUT &= ~(BIT4);                       // Disable DRV8836

    //Motor driver config
    P2DIR |= (BIT0 + BIT1);                 // Setup DA and DB
    P2OUT |= (BIT1);                        // Forward
    P2OUT &= ~(BIT0);                       //

    P3DIR |= (BIT4 + BIT5);                 // Setup PA and PB
    P3OUT &= ~(BIT4 + BIT5);

    /*P3SEL0 |= (BIT4 + BIT5);                // Option select timer output

    TB0CCR0 = 2000;                           // PWM Period
    TB0CCTL3 = OUTMOD_7;                      // CCR3 reset/set
    TB0CCR3 = 800;                            // CCR3 PWM duty cycle left motor
    TB0CCTL4 = OUTMOD_7;                      // CCR4 reset/set
    TB0CCR4 = 900;                            // CCR4 PWM duty cycle right motor
    TB0CTL = TBSSEL__SMCLK | MC__UP | TBCLR;  // SMCLK, up mode, clear TBR*/

    /*TB0CCR0 = 20000;                           // PWM Period (1khz)
    TB0CCR3 = 10000;                            // CCR3 PWM duty cycle left motor
    TB0CCR4 = 11000;                            // CCR4 PWM duty cycle right motor
    TB0CTL = TBSSEL__SMCLK | MC__UP | TBCLR;  // SMCLK, up mode, clear TBR, interrupt enable
    */
    //__bis_SR_register(GIE);            //enable interrupts
}

void enbl_mot() {
    TB0CCTL0 |= CCIE;
    TB0CCTL3 |= CCIE;
    TB0CCTL4 |= CCIE;
}

void dsbl_mot(){
    TB0CCTL0 &= ~CCIE;
    TB0CCTL3 &= ~CCIE;
    TB0CCTL4 &= ~CCIE;
    cnt = 0;                                 // To ramp again when the motors are started
}

// Timer0_B0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_B0_VECTOR))) Timer0_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    //if(fram.cnt < RUN_TIME2){
        P1OUT |= (BIT4);                        // Enable DRV8836
        P3OUT |= (BIT4 + BIT5);                 // Enable both motors
        //fram.cnt++;
    //} else{
        //dsbl_mot();
        //fram.cnt = 0;
        //running = 0;
    //}
}

// TimerB Interrupt Vector (TBIV) handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_B1_VECTOR))) TIMER0_B1_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(TB0IV,TB0IV_TBIFG))
  {
    case TB0IV_NONE:   break;               // No interrupt
    case TB0IV_TBCCR1: break;               // CCR1 not used
    case TB0IV_TBCCR2: break;               // CCR2 not used
    case TB0IV_TBCCR3:
        // Ramp PWM
        /*if(cnt < 50){
            TB0CCR3 += 20;
            cnt++;
        }*/
        P3OUT &= ~BIT4;
        break;                              // CCR3 not used
    case TB0IV_TBCCR4:
        // Ramp PWM
        /*if(cnt < 50){
            TB0CCR4 += 20;
            cnt++;
        }*/
        P3OUT &= ~BIT5;
        P1OUT &= ~(BIT4);                   // Disable DRV8836
        break;                              // CCR4 not used
    case TB0IV_TBCCR5: break;               // CCR5 not used
    case TB0IV_TBCCR6: break;               // CCR6 not used
    case TB0IV_TBIFG:  break;               // overflow

    default: break;
  }
}

