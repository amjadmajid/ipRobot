/*
 * particle_filter.c
 *
 *  Author: Koen Schaper
 */

#include <stdint.h>
#include <math.h>
#include "particle_filter.h"
#include "const_map.h"

#ifdef PC
#include <stdlib.h>
#include <time.h>
#else
#include "msp_lib.h"
#endif

/* libfixmath from:
 * https://github.com/PetteriAimonen/libfixmath
 */
#include "libfixmath/fix16.h"

extern ConstMap map;
PartArray parr;

#ifdef PC
void init_rand(){
    srand(time(NULL));
}

uint16_t get_rand(){
    return rand();
}
#endif

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
        parr.parts[i].t = 0;
        parr.parts[i].w = (1.0 / num_parts);
    }
}

// only used to give Python access
PartArray* get_parts(){
    return &parr;
}

/*
 * The Marsaglia polar method
 * https://en.wikipedia.org/wiki/Marsaglia_polar_method
 */
float gaussian(float mean, float stddev){

    static float extra;
    static uint8_t cache = 0;
    float s, z, u, v, t;

    if (cache) {
        cache = 0;
        z = extra * stddev + mean;
    }
    else {
        do {
         u = 2.0 * get_rand() / RAND_MAX - 1.0;
         v = 2.0 * get_rand() / RAND_MAX - 1.0;
         s = u * u + v * v;
      } while(s >= 1.0 || s == 0.0);

      t = sqrt(-2.0 * log(s) / s);
      extra = v * t;
      cache = 1;
      z = u * t * stddev + mean;
   }
   return z;
}

// for now only converts wheel odometry into
void motion_model(uint8_t left, uint8_t right){

}

// update all particle position according to steps done in certain direction
uint8_t move(uint8_t dist, float ang){

    uint8_t i, dcnt = 0;
    int8_t x, y, xn, yn;

    xn = cos(ang)*dist;
    yn = sin(ang)*dist;

    for(i=0; i<parr.num_parts; i++){
        if(parr.parts[i].w > 0){
            x = parr.parts[i].x + xn;
            y = parr.parts[i].y + yn;

            if(is_wall(x, y)){
                parr.parts[i].w = 0;
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
        } while(parr.parts[j].w == 0);
        // make this change consistent ?
        parr.parts[j].w++;
    }
}
