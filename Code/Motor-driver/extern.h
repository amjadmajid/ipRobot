/*
 * extern.h
 *
 *  Created on: 23 jan. 2017
 *      Author: Koen Schaper
 */

#ifndef EXTERN_H_
#define EXTERN_H_

// Note: INFOD has a length of 128 bytes
struct NVvar {
    uint8_t cp;
    uint8_t cnt;
    uint8_t len;
    uint8_t inst[100];
};

#endif /* EXTERN_H_ */
