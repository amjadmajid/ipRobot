/*
 * motor_ctrl.h
 *
 *  Created on: 23 jan. 2017
 *      Author: Koen Schaper
 */

#ifndef MOTOR_CTRL_H_
#define MOTOR_CTRL_H_

#define DELAY 100000

extern struct NVvar * fram;

void drv_init();
void prep_inst(uint8_t cmd, uint8_t len);
void drv_mot();
void dsbl_mot();

#endif /* MOTOR_CTRL_H_ */
