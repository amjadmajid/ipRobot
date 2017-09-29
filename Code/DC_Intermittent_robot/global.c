/*
 * global.c
 *
 *  Created on: 29 sep. 2017
 *      Author: Koen Schaper
 */

#include <stdint.h>
#include "global.h"

void swap(NVvar *a, NVvar *b){

    NVvar tmp = *b;
    *b   = *a;
    *a   = tmp;
}
