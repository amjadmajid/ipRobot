/*
 * global.h
 *
 *  Created on: 23 jan. 2017
 *      Author: Koen Schaper
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#define DEBUG 0

#define NUM_CP 80
#define RUN_TIME 50 //in mm
#define DELAY RUN_TIME*690000/NUM_CP
#define RUN_TIME2 RUN_TIME*(360000/4000)

#define CNT_AFTER 1

typedef struct NVvar {
    uint8_t cp;
    int16_t cnt;
}NVvar;

#endif /* GLOBAL_H_ */
