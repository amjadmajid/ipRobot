/*
 * motor_ctrl.h
 *
 *  Created on: 7 jul. 2017
 *      Author: Koen Schaper
 */

#ifndef MOTOR_CTRL_H_
#define MOTOR_CTRL_H_

#define MIN 150

void drv_init();
void enbl_mot();
void dsbl_mot();
void drv_mot(int8_t sl, int8_t sr);

#endif /* MOTOR_CTRL_H_ */
