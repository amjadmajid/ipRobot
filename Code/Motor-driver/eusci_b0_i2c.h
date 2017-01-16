/*
 * eusci_b0_i2c.h
 *
 *  Created on: 16 jan. 2017
 *      Author: Koen Schaper
 */

#ifndef EUSCI_B0_I2C_H_
#define EUSCI_B0_I2C_H_

#define DATA_SIZE 2
#define SLAVE_ADDR 116 // TCA9539 address pins to GND

void i2c_init();
void i2c_transmit(uint8_t cmd, uint8_t data);

#endif /* EUSCI_B0_I2C_H_ */
