/*
 * ctrl_loop.c
 *
 *  Created on: 8 aug. 2017
 *      Author: Koen Schaper
 */

#include <msp430.h>
#include <stdint.h>
// include mathematical constants from math.h
#define __BSD_VISIBLE
#include <math.h>
#include "global.h"
#include "ctrl_loop.h"
#include "eusci_b0_i2c.h"
#include "gyro_sens.h"
#include "motor_ctrl.h"

#if DEBUG
#pragma PERSISTENT(sensor_data);
int16_t sensor_data[400] = {0};
#endif

uint8_t curr_cmd;

#if RAMP
uint8_t rcnt = 19;
uint8_t ramp = 3;
#else
uint8_t ramp = 0;
#endif
int16_t lspeed = 1;
int16_t rspeed = 1;

// run set_setpoint()
float set = 0;
float set2 = 0;

// run set_tunings()
float kp = 0, ki = 0, kd = 0;

// run set_limits()
float out_max = 75, out_min = -75;

float iterm = 0;
float prev = 0;

uint16_t num_loops = 0;
float ang = 0;

robot_conf curr_conf;

void ctrl_init(robot_conf set_conf){

    curr_conf = set_conf;

    // assume i2c is already initialized
    gyro_init();
    drv_init(curr_conf.mc);

    TA3CCR0 = 125000 * SAMPLE_TIME;           // Set timer frequency
    TA3CTL = TASSEL__SMCLK | ID__8 | MC__UP;  // SMCLK, divide by 8, UP mode

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
    uint8_t r = 30;
    switch(cmd) {
        case STRAIGHT:
        case CURVE_LEFT:
        case CURVE_RIGHT:
            set_tunings(curr_conf.st.Kp, curr_conf.st.Ki, curr_conf.st.Kd);
            if(curr_cmd == CURVE_LEFT){
                set_setpoint((VEL_CAL / r)*(180 / M_PI));
                set2 = arg;
            }
            else if(curr_cmd == CURVE_RIGHT){
                set_setpoint(-(VEL_CAL / r)*(180 / M_PI));
                set2 = -arg;
            }
            else{ //STRAIGHT
                set_setpoint(0);
#if DEBUG
                num_loops = 200; }
#else
                num_loops = arg;//(uint16_t)((float)arg / VEL_CAL / SAMPLE_TIME);
                }
#endif
#if !RAMP
            lspeed = MOT_TRG;
            rspeed = MOT_TRG;
#endif
            break;
        case TURN_LEFT:
        case TURN_RIGHT:
            set_tunings(curr_conf.tr.Kp, curr_conf.tr.Ki, curr_conf.tr.Kd);
            if(curr_cmd == TURN_LEFT)
                set_setpoint(arg);
            else // TURN_RIGHT
                set_setpoint(-arg);
            ang = fram.ang;                           // Always restore angle progress
            break;
        //default : /* Optional */
    }
    set_limits(curr_conf.mc.smax, -curr_conf.mc.smax);
    enbl_mot();
    TA3CCTL0 = CCIE;                          // TACCR0 interrupt enabled
}

void dsbl_loop(){
    dsbl_mot();
    TA3CCTL0 &= ~CCIE;
    set = 0;                                  // Always return set to 0 (straight)
    iterm = 0;
    prev = 0;
    // Double buffer to keep consistency
    fram_wc = fram;
    fram_wc.cnt = 0;
    fram_wc.ang = 0;
    fram_wc.stop = 1;
    swap(&fram, &fram_wc);
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

// Timer3_A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER3_A0_VECTOR
__interrupt void Timer3_A0_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER3_A0_VECTOR))) Timer3_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    int16_t data;
    float omega, turn;
    data = gyro_read();
    omega = data / 32.767;
    ang += omega * SAMPLE_TIME; // integrate to get the angle
    fram.ang = ang;             // save the current angle
    switch(curr_cmd) {
        case STRAIGHT:
#if DEBUG
        sensor_data[fram.cnt] = (int16_t)omega;
#endif
        if(fram.cnt >= num_loops || fram.stop){
            dsbl_loop();
            return;
        }
        turn = pid_compute(omega);
        lspeed = ramp + lspeed - (int16_t)turn;
        rspeed = ramp + rspeed + (int16_t)turn;
#if RAMP
        // Ramp PWM with constant slope
        if(fram.cnt == rcnt)
            ramp = 0;
#endif
#if !DEBUG
        fram.cnt++;
#endif
        break;
        case CURVE_LEFT:
        case CURVE_RIGHT:
#if DEBUG
        sensor_data[fram.cnt] = (int16_t)ang;
#endif
        if(fabs(set2 - ang) < 10){
            dsbl_loop();
            return;
        }
        turn = pid_compute(omega);
        lspeed = lspeed - (int16_t)turn;
        rspeed = rspeed + (int16_t)turn;
        break;
        case TURN_LEFT:
        case TURN_RIGHT:
#if DEBUG
        if(fram.cnt < 400)
            sensor_data[fram.cnt] = (int16_t)ang;
#endif
        if(fabs(set - ang) < TOLERANCE_DEGREES){
            dsbl_loop();
            return;
        }
        turn = pid_compute(ang);
        lspeed = -(int16_t)turn;
        rspeed = +(int16_t)turn;
        break;
    //default : /* Optional */
    }
    drv_mot(lspeed, rspeed);
#if DEBUG
    fram.cnt++;
#endif
}
