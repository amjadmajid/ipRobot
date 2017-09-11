/*
 * ctrl_loop.h
 *
 *  Created on: 8 aug. 2017
 *      Author: Koen Schaper
 */

#ifndef CTRL_LOOP_H_
#define CTRL_LOOP_H_

extern NVvar fram;

void ctrl_init();
void enbl_loop();
void dsbl_loop();
void set_setpoint(float sp);
void set_tunings(float Kp, float Ki, float Kd);
void set_limits(float max, float min);
float pid_compute(float input);

#endif /* CTRL_LOOP_H_ */
