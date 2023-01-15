/*
 * Keypad.c
 *
 *  Created on: Nov 27, 2020
 *      Author: Mostafa
 */
#include "Keypad.h"

/* array storing all possible keypad values in order */
uint8 keys[4][4]={{'7','8','9','%'},
				  {'4','5','6','*'},
				  {'1','2','3','-'},
				  {'c','0','=','+'}

							};

void Keypad_init(void){

	Keypad_DIR = 0b00001111;    // make rows output and col input
	Keypad_PORT = 0b11111111;   // activate internal pullup for the input pins and make the output pins High by default


}
/*note to myself*/
/*clean up the code use logical names*/
uint8 Keypad_getKey(void){
	uint8 key = 'x';
	int i,j;

		for (i=0;i<4;i++){
			//Keypad_DIR = (0b00001000>>i);     /*in the simulation it wont make a diff*/
			Keypad_PORT = (~(0b00001000)>>i);
			for(j=0;j<4;j++){
				if(BIT_IS_CLEAR(Keypad_PIN,(j+4))){

					key = keys[3-i][j];

				}

			}

		}

		return key;

}
