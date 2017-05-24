/*
 * particle_filter.h
 *
 *  Created on: 23 jan. 2017
 *      Author: Koen Schaper
 */

#ifndef PARTICLE_FILTER_H_
#define PARTICLE_FILTER_H_

typedef struct Particle {
    float x;
    float y;
    float t;
    float w;
}Particle;

typedef struct PartArray {
    uint8_t num_parts;
    struct Particle parts[100];
}PartArray;

void part_init(uint8_t num_parts);
float rand_n(float mu, float sigma);
float move(float dist, float ang);
float update(float w);
void resample();

#endif /* PARTICLE_FILTER_H_ */
