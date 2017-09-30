/*
 * msp_lib.c
 *
 *  Created on: 20 apr. 2017
 *      Author: Koen Schaper
 */

#include <msp430.h>
#include <stdint.h>
#include "msp_lib.h"

/*
* Based on the random number generator example:
* http://processors.wiki.ti.com/index.php/MSP430_FAQ#How_to_generate_random_number_with_MSP430_devices.3F
* Modified to work with MSP430FR5969 with timer A3 where ACLK is connected to CCI0B
* Only used to generate a seed value
*/
uint16_t get_seed() {

    uint8_t i;
    uint16_t result = 0;

    // Timer_A3
    TA3CCTL0 = CM_1 + CCIS_1 + CAP;
    TA3CTL |= TASSEL__SMCLK + MC__CONTINOUS + TACLR;

    for(i=0 ; i<16 ; i++) {

        // shift left result
        result <<= 1;
        // wait until Capture flag is set
        while(!(TA3CCTL0 & CCIFG));
        // clear flag
        TA3CCTL0 &= ~CCIFG;
        // check LSB
        if(TA3CCR0 & 0x01) {
            result |= 0x01;
        }
        // change the division of timer input clock
        TA3CTL = (TA3CTL & 0xFF3F) | ((TA3CCR0 & 0x03) << 6);
    }
    // stop timer
    TA3CTL = MC__STOP;
    return result;
}
