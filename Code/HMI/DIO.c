/*
 * DIO.c
 *
 *  Created on: Nov 27, 2020
 *      Author: Mostafa
 */

#include "DIO.h"

uint8 decimaltoOctal(uint8 deciNum)
{

    // initializations
    uint8 octalNum = 0, countval = 1;


    while (deciNum != 0) {

        // decimals remainder is calculated
        int remainder = deciNum % 8;

        // storing the octalvalue
        octalNum += remainder * countval;

        // storing exponential value
        countval = countval * 10;
        deciNum /= 8;
    }
    return octalNum;
}

uint8 DIO_WriteChannel(uint8 channelNumber,bool level){

	// switch case with multiple entries cause same result i.e no break
	// OR
	//Conceder using the octal numbering system

	uint8 Octal;
	uint8 error = 0;
	Octal = decimaltoOctal(channelNumber);
	switch((Octal/10)){


	case 0 :
		switch(level){
		case 0 :
			SET_BIT(DDRA,(Octal%10));
			CLEAR_BIT(PORTA,(Octal%10));
			break;
		case 1 :
			SET_BIT(DDRA,(Octal%10));
			SET_BIT(PORTA,(Octal%10));
			break;
		}
		break;
	case 1 :
		switch(level){
		case 0 :
			SET_BIT(DDRB,(Octal%10));
			CLEAR_BIT(PORTB,(Octal%10));
			break;
		case 1 :
			SET_BIT(DDRB,(Octal%10));
			SET_BIT(PORTB,(Octal%10));
			break;
		}
		break;
	case 2 :
		switch(level){
		case 0 :
			SET_BIT(DDRC,(Octal%10));
			CLEAR_BIT(PORTC,(Octal%10));
			break;
		case 1 :
			SET_BIT(DDRC,(Octal%10));
			SET_BIT(PORTC,(Octal%10));
			break;
		}
		break;
	case 3 :

		switch(level){
		case 0 :
			SET_BIT(DDRD,(Octal%10));
			CLEAR_BIT(PORTD,(Octal%10));
			break;
		case 1 :
			SET_BIT(DDRD,(Octal%10));
			SET_BIT(PORTD,(Octal%10));
			break;
		}
		break;

	default :   /*Return 1 indicating a wrong channel number */
		error = 1;

	}

	return error;	/* correct channel number  */

}

uint8 DIO_ReadChannel(uint8 channelNumber){

	// switch case with multiple entries cause same result i.e no break
	//Conceder using the octal numbering system

	uint8 Octal;
	uint8 pinState;
	Octal = decimaltoOctal(channelNumber);
		switch((Octal/10)){


		case 0 :

			CLEAR_BIT(DDRA,(Octal%10)); // make the pin INPUT
			if(BIT_IS_CLEAR(PINA,(Octal%10))){
				pinState = 0;
			}else {
				pinState = 1;
			}
			break;
		case 1 :

			CLEAR_BIT(DDRB,(Octal%10)); // make the pin INPUT
			if(BIT_IS_CLEAR(PINB,(Octal%10))){
				pinState = 0;
			}else {
				pinState = 1;
			}

			break;
		case 2 :

			CLEAR_BIT(DDRC,(Octal%10)); // make the pin INPUT
			if(BIT_IS_CLEAR(PINC,(Octal%10))){
				pinState = 0;
			}else {
				pinState = 1;
			}
			break;
		case 3 :

			CLEAR_BIT(DDRD,(Octal%10)); // make the pin INPUT
			if(BIT_IS_CLEAR(PIND,(Octal%10))){
				pinState = 0;
			}else {
				pinState = 1;
			}

			break;

		default :   /*Return 3 indicating a wrong channel number */
			pinState = 3;

		}



	return pinState;
}
