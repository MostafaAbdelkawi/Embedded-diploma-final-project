/*
 * LCD.c
 *
 *  Created on: Oct 30, 2020
 *      Author: Mostafa
 */


/* include the header file of the driver */
#include "LCD.h"


uint8 cursorPos=0;    /*variable to store current cursor location*/


/************************************************************************************
* NAME :	void LCD_sendCommand(uint8 command)
*
* DESCRIPTION :	the input command and outputs it to 2*16 LCD in 8bit mode
*
* INPUTS :	(uint8) command the input command for the LCD
*
* OUTPUTS :	Void
*
* PROCESS pseudo code :
* 			 1-clears the RS bit, to select command mode
* 			 2-clears the RW bit, to select write mode
* 			 3-sets the EN bit, start of  EN pulses
* 			 4-send the command to the LCD , data can only be send within EN pulse
* 			 5-clears the EN bit, end of EN pulse
*
* NOTES :	all delay calls are written to follow the required timing for the LCD
**************************************************************************************/
void LCD_sendCommand(uint8 command){

	CLEAR_BIT(LCD_RS_Reg,LCD_RS_Bit) ;
	CLEAR_BIT(LCD_RW_Reg,LCD_RW_Bit);
	_delay_ms(1);
	SET_BIT(LCD_EN_Reg,LCD_EN_Bit);
	_delay_ms(1);
	LCD_dataPort = command;
	_delay_ms(1);
	CLEAR_BIT(LCD_EN_Reg,LCD_EN_Bit);
	_delay_ms(1);


}



/************************************************************************************
* NAME :	void LCD_displayCharacter(uint8 data)
*
* DESCRIPTION :	displays the input character
*
* INPUTS :	(uint8) data the character to be displayed on the LCD
*
* OUTPUTS :	Void
*
* PROCESS pseudo code :
* 			 1-sets the RS bit, to select data mode
* 			 2-clears the RW bit, to select write mode
* 			 3-sets the EN bit, start of  EN pulses
* 			 4-send the character to the LCD , data can only be send within EN pulse
* 			 5-clears the EN bit, end of EN pulse
*
* NOTES :	all delay calls are written to follow the required timing for the LCD
**************************************************************************************/
void LCD_displayCharacter(uint8 data){


	SET_BIT(LCD_RS_Reg,LCD_RS_Bit) ;
	CLEAR_BIT(LCD_RW_Reg,LCD_RW_Bit);
	_delay_ms(1);
	SET_BIT(LCD_EN_Reg,LCD_EN_Bit);
	_delay_ms(1);
	LCD_dataPort = data;
	_delay_ms(1);
	CLEAR_BIT(LCD_EN_Reg,LCD_EN_Bit);
	_delay_ms(1);

	/*cursorPos++;
	if(cursorPos == 16){
		LCD_goToRowColumn(1,0);

	}
	if(cursorPos == 32){
		LCD_goToRowColumn(0,0);
		cursorPos = 0;
	}*/

}



/************************************************************************************
* NAME :	void LCD_displayString(char *Str)
*
* DESCRIPTION :	displays the input string on the LCD making use of
* 				LCD_displayCharacter function
*
* INPUTS :	(char*) Str is a pointer, pointing at the first character
* 			of an array of characters i.e string
*
* OUTPUTS :	Void
*
* PROCESS pseudo code :
* 			 1-checks if the current char to be displayed is not the end of the string
* 			 2-loops the LCD_displayCharacter function as long as
* 			   the current char is'nt \0
*
* NOTES :	Str++ is possible because it is not a constant pointer
* 			 like the name of an array
**************************************************************************************/
void LCD_displayString(char *Str){

	while(*Str != '\0'){

		LCD_displayCharacter(*Str);
		Str++;

	}

}



/************************************************************************************
* NAME :	void LCD_clearScreen(void)
*
* DESCRIPTION :	clears the entire screen
*
* INPUTS :	Void
*
* OUTPUTS :	Void
*
* PROCESS pseudo code :
* 			 1-Uses LCD_sendCommand to send the clear screen command to the LCD
*
*
* NOTES :	none
**************************************************************************************/
void LCD_clearScreen(void){

	LCD_sendCommand(LCD_CLEAR_SCREEN_COMMAND);

}




/************************************************************************************
* NAME :	void LCD_displayInterger(int int_value)
*
* DESCRIPTION :	display an integer on the screen
*
* INPUTS :	(int) int_value ,the integer to be displayed
*
* OUTPUTS :	Void
*
* PROCESS pseudo code :
* 			 1-Uses itoa -integer to ASCII- from the standard library
* 			 	 to convert int data type to char
* 			 2-it saves each digit of the input int to and array of char
* 			 	 representing the number in its ASCII code
* 			 3-it then corrects the ghosting problem, making sure that
* 			 	the char array is always the same size
* 			 4-then it uses the function LCD_displayString to display it on the LCD
*
*
* NOTES :	ghosting problem happens when we try to write a new number that is lower
* 			in digit count than the previous one
* 			Ex: try to write 512 after writing  1023
* 				after displaying the LCD will show 5123 instead of 512
* 				because the new number is only 3 digits long
* 				 so the 3 won't be overwritten
**************************************************************************************/
void LCD_displayInterger(int int_value){

	   char buff[16]; /* String to hold the ascii result */
	   itoa(int_value,buff,10); /* 10 for decimal */
	   uint8 i;
	  /* switch(strlen(buff)){

	   case 1 :

		   strcat(buff,"   ");
		   break;
	   case 2 :

		   strcat(buff,"  ");
		   break;
	   case 3 :

		   strcat(buff," ");
		   break;
	   }
	   */
	   for(i = 0; i<(4-(strlen(buff)));i++){ /* the 4 in here is the maximum number of digits */
		   	   	   	   	   	   	   	   	   	 /* in our case is 4 digits with value of 1023 */
		   strcat(buff," ");

	   }
	   LCD_displayString(buff);

}



/************************************************************************************
* NAME :	void LCD_goToRowColumn(uint8 row,uint8 column)
*
* DESCRIPTION :	sets the cursor to a desired position
*
* INPUTS : (uint8) row and column , the desired position of the cursor
*
* OUTPUTS :	Void
*
* PROCESS pseudo code :
* 			 1-calculates the position according to the LCD datasheet
* 			 2-sends the final position command to the LCD using LCD_sendCommand
*
*
* NOTES :	none
**************************************************************************************/
void LCD_goToRowColumn(uint8 row,uint8 column){


	uint8 Location;

	switch(row)
	{
		case 0:
				Location=column;
				break;
		case 1:
				Location=column+0x40;
				break;
		case 2:
				Location=column+0x10;
				break;
		case 3:
				Location=column+0x50;
				break;
	}


	LCD_sendCommand(Location | SET_CURSOR_LOCATION_COMMAND);

}



/************************************************************************************
* NAME :	void LCD_init(void)
*
* DESCRIPTION :	initialize the pins for the LCD
* 				and tells the LCD processor which mode to use
*
* INPUTS : Void
*
* OUTPUTS :	Void
*
* PROCESS pseudo code :
* 			 1-makes the pins output for LCD data and control lines
* 			 2- using LCD_sendCommand function is send the current mode
* 			 	and other options
*
*
* NOTES :	none
**************************************************************************************/
void LCD_init(void){

	LCD_CNTR_DIR_REG = (1<<LCD_RS_Bit)|(1<<LCD_RW_Bit)|(1<<LCD_EN_Bit);
	LCD_DATA_DIR_REG = 0xFF;
	LCD_sendCommand(LCD_2x4_INIT_COMMAND);
	LCD_sendCommand(LCD_CUR_OFF_COMMAND);
	LCD_sendCommand(LCD_CLEAR_SCREEN_COMMAND);

}






