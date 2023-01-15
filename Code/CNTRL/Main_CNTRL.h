/*
 * Main_CNTRL.h
 *
 *  Created on: Dec 20, 2020
 *      Author: Mostafa
 */

#ifndef MAIN_CNTRL_H_
#define MAIN_CNTRL_H_

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

#define DOOR_HOLD_TIME_STAMP 15
#define DOOR_CLOSE_TIME_STAMP 19
#define DOOR_CLOSED_TIME_STAMP 34
#define BUZZER_CLOSE_TIME_STAMP 60

#define PASSWORD_ADDRESS 0x0311   //311-312-313-314-315
#define PASSWORD_LENGTH 5

enum{

	STORE_PASSWORD,VERIFY_PASSWORD,BUZZER_ON,OPEN_DOOR
};


uint8 checkNewUser(void);
bool compare_arrays(uint8 *Arr1 , uint8 *Arr2, uint8 size);

void door_seq(void);
void buzzer_off(void);

#endif /* MAIN_CNTRL_H_ */
