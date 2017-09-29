/*
 * ctrl_loop.h
 *
 *  Created on: 8 aug. 2017
 *      Author: Koen Schaper
 */

#ifndef CTRL_LOOP_H_
#define CTRL_LOOP_H_

#define SAMPLE_TIME 0.01 // 100Hz

#define STRAIGHT   0x01
#define TURN_LEFT  0x02
#define TURN_RIGHT 0x03

// Speed / Distance tuning parameters
#define MOT_TRG 40
#define VEL_CAL 25 //approx 25cm/s
#define STEP_OFF 3

#define TOLERANCE_DEGREES 2.0

extern NVvar fram;

void ctrl_init();
void move(uint8_t cmd, int16_t arg);

#endif /* CTRL_LOOP_H_ */
