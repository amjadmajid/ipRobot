/*
 * ctrl_loop.c
 *
 *  Created on: 8 aug. 2017
 *      Author: Koen Schaper
 */

#include <msp430.h>
#include <stdint.h>
#include <math.h>
#include "global.h"
#include "ctrl_loop.h"
#include "eusci_b0_i2c.h"
#include "gyro_sens.h"
#include "motor_ctrl.h"

#ifdef DEBUG
#pragma PERSISTENT(sensor_data);
int16_t sensor_data[400] = {0};
#endif

uint8_t curr_cmd;

int16_t lspeed = 0;
int16_t rspeed = 0;

// run set_setpoint()
float set = 0;

// run set_tunings()
float kp = 0;
float ki = 0;
float kd = 0;

// run set_limits()
float out_max = 75, out_min = -75;

float iterm = 0;
float prev = 0;

uint16_t num_loops = 0;
float ang = 0;

void ctrl_init(){

    // assume i2c is already initialized
    gyro_init();
    drv_init();

    TA2CCR0 = 125000 * SAMPLE_TIME;           // Set timer frequency
    TA2CTL = TASSEL__SMCLK | ID__8 | MC__UP;  // SMCLK, divide by 8, UP mode

    __bis_SR_register(GIE);                   // Enable interrupt
}

void set_setpoint(float sp){
    set = sp;
}

// allow changing of tuning parameters for the different commands
void set_tunings(float Kp, float Ki, float Kd){
    kp = Kp;
    ki = Ki * SAMPLE_TIME;
    kd = Kd / SAMPLE_TIME;
}

void set_limits(float max, float min){
    out_max = max;
    out_min = min;
}

/*
 * Move, 0x01 straight, 0x02 turn left, 0x03 turn right
 * arg : 0x01 length in cm, 0x02 ang in deg, 0x03 ang in deg
 */
void move(uint8_t cmd, int16_t arg){
    curr_cmd = cmd;

    switch(cmd) {
        case STRAIGHT:
            set_tunings(0.13*0.6, (20/100)/2, (20/100)/8);
            num_loops = (uint16_t)((float)arg / VEL_CAL / SAMPLE_TIME);
            set_limits(SMAX, -SMAX);
            fram.cnt -= STEP_OFF;                     // Extra steps on startup!
            lspeed = MOT_TRG;
            rspeed = MOT_TRG;
            enbl_mot();
            TA2CCTL0 = CCIE;                          // TACCR0 interrupt enabled
            break;
        case TURN_LEFT:
            set_tunings(0.75, 0, 0);
            set_setpoint(arg);
            set_limits(SMAX, -SMAX);
            ang = fram.ang;                           // Always restore angle progress
            enbl_mot();
            TA2CCTL0 = CCIE;                          // TACCR0 interrupt enabled
            break;
        case TURN_RIGHT:
            set_tunings(0.35, 0, 0);
            set_setpoint(-arg);
            set_limits(SMAX, -SMAX);
            ang = fram.ang;                           // Always restore angle progress
            enbl_mot();
            TA2CCTL0 = CCIE;                          // TACCR0 interrupt enabled
            break;
       //default : /* Optional */
       //statement(s);
    }
}

void dsbl_loop(){
    dsbl_mot();
    TA2CCTL0 &= ~CCIE;
    set = 0;                                  // Always return set to 0 (straight)
    iterm = 0;
    prev = 0;
    fram.cnt = 0;
    fram.ang = 0;
    fram.stop = 1;
}

// compute PID output
static inline float pid_compute(float input){
    float err, dinp, output;
    err = set - input;
    iterm += (ki * err);
    if(iterm > out_max)
        iterm = out_max;
    else if(iterm < out_min)
        iterm = out_min;
    dinp = input - prev;
    output = kp*err + iterm + kd*dinp;
    /*if(output > out_max)
        output = out_max;
    else if(output < out_min)
        output = out_min;*/
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
    float omega, turn;
    data = gyro_read();
    omega = data / 32.767;
    if(curr_cmd == STRAIGHT){
#if DEBUG
        sensor_data[fram.cnt] = (int16_t)omega;
#endif
        if(fram.cnt >= num_loops | fram.stop)
            dsbl_loop();
        turn = pid_compute(omega);
        lspeed = lspeed - (int16_t)turn;
        rspeed = rspeed + (int16_t)turn;
//#if DEBUG==0
        fram.cnt++;
//#endif
    }else if(curr_cmd == TURN_LEFT || curr_cmd == TURN_RIGHT){
        ang += omega * SAMPLE_TIME; // integrate to get the angle
        fram.ang = ang;             // save the current angle
#if DEBUG
        sensor_data[fram.cnt] = (int16_t)ang;
#endif
        if(fabs(set - ang) < TOLERANCE_DEGREES){
            dsbl_loop();
            return;
        }
        turn = pid_compute(ang);
        lspeed = -(int16_t)turn;
        rspeed = +(int16_t)turn;
    }
    drv_mot(lspeed, rspeed);
#if DEBUG
    fram.cnt++;
#endif
}
