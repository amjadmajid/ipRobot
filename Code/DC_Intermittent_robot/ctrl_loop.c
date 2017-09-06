/*
 * ctrl_loop.c
 *
 *  Created on: 8 aug. 2017
 *      Author: Koen Schaper
 */

#include <msp430.h>
#include <stdint.h>
#include "global.h"
#include "ctrl_loop.h"
#include "eusci_b0_i2c.h"
#include "gyro_sens.h"
#include "motor_ctrl.h"

// Ziegler Nichols PID tuning parameters
#define Ku 0.6
#define Tu 0.24 // 12/50
#define st 0.02 // Sample time (50Hz)

#pragma PERSISTENT(sensor_data);
int16_t sensor_data[400] = {0};

uint16_t lspeed = 600;
uint16_t rspeed = 600;

float Kp = 0.6 * Ku;
float Ki = 0; //(Tu / 2) * st; // (Tu / 1.2) * st;
float Kd = 0; //(Tu / 8) / st;

int16_t data = 0;
float set = 0, omega = 0, err = 0;
float ierr = 0;
float prev = 0, derr = 0;

void ctrl_init(){

    // assume i2c is already initialized
    gyro_init();
    drv_init();

    TA2CCR0 = 188;                             // Set timer to approx. 50Hz
    TA2CTL = TASSEL__ACLK | MC__UP;           // ACLK = VLO, UP mode

    __bis_SR_register(GIE);                   // Enable interrupt
}

void enbl_loop(uint16_t dur){
    enbl_mot();
    mot_for(lspeed, rspeed);
    TA2CCTL0 = CCIE;                          // TACCR0 interrupt enabled
}

void upd_set(float s){
    set = s;
}

void dsbl_loop(){
    TA2CCTL0 &= ~CCIE;
    dsbl_mot();
    set = 0;                                  // Always return set to 0 (straight)
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
    data = -gyro_read();
    omega = data / 131.0;
    err = set - omega;
    ierr += err;
    derr = err - prev;
    if(fram.cnt < 100 && RAMP){
        lspeed += 5;
        rspeed += 5;
    }
    rspeed = rspeed - (Kp*err + Ki*ierr + Kd*derr);
    set_for_sp(lspeed, rspeed);
    sensor_data[fram.cnt] = data;
    prev = err;
    fram.cnt++;
}
