/*
 * const_map.h
 *
 *  Created on: 20 apr. 2017
 *      Author: Koen Schaper
 */

#ifndef CONST_MAP_H_
#define CONST_MAP_H_

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

void const_map_init();
uint8_t is_wall(uint8_t x, uint8_t y);

#endif /* CONST_MAP_H_ */
