/*
 * prox_sens.h
 *
 *  Created on: 13 mrt. 2017
 *      Author: Koen Schaper
 */

#ifndef PROX_SENS_H_
#define PROX_SENS_H_

#define MAX_ADDR 74

void prox_init();
uint8_t prox_read();
int16_t als_read();

#endif /* PROX_SENS_H_ */
