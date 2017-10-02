/*
 * motor_ctrl.h
 *
 *  Created on: 23 jan. 2017
 *      Author: Koen Schaper
 */

#ifndef MOTOR_CTRL_H_
#define MOTOR_CTRL_H_

#define DELAY (8000000 / 20) //50000 // 25 Hz
#define TCA_ADDR 112 // both TCA9538 address pins to GND (116 for TCA9539)

void drv_init();
void drv_mot(uint8_t data);
void dsbl_mot();

#endif /* MOTOR_CTRL_H_ */
