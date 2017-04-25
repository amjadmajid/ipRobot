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
float rand_n(){

    static float extra;
    static uint8_t cache = 0;
    float s, z, u, v, t;

    if (cache) {
        cache = 0;
        z = extra; //* stddev + mean;
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
      z = u * t; //* stddev + mean;
   }
   return z;
}

// update all particle position according to steps done in certain direction
float move(float dist, float ang){

    uint8_t i;
    float dr, tr, x, y, t, w;

    for(i=0; i<parr.num_parts; i++){
        // Model error
        dr = ang; //gaussian(ang, 0);
        tr = dist; //gaussian(dist, 0);

        t = parr.parts[i].t + dr;
        x = parr.parts[i].x + (tr * cos(dr));
        y = parr.parts[i].y + (tr * sin(dr));

        if(is_wall(x, y)){
            parr.parts[i].w = 0;
        } else {
            parr.parts[i].x = x;
            parr.parts[i].y = y;
            parr.parts[i].t = t;
            w += parr.parts[i].w;
        }
    }
    return w;
}


// normalize particle weight and compute Neff
float update(float w){
    float wn, neff;
    int i;
    wn = 1.0 / (w);
    for (i=0; i<parr.num_parts; i++){
        if(parr.parts[i].w > 0){
            parr.parts[i].w = parr.parts[i].w * wn;
            neff += parr.parts[i].w * parr.parts[i].w;
        }
    }
    return 1/neff;
}

// "random" increase particle weight (only with died amount of particles)
/*
 * Low variance resampling
 */
void resample(){

    uint8_t m, M, i, j;
    float r, w, U;
    M = parr.num_parts;
    r = rand_n() / M;
    w = parr.parts[1].w;
    i = 1, j = 1;
    PartArray temp;
    temp.num_parts = M;

    for(m=1; m < M; m++){
        U = (r + (m - 1) / M);
        while(U > w){
            i = i + 1;
            w = w + parr.parts[i].w;
        }
        temp.parts[j] = parr.parts[i];
        j = j + 1;
    }
    parr = temp;
}
