/*
 * Main_CNTRL.c
 *
 *  Created on: Dec 1, 2020
 *      Author: Mostafa
 */
/*
 * include all the drivers header files to use their functions
 * */
#include "UART.h"

#include "Buzzer.h"

#include "EEPROM.h"

#include "I2C.h"

#include "Timer.h"

#include "motor.h"

#include "Main_CNTRL.h"


int main(void){

	/*UART configuration*/
	/*parity type ,number of stop bits , data length , buad rate*/
	UART_configType UART_config ={Even_Parity,_2bit,_8bit,115200};
	UART_init(&UART_config);

	/*I2C configuration*/
	/* prescaler , bit rate register , slave mode address*/
	I2C_configType I2C_config = {_1_I2C,2,0x05};   // for the EEPROM
	I2C_init(&I2C_config);


	/*timers configuration*/
	/*which timer , in which mode , its prescaler , its compare value , its initial value*/
	TIMER_configType TIMER_config = {TIMER_1,COMPARE_MATCH,_256_TIMER,31250,0};  // interrupt every 1s
	Timers_INIT(&TIMER_config);

	/*stop the timer and clear the counter*/
	Timers_stop(TIMER_1);
	Timers_clearCounter(TIMER_1);

	/*enable global interrupts */
	SREG|=(1<<7);
	/*key is a variable to store the operation code that the HMI is sending*/
	uint8 key;
	/*variable to keep track of loops*/
	uint8 i;

	/* making initial values for the EEPROM
	 * because at start it has garbage
	 * it must be zeroed at first in order to know if the user is new or old
	 * later we can store the EEPROM write cycles
	 *
	 * */
	for ( i = 0; i<PASSWORD_LENGTH;i++){

		EEPROM_write(PASSWORD_ADDRESS+i ,0);
		_delay_ms(10);

	}

	/*place holders for passwords stored in the EEPROM and the one come from the user*/
	uint8 EEPROMpassword[PASSWORD_LENGTH] ={0};
	uint8 userPassword[PASSWORD_LENGTH] ={0};


	_delay_ms(5000);    // to ensure that the HMI MCU is waiting for the ready signal


	UART_sendByte(checkNewUser()); // MC2 is ready
	while(1){

		key = UART_receiveByte();	// infinite loop waiting for HMI to operate

		switch(key){


		case STORE_PASSWORD:

			for (i = 0; i<PASSWORD_LENGTH;i++){

				EEPROM_write(PASSWORD_ADDRESS+i , UART_receiveByte());
				_delay_ms(10);

			}

			break;
		case VERIFY_PASSWORD:

			for (i = 0; i<PASSWORD_LENGTH;i++){

				userPassword[i] = UART_receiveByte();
				_delay_ms(10);
				EEPROM_read(PASSWORD_ADDRESS+i , &EEPROMpassword[i]);
				_delay_ms(10);

			}
			_delay_ms(250);
			/*compares the EEPROM password with the user input */
			if(compare_arrays(userPassword,EEPROMpassword,PASSWORD_LENGTH)){

				_delay_ms(20);
				UART_sendByte(PASSWORD_MATCH);

			}else{

				_delay_ms(20);
				UART_sendByte(PASSWORD_DOESNT_MATCH);

			}

			break;
		case BUZZER_ON:

			/*turns on the buzzer*/
			buzzerOn();
			/*setting timer 1 callback function to turn off the buzzer after 1 mint*/
			Timers_setTimer1CompareCallBack(buzzer_off);
			/*start the timer */
			Timers_resume(TIMER_1,_256_TIMER);

			break;

		case OPEN_DOOR:

			motor_instruction = MOTOR_FORWARD;
			Motor_ExecuteInstruction();
			Timers_setTimer1CompareCallBack(door_seq);
			Timers_resume(TIMER_1,_256_TIMER);
			UART_sendByte(DOOR_IS_OPENING);

			break;



		} // switch case


	} // supper loop

} // main




uint8 checkNewUser(void){

	uint8 i;
	uint8 buffer[PASSWORD_LENGTH] = {0};
	uint8 storedPassword[PASSWORD_LENGTH] = {0};

	for (i = 0; i<PASSWORD_LENGTH;i++){

		EEPROM_read(PASSWORD_ADDRESS+i , &storedPassword[i]);
		_delay_ms(10);

	}
	if(compare_arrays(buffer,storedPassword,PASSWORD_LENGTH)){
		return NEW_USER;
	}else{
		return OLD_USER;
	}


}


/*************************************************************************
* NAME :	bool compare_arrays(uint8 *Arr1 , uint8 *Arr2, uint8 size)
*
* DESCRIPTION :	compares 2 arrays and returns true if they match and false if they doesn't match
*
* INPUTS :	pointer to the first element of the first array
* 			pointer to the first element of the second array
*
* OUTPUTS :	boolean
*
*
* NOTES :	none
***************************************************************************/
bool compare_arrays(uint8 *Arr1 , uint8 *Arr2, uint8 size){

	uint8 i;
	for(i =0;i<size;i++){

		if(Arr1[i] == Arr2[i]){

		}else{
			return 0;
		}


	}

	return 1;
}





/*************************************************************************
 * NAME :	void buzzer_off(void)
 *
 * DESCRIPTION : the callback function that turns off the buzzer after 1 mint
 * 		and sends UART messages to sync the LCD screen
 *
 * INPUTS :		void
 *
 * OUTPUTS :	void
 *
 *
 * NOTES :	none
 ***************************************************************************/
void buzzer_off(void){

	static uint8 CallBack_counter = 0;
	if(CallBack_counter == BUZZER_CLOSE_TIME_STAMP){   //60 sec = 1mint  ********** remember to remove magic numbers later ***********
		buzzerOff();
		UART_sendByte(UNLOCK_SCREEN);
		_delay_ms(10);
		Timers_setTimer1CompareCallBack(NULL);  // Detach the callback
		CallBack_counter = 0;
	}
	CallBack_counter++;
}


/*************************************************************************
 * NAME :	void door_seq(void
 *
 * DESCRIPTION : the callback function that holds the door after 15 sec
 * 			for 4 sec then close it in 15 sec as well
 *
 * INPUTS :		void
 *
 * OUTPUTS :	void
 *
 *
 * NOTES :	none
 ***************************************************************************/
void door_seq(void){

	static uint8 CallBack_counter_2 = 0;
	if(CallBack_counter_2 == DOOR_HOLD_TIME_STAMP){   //15 sec   ********** remember to remove magic numbers later ***********
		motor_instruction = MOTOR_BREAK;	// set instruction to break
		Motor_ExecuteInstruction();			// execute the break instruction
		UART_sendByte(DOOR_IS_OPEN);		// send UART message to sync the LCD with motor movement


	}
	if(CallBack_counter_2 == DOOR_CLOSE_TIME_STAMP){
		motor_instruction = MOTOR_BACKWARD;
		Motor_ExecuteInstruction();
		UART_sendByte(DOOR_IS_CLOSING);

	}
	if(CallBack_counter_2 == DOOR_CLOSED_TIME_STAMP){

		motor_instruction = MOTOR_BREAK;
		Motor_ExecuteInstruction();

		UART_sendByte(DOOR_IS_CLOSE);
		UART_sendByte(DOOR_IS_CLOSE);
		CallBack_counter_2 = 0;
		Timers_setTimer1CompareCallBack(NULL);  // Detach the callback
	}
	CallBack_counter_2++;

}


