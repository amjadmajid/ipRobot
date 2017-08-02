/*
 * motor_ctrl.h
 *
 *  Created on: 7 jul. 2017
 *      Author: Koen Schaper
 */

#ifndef MOTOR_CTRL_H_
#define MOTOR_CTRL_H_

void drv_init();
void enbl_mot();
void dsbl_mot();
void forward(uint16_t sl, uint16_t sr);
void set_fspeed(uint16_t sl, uint16_t sr);
void reverse();

#endif /* MOTOR_CTRL_H_ */
