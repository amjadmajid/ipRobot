/*
 * motor_ctrl.h
 *
 *  Created on: 7 jul. 2017
 *      Author: Koen Schaper
 */

#ifndef MOTOR_CTRL_H_
#define MOTOR_CTRL_H_

#define TCA_ADDR 112 // both TCA9538 address pins to GND (116 for TCA9539)

extern NVvar fram;
extern uint8_t running;

void drv_init();
void enbl_mot();
void dsbl_mot();

#endif /* MOTOR_CTRL_H_ */
