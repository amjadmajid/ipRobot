/*
 * global.h
 *
 *  Created on: 23 jan. 2017
 *      Author: Koen Schaper
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#define DEBUG 1

typedef struct NVvar {
    uint8_t cp_nr;
    uint8_t inst;
}NVvar;

typedef struct pi {
    uint8_t cnt;
    int8_t lstate;
    int8_t rstate;
}pi;

#endif /* GLOBAL_H_ */
