/*
 * global.h
 *
 *  Created on: 23 jan. 2017
 *      Author: Koen Schaper
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#define DEBUG 0

typedef struct NVvar {
    uint8_t cp;
    uint8_t once;
    uint16_t arg;
    uint16_t cnt;
    float ang;
    uint8_t stop;
}NVvar;

#endif /* GLOBAL_H_ */
