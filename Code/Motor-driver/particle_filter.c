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
// only used to give Python access
PartArray* get_parts(){
    return &parr;
}
#endif

float get_rand(){
    return ((float)rand() / RAND_MAX);
}


// initialize x number of particles in non wall area's
void part_init(uint8_t num_parts){

    uint8_t i;
    uint8_t x, y;

    parr.num_parts = num_parts;

    for(i=0; i<num_parts; i++){
        do {
            x = get_rand() * map.x_size;
            y = get_rand() * map.y_size;
        } while(is_wall(x, y));
        // make these changes consistent ?
        parr.parts[i].x = x;
        parr.parts[i].y = y;
        parr.parts[i].t = 0;
        parr.parts[i].w = (1.0 / num_parts);
    }
}


/*
 * The Marsaglia polar method
 * https://en.wikipedia.org/wiki/Marsaglia_polar_method
 */
float rand_n(float mu, float sigma){

    static float extra;
    static uint8_t cache = 0;
    float s, z, u, v, t;

    if (cache) {
        cache = 0;
        z = extra * sigma + mu;
    }
    else {
        do {
         u = 2.0 * get_rand() - 1.0;
         v = 2.0 * get_rand() - 1.0;
         s = u * u + v * v;
      } while(s >= 1.0 || s == 0.0);

      t = sqrt(-2.0 * log(s) / s);
      extra = v * t;
      cache = 1;
      z = u * t * sigma + mu;
   }
   return z;
}


// update all particle position according to steps done in certain direction
float move(float dist, float ang){

    uint8_t i;
    float dr, tr, x, y, t, w;

    for(i=0; i<parr.num_parts; i++){
        // Model error
        dr = rand_n(ang, ((float)M_PI/180)*2);
        tr = rand_n(dist, 0.01);

        t = parr.parts[i].t + dr;
        x = parr.parts[i].x + (tr * cos(t));
        y = parr.parts[i].y + (tr * sin(t));

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
        parr.parts[i].w = parr.parts[i].w * wn;
        neff += parr.parts[i].w * parr.parts[i].w;
    }
    return 1/neff;
}


// low variance resampling (only resample if Neff is smaller than value x)
void resample(){

    uint8_t m, M, i;
    float r, w, u;
    M = parr.num_parts;
    r = get_rand() / M;
    w = parr.parts[0].w;
    i = 0;
    PartArray temp;
    temp.num_parts = M;

    for(m=0; m < M; m++){
        u = (r + (float)m / M);
        while(u > w){
            i += 1;
            w = w + parr.parts[i].w;
        }
        temp.parts[m] = parr.parts[i];
    }
    parr = temp;
}
