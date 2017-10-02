/*
 * motor_ctrl.h
 *
 *  Created on: 7 jul. 2017
 *      Author: Koen Schaper
 */

#ifndef MOTOR_CTRL_H_
#define MOTOR_CTRL_H_

// Maximum motor value
#define SMAX 75

// Minimum motor values
#define LMIN 80
#define RMIN 65

void drv_init();
void enbl_mot();
void dsbl_mot();
void drv_mot(int16_t sl, int16_t sr);

#endif /* MOTOR_CTRL_H_ */
