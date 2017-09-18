/*
 * global.h
 *
 *  Created on: 23 jan. 2017
 *      Author: Koen Schaper
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#define DEBUG 0

#define CNT_AFTER 1

typedef struct NVvar {
    uint8_t cp;
    uint8_t once;
    uint8_t stop;
    uint16_t sarg;
    uint16_t cnt;
}NVvar;

#endif /* GLOBAL_H_ */
