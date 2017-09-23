/*
 * motor_ctrl.h
 *
 *  Created on: 7 jul. 2017
 *      Author: Koen Schaper
 */

#ifndef MOTOR_CTRL_H_
#define MOTOR_CTRL_H_

#define SMAX 130

#define LMIN 130
#define RMIN 110

void drv_init();
void enbl_mot();
void dsbl_mot();
void drv_mot(int16_t sl, int16_t sr);

#endif /* MOTOR_CTRL_H_ */
