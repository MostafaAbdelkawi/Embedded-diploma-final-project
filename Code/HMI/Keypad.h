/*
 * Keypad.h
 *
 *  Created on: Nov 27, 2020
 *      Author: Mostafa
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_


#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"

#define Keypad_PORT PORTA   // output
#define Keypad_PIN PINA		// input
#define Keypad_DIR DDRA		// direction selection

void Keypad_init(void);
uint8 Keypad_getKey(void);


#endif /* KEYPAD_H_ */
