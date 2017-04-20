/*
 * particle_filter.c
 *
 *  Author: Koen Schaper
 */

#include <msp430.h>
#include <stdint.h>
#include "particle_filter.h"
#include "const_map.h"
#include "msp_lib.h"

/* libfixmath from:
 * https://github.com/PetteriAimonen/libfixmath
 */
#include "libfixmath/fix16.h"

extern ConstMap map;
PartArray parr;

// initialize x number of particles in non wall area's
void part_init(uint8_t num_parts){

    uint8_t i;
    uint8_t x, y;

    parr.num_parts = num_parts;

    for(i=0; i<num_parts; i++){
        do {
            x = get_rand() % map.x_size;
            y = get_rand() % map.y_size;
        } while(is_wall(x, y));
        // make these changes consistent ?
        parr.parts[i].x = x;
        parr.parts[i].y = y;
        parr.parts[i].wgt = 1;
    }
}

// update all particle position according to steps done in certain direction
uint8_t move(uint8_t dist, uint8_t ang){

    uint8_t i, dcnt = 0;
    uint8_t x, y, xn, yn;

    //xn = cos(ang)*dist;
    //yn = sin(ang)*dist;

    for(i=0; i<parr.num_parts; i++){
        if(parr.parts[i].wgt > 0){
            x = parr.parts[i].x + xn;
            y = parr.parts[i].y + yn;

            if(is_wall(x, y)){
                parr.parts[i].wgt = 0;
                dcnt++;
            } else {
                parr.parts[i].x = x;
                parr.parts[i].y = y;
            }
        }
    }
    return dcnt;
}

// "random" increase particle weight (only with died amount of particles)
void resample(uint8_t dcnt){

    uint8_t i, j;

    for(i=0; i<dcnt; i++){
        do{
            j = get_rand() % parr.num_parts;
        } while(parr.parts[j].wgt == 0);
        // make this change consistent ?
        parr.parts[j].wgt++;
    }
}
