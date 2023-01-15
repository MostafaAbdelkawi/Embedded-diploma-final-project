/*
 * Buzzer.c
 *
 *  Created on: Nov 27, 2020
 *      Author: Mostafa
 */
#include "Buzzer.h"



/*
 * turns on the buzzer using the DIO driver
 * */
void buzzerOn(void){

	DIO_WriteChannel(BUZZER_CHANNEL,1);

}


/*
 * turns off the buzzer using the DIO driver
 * */
void buzzerOff(void){

	DIO_WriteChannel(BUZZER_CHANNEL,0);

}
