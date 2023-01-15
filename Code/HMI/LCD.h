/*
 * LCD.h
 *
 *  Created on: Oct 30, 2020
 *      Author: Mostafa
 */


/*
 * Preprocessors to protect the header file
 * from being included more than once in the same file
 */
#ifndef LCD_H_
#define LCD_H_

/*
 * includes the necessary files
 */
#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"


/*
 * Preprocessor defines to abstract the driver from the hardware
 * and eliminate the magic numbers for different instructions
 *
 */
#define LCD_dataPort PORTC

#define LCD_RS_Reg PORTD
#define LCD_RW_Reg PORTD
#define LCD_EN_Reg PORTD

#define LCD_CNTR_DIR_REG DDRD  /* CNTR = control */
#define LCD_DATA_DIR_REG DDRC

#define LCD_RS_Bit PD4
#define LCD_RW_Bit PD5
#define LCD_EN_Bit PD6

#define LCD_2x4_INIT_COMMAND 0x38
#define LCD_CUR_OFF_COMMAND 0x0C
#define LCD_CLEAR_SCREEN_COMMAND 0x01
#define SET_CURSOR_LOCATION_COMMAND 0x80


/************************************************************************************
*
*Function prototypes to enable the user to use the function on including the header
*
**************************************************************************************/
void LCD_sendCommand(uint8 command);
void LCD_displayCharacter(uint8 data);
void LCD_displayString(char *Str);
void LCD_init(void);
void LCD_clearScreen(void);
void LCD_displayInterger(int int_value);
void LCD_goToRowColumn(uint8 row,uint8 column);



#endif /* LCD_H_ */
