/*
 * gyro_sens.h
 *
 *  Created on: 1 mei 2017
 *      Author: Koen Schaper
 */

#ifndef GYRO_SENS_H_
#define GYRO_SENS_H_

#define BMG_ADDR 0x68

void gyro_init();
uint16_t gyro_read();

#endif /* GYRO_SENS_H_ */
