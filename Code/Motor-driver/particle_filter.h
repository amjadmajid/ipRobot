/*
 * particle_filter.h
 *
 *  Created on: 23 jan. 2017
 *      Author: Koen Schaper
 */

#ifndef PARTICLE_FILTER_H_
#define PARTICLE_FILTER_H_

typedef struct Wall {
    uint8_t x1;
    uint8_t x2;
    uint8_t y1;
    uint8_t y2;
}Wall;

typedef struct ConstMap {
    uint8_t x_size;
    uint8_t y_size;
    uint8_t num_walls;
    struct Wall walls[31];
}ConstMap;

typedef struct Particle {
    uint8_t x;
    uint8_t y;
    uint8_t ang;
}Particle;

typedef struct PartArray {
    uint8_t num_parts;
    struct Particle parts[63];
}PartArray;

void const_map_init();
uint8_t is_wall(uint8_t x, uint8_t y);
uint16_t get_rand();
void part_init(uint8_t num_parts);

#endif /* PARTICLE_FILTER_H_ */
