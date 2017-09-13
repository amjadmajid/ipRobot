/*
 * ctrl_loop.h
 *
 *  Created on: 8 aug. 2017
 *      Author: Koen Schaper
 */

#ifndef CTRL_LOOP_H_
#define CTRL_LOOP_H_

#define SAMPLE_TIME 0.02 // 50Hz

#define STRAIGHT   0x01
#define TURN_LEFT  0x02
#define TURN_RIGHT 0x03

extern NVvar fram;

void ctrl_init();
void enbl_loop(int16_t ls, int16_t rs);
void move(uint8_t cmd, int16_t arg);
void dsbl_loop();
void set_setpoint(float sp);
void set_tunings(float Kp, float Ki, float Kd);
void set_limits(float max, float min);
float pid_compute(float input);

#endif /* CTRL_LOOP_H_ */
