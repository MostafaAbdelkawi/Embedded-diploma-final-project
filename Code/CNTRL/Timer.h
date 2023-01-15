/*
 * Timer.h
 *
 *  Created on: Nov 27, 2020
 *      Author: Mostafa
 */

#ifndef TIMER_H_
#define TIMER_H_
/*
 * includes the necessary files
 */

#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"

/* enums containing declarations needed for the driver
 *
 * which timer , which mode , prescaler , pointer to function , compare or initial value  */
typedef enum
{
   TIMER_0,TIMER_1,TIMER_2

}_TIMER;

typedef enum
{
   OVERFLOW,COMPARE_MATCH

}_TIMER_MODE;

typedef enum
{
   OFF,_1_TIMER,_8_TIMER,_64_TIMER,_256_TIMER,_1024_TIMER

}_TIMER_PRESCALER;

/*structure holding all the config variables*/
typedef struct
{
	_TIMER timer;
	_TIMER_MODE timerMode;
	_TIMER_PRESCALER prescaler;
	uint16 compareValue;
	uint16 initialValue;
	// void(*User_Callback_PTR)(void); not in the init function


}TIMER_configType;



/************************************************************************************
*
*Function prototypes to enable the user to use the function on including the header
*
**************************************************************************************/
uint8 Timers_INIT(const TIMER_configType * TIMER_configType);


void Timers_setTimer0OverFlowBack(void(*a_ptr)(void));
void Timers_setTimer0CompareCallBack(void(*a_ptr)(void));
void Timers_setTimer1OverFlowBack(void(*a_ptr)(void));
void Timers_setTimer1CompareCallBack(void(*a_ptr)(void));
void Timers_setTimer2OverFlowBack(void(*a_ptr)(void));
void Timers_setTimer2CompareCallBack(void(*a_ptr)(void));

void Timers_clearCounter(_TIMER timer);
void Timers_stop(_TIMER timer);
void Timers_resume(_TIMER timer,_TIMER_PRESCALER prescaler);

#endif /* TIMER_H_ */
