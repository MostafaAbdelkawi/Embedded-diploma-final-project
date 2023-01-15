/*
 * motor.c
 *
 *  Created on: Nov 4, 2020
 *      Author: Mostafa
 */

/* include the header file of the driver */
#include "motor.h"


/* global variable to use in the motor_executeInstruction function */
uint8 motor_instruction = MOTOR_FORWARD ;




/*************************************************************************
* NAME :	void Motor_ExecuteInstruction(void)
*
* DESCRIPTION :	Executes the instruction stored in
* 				the global variable morot_instruction
*
* INPUTS :	Void
*
* OUTPUTS :	Void
*
* PROCESS pseudo code :
* 			 1-Determine the current instruction
* 			 2-Executes it
*
* NOTES :	switch case is used instead of if statement because
* 			the motor can only be at one state at the same time
* 			i.e a motor can't be forward and backward at the same time
***************************************************************************/
void Motor_ExecuteInstruction(void){

/*	if (motor_instruction == MOTOR_FORWARD){

		SET_BIT(MOTOR_PORT,MOTOR_IN1);
		CLEAR_BIT(MOTOR_PORT,MOTOR_IN2);

	}else if (motor_instruction == MOTOR_BACKWARD){

		SET_BIT(MOTOR_PORT,MOTOR_IN2);
		CLEAR_BIT(MOTOR_PORT,MOTOR_IN1);
	}else if (motor_instruction == MOTOR_BREAK){

		CLEAR_BIT(MOTOR_PORT,MOTOR_IN1);
		CLEAR_BIT(MOTOR_PORT,MOTOR_IN2);
	}*/

	switch(motor_instruction){

		case MOTOR_FORWARD:
			DIO_WriteChannel(MOTOR_IN1_CHANNEL,1);
			DIO_WriteChannel(MOTOR_IN2_CHANNEL,0);

			break;
		case MOTOR_BACKWARD:
			DIO_WriteChannel(MOTOR_IN1_CHANNEL,0);
			DIO_WriteChannel(MOTOR_IN2_CHANNEL,1);

			break;
		case MOTOR_BREAK:
			DIO_WriteChannel(MOTOR_IN1_CHANNEL,0);
			DIO_WriteChannel(MOTOR_IN2_CHANNEL,0);
			break;

	}
}



