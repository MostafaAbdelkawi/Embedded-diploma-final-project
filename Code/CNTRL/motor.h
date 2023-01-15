/*
 * motor.h
 *
 *  Created on: Nov 4, 2020
 *      Author: Mostafa
 */

/*
 * Preprocessors to protect the header file
 * from being included more than once in the same file
 */
#ifndef MOTOR_H_
#define MOTOR_H_

/*
 * includes the necessary files
 */
#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"
#include "DIO.h"

/*
 * Preprocessor defines to abstract the driver from the hardware
 * and eliminate the magic numbers for different instructions
 *
 */
#define MOTOR_IN1_CHANNEL 0
#define MOTOR_IN2_CHANNEL 1


#define MOTOR_FORWARD 0
#define MOTOR_BACKWARD 1
#define MOTOR_BREAK 2


/************************************************************************************
*
*extern the global variable to use it in other files
*written as practice for using extern and variable scope topic
*can be avoided by making the function "Motor_ExecuteInstruction" take an uint8 input
*and use it directly
*
**************************************************************************************/
extern uint8 motor_instruction;

/************************************************************************************
*
*Extern a function from the Timer0_PWM driver to use it in setting the motor speed
*
**************************************************************************************/
//void TM0_PWM_Set_OCR0(uint8 top); /* Extern it from Timer0_PWM.c */


/************************************************************************************
*
*Function prototypes to enable the user to use the function on including the header
*
**************************************************************************************/
void Motor_pinsInit();

void Motor_ExecuteInstruction(void);





#endif /* MOTOR_H_ */
