/*
 * Buzzer.h
 *
 *  Created on: Nov 27, 2020
 *      Author: Mostafa
 */

#ifndef BUZZER_H_
#define BUZZER_H_


#include "DIO.h"
#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"

#define BUZZER_CHANNEL 8

void buzzerOn(void);

void buzzerOff(void);


#endif /* BUZZER_H_ */
