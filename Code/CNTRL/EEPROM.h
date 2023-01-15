/*
 * EEPROM.h
 *
 *  Created on: Nov 27, 2020
 *      Author: Mostafa
 */

#ifndef EEPROM_H_
#define EEPROM_H_


#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"
#include "I2C.h"

void EEPROM_write(uint16 address, uint8 data);

void EEPROM_read(uint16 address, uint8 *data);

#endif /* EEP ROM_H_ */
