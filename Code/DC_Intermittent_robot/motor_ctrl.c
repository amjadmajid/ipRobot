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

uint16_t cnt = 0;
uint8_t test = 1;

void drv_init() {

    P1DIR |= BIT4;                      // Set P1.4 (AUX3) to output
    P1OUT |= BIT4;                      // Enable TCA9538

    P3DIR |= BIT4 | BIT5;              // Set P3.4 and P3.5 (AUX1 and AUX2) to output
    P3OUT &= ~(BIT4 |BIT5);            // Disable both motor drivers

    // make all outputs low
    i2c_write(TCA_ADDR, 0x01, 0x00);   //0x02 for TCA9539

    //configure output
    i2c_write(TCA_ADDR, 0x03, 0x00);   //0x06 for TCA9539


    //configure motor ctrl(Timer0_A0)
    TA0CCTL0 = CCIE;                                // TACCR0 interrupt enabled
    TA0CCR0 = 2000;                                 // PWM frequency = 1kHz
    //configure motor ctrl (Timer0_A1)
    TA0CCTL1 = CCIE;
    TA0CCR1 = 400;                                  // Dutycycle = 10% (cannot be smaller than 200 and not bigger than 1800)

    TA0CCTL2 = CCIE;
    TA0CCR2 = 600;

    TA0CTL = TASSEL__SMCLK | MC__UP | TACLR;        // SMCLK, up mode, clear TAR

    __bis_SR_register(GIE);            //enable interrupts
}

void drv_mot() {

    // enable right motor driver
    P3OUT |= (BIT4);
    //i2c_write(TCA_ADDR, 0x01, 0x08); //0x0A
    //i2c_write(TCA_ADDR, 0x01, 0x0B); //Dual motor
    // forward
    //i2c_write(TCA_ADDR, 0x01, 0x0C); // 0x0F to drive both motors
    i2c_write(TCA_ADDR, 0x01, 0x0E); // Dual motor
}

void dsbl_lmot() {

    i2c_write(TCA_ADDR, 0x01, 0x02);
}

void dsbl_mot() {

    i2c_write(TCA_ADDR, 0x01, 0x00);
    // disable right motor driver
    P3OUT &= ~(BIT4);
}

void dsbl_tim(){
    TA0CCTL0 &= ~CCIE;
    TA0CCTL1 &= ~CCIE;
    TA0CCTL2 &= ~CCIE;
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
        /* ramp pwm in 160 steps 10% -> 90% (200 -> 1800)
        *  over a time of 160ms
        */
        /*if(cnt < 40){
            TA0CCR1 += 10;
            cnt++;
        }*/
        if(test == 1){
            dsbl_lmot();
            test = 0;
        } else {
            test = 1;
        }
        break;
    case TA0IV_TACCR2:                      // CCR2
        dsbl_mot();
        break;
    case TA0IV_3:      break;               // reserved
    case TA0IV_4:      break;               // reserved
    case TA0IV_5:      break;               // reserved
    case TA0IV_6:      break;               // reserved
    case TA0IV_TAIFG:  break;               // overflow
    default: break;
  }
}

