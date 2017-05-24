/*
 * const_map.c
 *
 *  Created on: 20 apr. 2017
 *      Author: Koen Schaper
 */

#include <stdint.h>
#include "const_map.h"

#pragma PERSISTENT(map);
ConstMap map = {0};

// put constrained map in nv mem (only run once)
void const_map_init() {
    map.x_size = 120;
    map.y_size = 120;
    map.num_walls = 17;

    map.walls[0].x1 = 39;
    map.walls[0].x2 = 41;
    map.walls[0].y1 = 0;
    map.walls[0].y2 = 21;

    map.walls[1].x1 = 79;
    map.walls[1].x2 = 120;
    map.walls[1].y1 = 19;
    map.walls[1].y2 = 21;

    map.walls[2].x1 = 19;
    map.walls[2].x2 = 21;
    map.walls[2].y1 = 19;
    map.walls[2].y2 = 41;

    map.walls[3].x1 = 59;
    map.walls[3].x2 = 61;
    map.walls[3].y1 = 19;
    map.walls[3].y2 = 41;

    map.walls[4].x1 = 19;
    map.walls[4].x2 = 101;
    map.walls[4].y1 = 39;
    map.walls[4].y2 = 41;

    map.walls[5].x1 = 39;
    map.walls[5].x2 = 41;
    map.walls[5].y1 = 39;
    map.walls[5].y2 = 61;

    map.walls[6].x1 = 79;
    map.walls[6].x2 = 81;
    map.walls[6].y1 = 39;
    map.walls[6].y2 = 61;

    map.walls[7].x1 = 0;
    map.walls[7].x2 = 21;
    map.walls[7].y1 = 59;
    map.walls[7].y2 = 61;

    map.walls[8].x1 = 59;
    map.walls[8].x2 = 81;
    map.walls[8].y1 = 59;
    map.walls[8].y2 = 61;

    map.walls[9].x1 = 59;
    map.walls[9].x2 = 61;
    map.walls[9].y1 = 59;
    map.walls[9].y2 = 101;

    map.walls[10].x1 = 99;
    map.walls[10].x2 = 101;
    map.walls[10].y1 = 59;
    map.walls[10].y2 = 81;

    map.walls[11].x1 = 19;
    map.walls[11].x2 = 61;
    map.walls[11].y1 = 79;
    map.walls[11].y2 = 81;

    map.walls[12].x1 = 79;
    map.walls[12].x2 = 101;
    map.walls[12].y1 = 79;
    map.walls[12].y2 = 81;

    map.walls[13].x1 = 19;
    map.walls[13].x2 = 21;
    map.walls[13].y1 = 79;
    map.walls[13].y2 = 101;

    map.walls[14].x1 = 79;
    map.walls[14].x2 = 81;
    map.walls[14].y1 = 79;
    map.walls[14].y2 = 101;

    map.walls[15].x1 = 79;
    map.walls[15].x2 = 120;
    map.walls[15].y1 = 99;
    map.walls[15].y2 = 101;

    map.walls[16].x1 = 39;
    map.walls[16].x2 = 41;
    map.walls[16].y1 = 99;
    map.walls[16].y2 = 120;
}

uint8_t is_wall(uint8_t x, uint8_t y) {

    uint8_t i;

    // check if coordinate is outside map
    if((x > map.x_size) || (y > map.y_size)) {
        return 1;
    }

    // check if coordinate is inside wall
    for(i=0; i < map.num_walls; i++) {
        if((x >= map.walls[i].x1) && (x <= map.walls[i].x2) && (y >= map.walls[i].y1) && (y <= map.walls[i].y2)){
            return 1;
        }
    }
    return 0;
}
