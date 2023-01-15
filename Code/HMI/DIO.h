/*
 * DIO.h
 *
 *  Created on: Nov 27, 2020
 *      Author: Mostafa
 */

#ifndef DIO_H_
#define DIO_H_
#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"

uint8 DIO_WriteChannel(uint8 channelNumber,bool level);

uint8 DIO_ReadChannel(uint8 channelNumber);

uint8 decimaltoOctal(uint8 deciNum);

#endif /* DIO_H_ */
