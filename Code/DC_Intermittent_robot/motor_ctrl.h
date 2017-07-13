/*
 * motor_ctrl.h
 *
 *  Created on: 7 jul. 2017
 *      Author: Koen Schaper
 */

#ifndef MOTOR_CTRL_H_
#define MOTOR_CTRL_H_

#define TCA_ADDR 112 // both TCA9538 address pins to GND (116 for TCA9539)

void drv_init();
void drv_mot();
void dsbl_mot();
void dsbl_tim();

#endif /* MOTOR_CTRL_H_ */
