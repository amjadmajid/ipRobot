/*
 * particle_filter.c
 *
 *  Author: Koen Schaper
 */

#include <msp430.h>
#include <stdint.h>
#include "particle_filter.h"

ConstMap * map = (ConstMap *) 0x1880;

// put constrained map in nv mem (only run once)
void create_map_const() {
    map->x_size = 10;
    map->y_size = 10;
    map->num_walls = 2;

    map->walls[0].x1 = 4;
    map->walls[0].x2 = 10;
    map->walls[0].y1 = 0;
    map->walls[0].y2 = 3;

    map->walls[1].x1 = 4;
    map->walls[1].x2 = 10;
    map->walls[1].y1 = 7;
    map->walls[1].y2 = 10;
}

uint8_t is_wall(uint8_t x, uint8_t y) {

    uint8_t i;

    // check if coordinate is outside map
    if((x > map->x_size) || (y > map->y_size)) {
        return 1;
    }

    // check if coordinate is inside wall
    for(i=0; i < map->num_walls; i++) {
        if((x > map->walls[i].x1) && (x < map->walls[i].x2) && (y > map->walls[i].y1) && (y < map->walls[i].y2)){
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
/*void part_init(uint16_t num_part){

} */

// update all particle position according to steps done in certain direction
/* void move(){

} */

// "random" increase particle weight according to likelihood of being there
// (only with died amount of particles)
/* void resample(){

} */
