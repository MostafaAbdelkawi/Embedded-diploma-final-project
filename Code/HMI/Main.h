/*
 * Main.h
 *
 *  Created on: Dec 20, 2020
 *      Author: Mostafa
 */

#ifndef MAIN_H_
#define MAIN_H_


#define ENTER_PASSWORD_SCREEN 0
#define RE_ENTER_PASSWORD_SCREEN 1
#define VERIFY_PASSWORD_SCREEN 2

#define DEFAULT_COMMAND 0xFE
#define NEW_USER 0x01
#define OLD_USER 0x02
#define LOCK_SCREEN 0x03
#define PASSWORD_MATCH 0x04
#define PASSWORD_DOESNT_MATCH 0x05
#define UNLOCK_SCREEN 0x06
#define DOOR_IS_OPENING 0x07
#define DOOR_IS_OPEN 0x08
#define DOOR_IS_CLOSING 0x09
#define DOOR_IS_CLOSE 0x10


enum{

	ENTERPASS,REENTERPASS,SHOWMAIN,ASKFORPASSWORD,SCREENLOCKED
};

enum{

	STORE_PASSWORD,VERIFY_PASSWORD,BUZZER_ON,OPEN_DOOR
};

#define PASSWORD_LENGTH 5


void sendPassword(void);
void getPasswordScreen(uint8 re);
void showMainScreen(void);
uint8 askForPassword(void);

bool compare_arrays(uint8 *Arr1 , uint8 *Arr2 , uint8 size);



#endif /* MAIN_H_ */
