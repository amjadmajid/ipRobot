/*
 * particle_filter.c
 *
 *  Author: Koen Schaper
 */

#include <msp430.h>
#include <stdint.h>
#include "particle_filter.h"

#pragma NOINIT(map)
ConstMap map;
#pragma NOINIT(parr)
PartArray parr;

// put constrained map in nv mem (only run once)
void const_map_init() {
    map.x_size = 100;
    map.y_size = 100;
    map.num_walls = 4;

    map.walls[0].x1 = 0;
    map.walls[0].x2 = 25;
    map.walls[0].y1 = 0;
    map.walls[0].y2 = 75;

    map.walls[1].x1 = 50;
    map.walls[1].x2 = 100;
    map.walls[1].y1 = 0;
    map.walls[1].y2 = 25;

    map.walls[1].x1 = 50;
    map.walls[1].x2 = 75;
    map.walls[1].y1 = 50;
    map.walls[1].y2 = 100;

    map.walls[1].x1 = 75;
    map.walls[1].x2 = 100;
    map.walls[1].y1 = 75;
    map.walls[1].y2 = 100;
}

uint8_t is_wall(uint8_t x, uint8_t y) {

    uint8_t i;

    // check if coordinate is outside map
    if((x > map.x_size) || (y > map.y_size)) {
        return 1;
    }

    // check if coordinate is inside wall
    for(i=0; i < map.num_walls; i++) {
        if((x > map.walls[i].x1) && (x < map.walls[i].x2) && (y > map.walls[i].y1) && (y < map.walls[i].y2)){
            return 1;
        }
    }
    return 0;
}

/*
* Based on the random number generator example:
* http://processors.wiki.ti.com/index.php/MSP430_FAQ#How_to_generate_random_number_with_MSP430_devices.3F
* Modified to work with MSP430FR59xx
*/
uint16_t get_rand() {

  uint8_t i;
  uint16_t result = 0;

  // Timer0_A3 Setup
  TA0CCTL2 = CM_1 + CCIS_1 + CAP;
  TA0CTL |= TASSEL__SMCLK + MC__CONTINOUS;

  for(i=0 ; i<16 ; i++) {

    // shift left result
    result <<= 1;
    // wait until Capture flag is set
    while(!(TA0CCTL2 & CCIFG));
    // clear flag
    TA0CCTL2 &= ~CCIFG;
    // check LSB
    if(TA0CCR2 & 0x01) {
        result |= 0x01;
    }
    // change the division of timer input clock
    TA0CTL = (TA0CTL & 0xFF3F) | ((TA0CCR2 & 0x03) << 6);
  }
  return result;
}

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
