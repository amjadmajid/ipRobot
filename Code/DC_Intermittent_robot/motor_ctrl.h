/*
 * motor_ctrl.h
 *
 *  Created on: 7 jul. 2017
 *      Author: Koen Schaper
 */

#ifndef MOTOR_CTRL_H_
#define MOTOR_CTRL_H_

extern NVvar fram;
extern uint8_t running;

void drv_init();
void enbl_mot();
void dsbl_mot();
void forward();
void reverse();

#endif /* MOTOR_CTRL_H_ */
