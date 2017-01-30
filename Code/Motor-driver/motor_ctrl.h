/*
 * motor_ctrl.h
 *
 *  Created on: 23 jan. 2017
 *      Author: Koen Schaper
 */

#ifndef MOTOR_CTRL_H_
#define MOTOR_CTRL_H_

#define MCF 8000000 // Hz
#define FREQ 20
#define DUTY 0.5

extern struct NVvar * fram;

void drv_init();
void prep_inst(uint8_t cmd, uint8_t len);
void drive_motors();

#endif /* MOTOR_CTRL_H_ */
