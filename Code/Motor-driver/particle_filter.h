/*
 * particle_filter.h
 *
 *  Created on: 23 jan. 2017
 *      Author: Koen Schaper
 */

#ifndef PARTICLE_FILTER_H_
#define PARTICLE_FILTER_H_

typedef struct Particle {
    uint8_t x;
    uint8_t y;
    uint8_t wgt;
}Particle;

typedef struct PartArray {
    uint8_t num_parts;
    struct Particle parts[250];
}PartArray;

uint8_t is_wall(uint8_t x, uint8_t y);
void part_init(uint8_t num_parts);
uint8_t move(uint8_t dist, uint8_t ang);
void resample(uint8_t dcnt);

#endif /* PARTICLE_FILTER_H_ */
