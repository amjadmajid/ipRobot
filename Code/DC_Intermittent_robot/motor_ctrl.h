/*
 * motor_ctrl.h
 *
 *  Created on: 7 jul. 2017
 *      Author: Koen Schaper
 */

#ifndef MOTOR_CTRL_H_
#define MOTOR_CTRL_H_

#define SMAX 200

#define LMIN 150
#define RMIN 120

void drv_init();
void enbl_mot();
void dsbl_mot();
void drv_mot(int16_t sl, int16_t sr);

#endif /* MOTOR_CTRL_H_ */
