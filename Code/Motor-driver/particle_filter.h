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
    struct Particle parts[250];
}PartArray;

void part_init(uint8_t num_parts);
float gaussian(float mean, float stddev);
void motion_model(uint8_t left, uint8_t right);
uint8_t move(uint8_t dist, float ang);
void resample(uint8_t dcnt);

#endif /* PARTICLE_FILTER_H_ */
