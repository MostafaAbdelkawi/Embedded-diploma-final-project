/*
 * Timer.c
 *
 *  Created on: Nov 27, 2020
 *      Author: Mostafa
 */
#include "Timer.h"


/* call back pointers for each timer and for each interrupt source */
static volatile void (*TIMER0_OVF_callBackPtr)(void) = NULL;
static volatile void (*TIMER0_COMP_callBackPtr)(void) = NULL;

static volatile void (*TIMER1_OVF_callBackPtr)(void) = NULL;
static volatile void (*TIMER1_COMPA_callBackPtr)(void) = NULL;


static volatile void (*TIMER2_OVF_callBackPtr)(void) = NULL;
static volatile void (*TIMER2_COMP_callBackPtr)(void) = NULL;

/*
 * timer 0 overflow ISR
 * checking if the callback pointer is NULL
 * if not then execute the callback function on ISR entry
 * */
ISR(TIMER0_OVF_vect){

	if(TIMER0_OVF_callBackPtr != NULL)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*TIMER0_OVF_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}



/*
 * timer 0 compare match ISR
 * checking if the callback pointer is NULL
 * if not then execute the callback function on ISR entry
 * */
ISR(TIMER0_COMP_vect){

	if(TIMER0_COMP_callBackPtr != NULL)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*TIMER0_COMP_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}

}



/*
 * timer 1 overflow ISR
 * checking if the callback pointer is NULL
 * if not then execute the callback function on ISR entry
 * */
ISR(TIMER1_OVF_vect){

	if(TIMER1_OVF_callBackPtr != NULL)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*TIMER1_OVF_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}

}
/*
 * timer 1 compare match ISR
 * checking if the callback pointer is NULL
 * if not then execute the callback function on ISR entry
 * */


ISR(TIMER1_COMPA_vect){

	if(TIMER1_COMPA_callBackPtr != NULL)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*TIMER1_COMPA_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}

}





/*
 * timer 2 overflow ISR
 * checking if the callback pointer is NULL
 * if not then execute the callback function on ISR entry
 * */
ISR(TIMER2_OVF_vect){

	if(TIMER2_OVF_callBackPtr != NULL)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*TIMER2_OVF_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}



/*
 * timer 2 compare match ISR
 * checking if the callback pointer is NULL
 * if not then execute the callback function on ISR entry
 * */
ISR(TIMER2_COMP_vect){

	if(TIMER2_COMP_callBackPtr != NULL)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*TIMER2_COMP_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}


/*************************************************************************
* NAME :	uint8 Timers_INIT(const TIMER_configType * TIMER_configType)
*
* DESCRIPTION :	initialize any timer in any mode specified by the user
*
* INPUTS :	pointer to struct of type TIMER_configType
*
* OUTPUTS :	uint8 used for error and false entries handling
*
* NOTES :	none
***************************************************************************/
uint8 Timers_INIT(const TIMER_configType * TIMER_configType){

	switch(TIMER_configType->timer){

	case TIMER_0 :	/* timer 0 is selected */

			TCCR0 = (1<<FOC0); //non PWM mode and CLEAR the rest of the register
			TCCR0 = (TCCR0&0xF8) | (TIMER_configType->prescaler); // select the prescaler
			TCNT0=(uint8)TIMER_configType->initialValue; // start counting from initial value


			switch(TIMER_configType->timerMode){
			case OVERFLOW :
				/*
				 * non PWM mode set FOC0
				 * normal mode opertarion set both WGM01 and WGM00
				 *
				 * timer 0 is 8bit 256 ticks to overflow
				 * F_CPU = 8Mhz
				 * prescaler is variable
				 * F_TIMER = F_CPU/prescaler
				 *  could be : (assuming 0 initial value)
				 * 		Frequency 		Time		Interrupt every
				 *
				 * 		8Mhz		--	0.125us	--	32us
				 * 		1Mhz 		--	1us		--	256us
				 * 		125KHz		--	8us		--	2ms
				 * 		31.25Khz	--	32us	--	8.2ms
				 * 		7.8125Khz 	--	128us	--	32.78ms
				 *
				 * */
				TCCR0|=(1<<WGM01)|(1<<WGM00); // select normal mode

				TIMSK |= (1<<TOIE0); // enables timer 0 overflow interrupt

			break;
			case COMPARE_MATCH :


				TCCR0 |= (1<<WGM01); // select compare mode
				OCR0 = (uint8)TIMER_configType->compareValue;
				TIMSK |= (1<<OCIE0);	// enables timer 0 Compare interrupt

			break;


			default:
				return 15;  //15 means wrong timer mode

			} // End of the second switch
	break;  // case TIMER_0 break


	case TIMER_1 :    /* timer 1 is selected */

			TCCR1A = (1<<FOC1A); //non PWM mode and CLEAR the rest of the register
			TCCR1B = 0;
			TCCR1B = (TCCR1B&0xF8) | (TIMER_configType->prescaler); // select the prescaler
			TCNT1=TIMER_configType->initialValue; // start counting from initial value

			switch(TIMER_configType->timerMode){
			case OVERFLOW :
					/*
					 * non PWM mode set FOC0
					 * normal mode opertarion set both WGM01 and WGM00
					 *
					 * timer 1 is 16bit 65536 ticks to overflow
					 * F_CPU = 8Mhz
					 * prescaler is variable
					 * F_TIMER = F_CPU/prescaler
					 *  could be : (assuming 0 initial value)
					 * 		Frequency 		Time		Interrupt every
					 *
					 * 		8Mhz		--	0.125us	--	8.2ms
					 * 		1Mhz 		--	1us		--	65.5ms
					 * 		125KHz		--	8us		--	524.3ms
					 * 		31.25Khz	--	32us	--	2.1s
					 * 		7.8125Khz 	--	128us	--	8.4s
					 *
					 * */
				/*
				 * no editing of TCCR1B is needed because its already zeroed in line 144
				 * */

				TIMSK |= (1<<TOIE1); // enables timer 1 overflow interrupt

			break;
			case COMPARE_MATCH :


				TCCR1B |= (1<<WGM12); // select compare mode

				OCR1A = TIMER_configType->compareValue;  // no casting because OCR1A is 16 bit
				TIMSK |= (1<<OCIE1A);	// enables timer 1 Compare interrupt

			break;


			default:
				return 15;  //15 means wrong timer mode



				} // End of the 2nd swtich case
		break;   // case overflow break




	break;   // case TIMER_1 break


	case TIMER_2 :	/* timer 2 is selected */


		TCCR2 = (1<<FOC2); //non PWM mode and CLEAR the rest of the register
		TCCR2 = (TCCR2&0xF8) | (TIMER_configType->prescaler); // select the prescaler
		TCNT2=(uint8)TIMER_configType->initialValue; // start counting from initial value

		switch(TIMER_configType->timerMode){
		case OVERFLOW :
			/*
			 * non PWM mode set FOC0
			 * normal mode opertarion set both WGM01 and WGM00
			 *
			 * timer 2 is 8bit 256 ticks to overflow
			 * F_CPU = 8Mhz
			 * prescaler is variable
			 * F_TIMER = F_CPU/prescaler
			 *  could be : (assuming 0 initial value)
			 * 		Frequency 		Time		Interrupt every
			 *
			 * 		8Mhz		--	0.125us	--	32us
			 * 		1Mhz 		--	1us		--	256us
			 * 		125KHz		--	8us		--	2ms
			 * 		31.25Khz	--	32us	--	8.2ms
			 * 		7.8125Khz 	--	128us	--	32.78ms
			 *
			 * */
			TCCR2|=(1<<WGM21)|(1<<WGM20); // select normal mode

			TIMSK |= (1<<TOIE2); // enables timer 0 overflow interrupt

		break;
		case COMPARE_MATCH :


			TCCR2 |= (1<<WGM21); // select compare mode
			OCR2 = (uint8)TIMER_configType->compareValue;
			TIMSK |= (1<<OCIE2);	// enables timer 0 Compare interrupt

		break;


		default:
			return 15;  //15 means wrong timer mode   ******remember to remove magic numbers********



		} // end of second switch
	break;   //  case TIMER_2 break


	default:
		return 5; // 5 means wrong timer number
	}


return 0;  // 0 means no errors occured
} // End of INIT function


/*************************************************************************
* NAME :	void Timers_setTimer0OverFlowCallBack(void(*a_ptr)(void)
*
* DESCRIPTION :	assigns a function to the callback using pointers
*
* INPUTS :	pointer to function
*
* OUTPUTS :	void
*
* NOTES :	none
***************************************************************************/
void Timers_setTimer0OverFlowCallBack(void(*a_ptr)(void)){

	TIMER1_COMPA_callBackPtr = a_ptr;

}

/*************************************************************************
* NAME :	void Timers_setTimer0CompareCallBack(void(*a_ptr)(void))
*
* DESCRIPTION :	assigns a function to the callback using pointers
*
* INPUTS :	pointer to function
*
* OUTPUTS :	void
*
* NOTES :	none
***************************************************************************/
void Timers_setTimer0CompareCallBack(void(*a_ptr)(void)){

	TIMER1_COMPA_callBackPtr = a_ptr;

}

/*************************************************************************
* NAME :	void Timers_setTimer1OverFlowCallBack(void(*a_ptr)(void))
*
* DESCRIPTION :	assigns a function to the callback using pointers
*
* INPUTS :	pointer to function
*
* OUTPUTS :	void
*
* NOTES :	none
***************************************************************************/
void Timers_setTimer1OverFlowCallBack(void(*a_ptr)(void)){

	TIMER1_COMPA_callBackPtr = a_ptr;

}

/*************************************************************************
* NAME :	void Timers_setTimer1CompareCallBack(void(*a_ptr)(void))
*
* DESCRIPTION :	assigns a function to the callback using pointers
*
* INPUTS :	pointer to function
*
* OUTPUTS :	void
*
* NOTES :	none
***************************************************************************/
void Timers_setTimer1CompareCallBack(void(*a_ptr)(void)){

	TIMER1_COMPA_callBackPtr = a_ptr;

}

/*************************************************************************
* NAME :	void Timers_setTimer2OverFlowBack(void(*a_ptr)(void))
*
* DESCRIPTION :	assigns a function to the callback using pointers
*
* INPUTS :	pointer to function
*
* OUTPUTS :	void
*
* NOTES :	none
***************************************************************************/
void Timers_setTimer2OverFlowBack(void(*a_ptr)(void)){

	TIMER1_COMPA_callBackPtr = a_ptr;

}

/*************************************************************************
* NAME :	void Timers_setTimer2CompareCallBack(void(*a_ptr)(void))
*
* DESCRIPTION :	assigns a function to the callback using pointers
*
* INPUTS :	pointer to function
*
* OUTPUTS :	void
*
* NOTES :	none
***************************************************************************/
void Timers_setTimer2CompareCallBack(void(*a_ptr)(void)){

	TIMER1_COMPA_callBackPtr = a_ptr;

}

/*************************************************************************
* NAME :	void Timers_stop(_TIMER timer)
*
* DESCRIPTION :	stops the timer from counting
*
* INPUTS :	_TIMER which timer to be stopped
*
* OUTPUTS :	void
*
* NOTES :	none
***************************************************************************/
void Timers_stop(_TIMER timer){
	switch(timer){

	case TIMER_0 :

		TCCR0 = (TCCR0&0xF8) | 0; // select the prescaler

		break;
	case TIMER_1 :

		TCCR1B = (TCCR1B&0xF8) | 0;

		break;
	case TIMER_2 :

		TCCR2 = (TCCR2&0xF8) | 0;

		break;


	}
}


/*************************************************************************
* NAME :	void Timers_clearCounter(_TIMER timer)
*
* DESCRIPTION :	clears the timer counter value
*
* INPUTS :	_TIMER which timer to be cleared
*
* OUTPUTS :	void
*
* NOTES :	none
***************************************************************************/
void Timers_clearCounter(_TIMER timer){
	switch(timer){

	case TIMER_0 :

		TCNT0 = 0;

		break;
	case TIMER_1 :

		TCNT1 = 0 ;

		break;
	case TIMER_2 :

		TCNT2 = 0;

		break;


	}
}

/*************************************************************************
* NAME :	void Timers_resume(_TIMER timer,_TIMER_PRESCALER prescaler)
*
* DESCRIPTION :	resumes the timer with the desired prescaler
*
* INPUTS :	_TIMER which timer to be resumed , _TIMER_PRESCALER which prescaler to set
*
* OUTPUTS :	void
*
* NOTES :	none
***************************************************************************/
void Timers_resume(_TIMER timer,_TIMER_PRESCALER prescaler){
	switch(timer){

	case TIMER_0 :

		TCCR0 = (TCCR0&0xF8) | prescaler; // select the prescaler

		break;
	case TIMER_1 :

		TCCR1B = (TCCR1B&0xF8) | prescaler;

		break;
	case TIMER_2 :

		TCCR2 = (TCCR2&0xF8) | prescaler; // select the prescaler

		break;


	}
}
