/*
 * ctrl_loop.h
 *
 *  Created on: 8 aug. 2017
 *      Author: Koen Schaper
 */

#ifndef CTRL_LOOP_H_
#define CTRL_LOOP_H_

extern NVvar fram;

void ctrl_init();
void enbl_loop();
void upd_set(float s);
void dsbl_loop();

#endif /* CTRL_LOOP_H_ */
