/*
 * Main.c
 *
 *  Created on: Nov 27, 2020
 *      Author: Mostafa
 */
/*
 * include all the drivers header files to use their functions
 * */
#include "LCD.h"
#include "Keypad.h"
#include "UART.h"
#include "DIO.h"
/* application code header file */
#include "Main.h"

/*place holders for passwords*/
uint8 password[5] = {0};
uint8 password_repeated[5] = {0};


/*
 * the main flow is done using states which is described in an ENUM
 * each state sets the next state
 * and the super loop keeps going over states
 * */
uint8 state = 0;


uint8 instruction = 1; // 0 means open door  , 1 means change password remeber to remove magic numbers


int main(void){


	/*UART configuration*/
	/*parity type ,number of stop bits , data length , buad rate*/
	UART_configType UART_config = {Odd_Parity,_2bit,_8bit,115200};
	/* UART init passing the config struct by address */
	UART_init(&UART_config);

	/*8 bit data mode LCD init*/
	LCD_init();
	/* 4*4 keypad init*/
	Keypad_init();

	/* variable to hold the instruction coming from the control MCU*/
	uint8 MCU2_command = 0;

	/* ask if the user is new or old */
	MCU2_command =UART_receiveByte();
	switch (MCU2_command){

	case NEW_USER:

		state = ENTERPASS;
		break;
	case OLD_USER:
		state = SHOWMAIN;
		break;

	}

	while(1){


		switch(state){

		case ENTERPASS:

			state = REENTERPASS;
			_delay_ms(25);
			getPasswordScreen(ENTER_PASSWORD_SCREEN);    // enter password



			break;

		case REENTERPASS:

			_delay_ms(100);
			getPasswordScreen(RE_ENTER_PASSWORD_SCREEN);	// re-enter it

			/*if the passwords match send it to the control MCU to be stored in the EEPROM */
			if(compare_arrays(password,password_repeated,PASSWORD_LENGTH)){
				UART_sendByte(STORE_PASSWORD);
				_delay_ms(20);
				sendPassword();
				state = SHOWMAIN;
				DIO_WriteChannel(31,1);
			/*if the passwords don't match ask the user to enter the password again */
			}else{
				state = ENTERPASS;
			}

			break;

		case SHOWMAIN:

			showMainScreen();

			break;


		case ASKFORPASSWORD:

			askForPassword();
			break;

		case SCREENLOCKED:

			LCD_goToRowColumn(0,0); /* display the number every time at this position */

			LCD_displayString("Screen Locked!  ");

			LCD_goToRowColumn(1,0); /* display the number every time at this position */

			LCD_displayString("               ");

			/*locks the screen and go into super loop waiting fot the control MCU to free the lock*/
			if(UART_receiveByte() == UNLOCK_SCREEN){
				state = SHOWMAIN;
			}



			break;

		} // switch case

	} // super loop

} // main




/*************************************************************************
 * NAME :	void getPasswordScreen(uint8 re)
 *
 * DESCRIPTION :	shows the enter password different screens enter,re-enter and verify
 * 				and takes a 5 digit password from the user
 * 				the screen will save the password after the user presses the " = " button
 * 				which acts as enter button
 *
 *
 * INPUTS :	uint8 to know which LCD string to display
 *
 * OUTPUTS :	void
 *
 *
 * NOTES :	none
 ***************************************************************************/
void getPasswordScreen(uint8 re){

	uint8 prevKey ='0';
	uint8 key= '0';
	uint8 counter = 0;
	uint8 exitLoop = 0;

	LCD_clearScreen(); /* clear LCD at the beginning */
	switch(re){
	case ENTER_PASSWORD_SCREEN:
		LCD_displayString("Enter Password");
		break;
	case RE_ENTER_PASSWORD_SCREEN:
		LCD_displayString("Enter again");
		break;
	case VERIFY_PASSWORD_SCREEN:
		LCD_displayString("Verify Password");
		//LCD_displayInterger(state);
		break;
	}
	LCD_goToRowColumn(1,0); /* display the number every time at this position */

	while(1){


		if(exitLoop == 1){
			break;
		}else{

			key = Keypad_getKey();
			if((key != prevKey) && key != 'x'){   /*get only the first click of the button*/


				switch(key){
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':

					if(counter >4){

						//	LCD_goToRowColumn(0,0); /* display the number every time at this position */
						LCD_clearScreen(); /* clear LCD at the beginning */
						LCD_displayString("Maximum!");

					}else{

						switch(re){
						case 0:
							LCD_displayCharacter('*');
							password[counter] = key;
							counter++;
							break;
						case 1:
							LCD_displayCharacter('*');
							password_repeated[counter] = key;
							counter++;
							break;
						case 2:
							LCD_displayCharacter('*');
							password[counter] = key;
							counter++;
							break;
						}


					}

					break;
				case '=':
					exitLoop = 1;
					_delay_ms(150);
					break;
				}


			}
			prevKey = key;

		}



	}
}

/*************************************************************************
 * NAME :	void sendPassword(void)
 *
 * DESCRIPTION :	sends the user password via UART to the control MCU
 *
 * INPUTS :	void
 *
 * OUTPUTS :	void
 *
 *
 * NOTES :	delay is for safety regarding protues simulation
 ***************************************************************************/
void sendPassword(void){

	uint8 i;
	for(i =0; i<PASSWORD_LENGTH;i++){

		UART_sendByte(password[i]);
		_delay_ms(30);

	}

}

/*************************************************************************
 * NAME :	void showMainScreen(void)
 *
 * DESCRIPTION :	displays the main menu screen , which has open door and change password options
 * 				it monitors the user input and changes the state to the needed
 *
 *
 * INPUTS :	void
 *
 * OUTPUTS :	void
 *
 *
 * NOTES :	the option selection is done by pressing + for open door or - for change password
 ***************************************************************************/
void showMainScreen(void){


	LCD_goToRowColumn(0,0); /* display the number every time at this position */

	LCD_displayString("+ : Open door    ");

	LCD_goToRowColumn(1,0); /* display the number every time at this position */

	LCD_displayString("- : Change PW    ");

	uint8 prevKey ='0';
	uint8 key= '0';



	key = Keypad_getKey();
	if((key != prevKey) && key != 'x'){   /*get only the first click of the button*/


		switch(key){

		case '+':

			instruction = 0;
			state = ASKFORPASSWORD;


			break;
		case '-':

			instruction = 1;
			state = ASKFORPASSWORD;
			// askforPassword(1);

			break;
		}


	}
	prevKey = key;

}










/*************************************************************************
 * NAME :	uint8 askForPassword(void)
 *
 * DESCRIPTION :	takes a password from the user then sends it to the CNTRL MCU to verify it
 * 				then it executes the required instruction opening the door or changing the password
 * 				and updates the LCD screen accordingly
 * 				while keeping track of the false password entries and send a warning UART message
 * 				if the false counter is bigger than 3
 *
 * INPUTS :	void
 *
 * OUTPUTS :	uint8 for error handling
 *
 *
 * NOTES :	error handling part of the function is not done yet
 * 			false counter warning triggers on the 5th iteration
 ***************************************************************************/
uint8 askForPassword(void){

	static uint8 falseCounter = 0;
	uint8 message = 0;
	getPasswordScreen(VERIFY_PASSWORD_SCREEN);

	UART_sendByte(VERIFY_PASSWORD);


	_delay_ms(20);
	sendPassword();

	message = UART_receiveByte();
	//UART send to match the passwords
	if(message == PASSWORD_MATCH){				/* password matches EEPROM */


		falseCounter = 0;

		switch (instruction){

		case 0:
			UART_sendByte(OPEN_DOOR);	// UART send to open the door
			if(UART_receiveByte() == DOOR_IS_OPENING){	//show opening door
				LCD_goToRowColumn(0,0); /* display the number every time at this position */

				LCD_displayString("Door is opening!");

				LCD_goToRowColumn(1,0); /* display the number every time at this position */

				LCD_displayString("               ");
				_delay_ms(15);
			}
			if(UART_receiveByte() == DOOR_IS_OPEN){

				LCD_goToRowColumn(0,0); /* display the number every time at this position */

				LCD_displayString("Door is open!   ");

				LCD_goToRowColumn(1,0); /* display the number every time at this position */

				LCD_displayString("               ");
				_delay_ms(15);

			}
			if(UART_receiveByte() == DOOR_IS_CLOSING){

				LCD_goToRowColumn(0,0); /* display the number every time at this position */

				LCD_displayString("Door is closing!");

				LCD_goToRowColumn(1,0); /* display the number every time at this position */

				LCD_displayString("               ");
				_delay_ms(15);

			}
			if(UART_receiveByte() == DOOR_IS_CLOSE){

				_delay_ms(15);
				state = SHOWMAIN; // UART receive to go to main screen again
			}
			// UART receive to go to main screen again


			break;
		case 1:

			state = ENTERPASS;
			_delay_ms(30);
			break;

		}

	}
	//_delay_ms(500);
	else if (message == PASSWORD_DOESNT_MATCH){  // it doesn't




		if(falseCounter >2){


			UART_sendByte(BUZZER_ON);
			state = SCREENLOCKED;//LOCK screen
			falseCounter = 0;
			//UART send to turn on the buzzer
		}else{
			falseCounter++;
			state = ASKFORPASSWORD; // go to the next iteration there is no need for this line
			// as the state is already ASKFORPASSOWRD and nothing changed it
		}
	}

	return 0;
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
