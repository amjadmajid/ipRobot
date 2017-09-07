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
#define ku 0.6
#define tu 0.24 // 12/50
#define st 0.02 // Sample time (50Hz)

#pragma PERSISTENT(sensor_data);
int16_t sensor_data[400] = {0};

uint16_t lspeed = 250;
uint16_t rspeed = 250;

float kp = 0.6 * ku;
float ki = 0; //(tu / 2); // (tu / 1.2);
float kd = 0; //(tu / 8);

float out_max = 250, out_min = -250;

float ang = 0;
float set = 0;
float iterm = 0;
float prev = 0;

void ctrl_init(){

    // assume i2c is already initialized
    gyro_init();
    drv_init();

    TA2CCR0 = 188;                            // Set timer to approx. 50Hz
    TA2CTL = TASSEL__ACLK | MC__UP;           // ACLK = VLO, UP mode

    __bis_SR_register(GIE);                   // Enable interrupt
}

void enbl_loop(uint16_t dur){
    enbl_mot();
    mot_for(lspeed, rspeed);
    TA2CCTL0 = CCIE;                          // TACCR0 interrupt enabled
}

void dsbl_loop(){
    TA2CCTL0 &= ~CCIE;
    dsbl_mot();
    set = 0;                                  // Always return set to 0 (straight)
}

// allow changing of tuning parameters for the different commands
void set_tunings(float Kp, float Ki, float Kd){
    kp = Kp;
    ki = Ki * st;
    kd = Kd / st;
}

void set_limits(float max, float min){
    out_max = max;
    out_min = min;
}

// compute PID output
float pid_compute(float input){
    float err, dinp, output;
    err = set - input;
    iterm += (ki * err);
    if(iterm > out_max)
        iterm = out_max;
    else if(iterm < out_min)
        iterm = out_min;
    dinp = input - prev;
    output = kp*err + iterm + kd*dinp;
    if(output > out_max)
        output = out_max;
    else if(output < out_min)
        output = out_min;
    prev = input;
    return output;
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
    int16_t data;
    float omega;
    data = gyro_read();
    omega = data / 131.0;
    ang += omega * st; // integrate to get the angle
    rspeed = rspeed - (int16_t)pid_compute(ang);
    set_for_sp(lspeed, rspeed);
    sensor_data[fram.cnt] = data;
    fram.cnt++;
}
