#include <msp430.h> 
#include <stdint.h>
#include "global.h"
#include "eusci_b0_i2c.h"
#include "gyro_sens.h"
#include "motor_ctrl.h"

/*
 * main.c
 */

#pragma PERSISTENT(sensor_data);
int16_t sensor_data[400] = {0};

#pragma PERSISTENT(fram);
NVvar fram = {0};

void init(void) {

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    FRCTL0 = 0xA500 | ((1) << 4);           // Disable FRAM wait cycles to allow clock operation over 8MHz

    CSCTL0_H = CSKEY_H;                     // Unlock clock registers
    CSCTL1 = DCOFSEL_6;                     // Set DCO to 8MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__4 | DIVM__1;   // Set all dividers
    CSCTL0_H = 0;                           // Lock CS registers

    PJDIR |= BIT6;
    PJOUT &= ~BIT6;
}

    int main(void) {

    const uint16_t lspeed = 300;
    uint16_t rspeed = 300;
    const float Kp = 0.14;
    const float Kd = 0.8;

    uint16_t cnt = 0;
    int16_t data = 0;
    float omega = 0;
    float prev = 0, derr = 0;

    init();
    i2c_init();
    gyro_init();
    drv_init();

    if(fram.cp == 0){
       fram.cp = 1;

        /*if(CNT_AFTER){
            fram.cnt--;
        }*/

        enbl_mot();
        forward(lspeed, rspeed);

        // Run at approx 400Hz
        while(cnt < 400){
            data = gyro_read();
            omega = data / 131.0;
            derr = omega - prev;
            rspeed = rspeed - (Kp*omega + Kd*derr);
            set_fspeed(lspeed, rspeed);
            sensor_data[cnt] = data;
            prev = omega;
            cnt++;
            __delay_cycles(20000);
        }
        dsbl_mot();

        /*__delay_cycles(8000000);
        reverse();
        __delay_cycles(8000000);
        dsbl_mot(); */

        //Enable LED to drain excess energy
        PJOUT |= BIT6;
    }

    return 0;
}
