/*
 * UART.h
 *
 *  Created on: Nov 27, 2020
 *      Author: Mostafa
 */

#ifndef UART_H_
#define UART_H_

#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"




typedef enum
{
	Disabled,Reserved,Even_Parity,Odd_Parity

}UART_parity;

typedef enum
{
	_1bit,_2bit

}UART_stopBits;

typedef enum
{
	_5bit , _6bits , _7bit  , _8bit

}UART_dataSize;

typedef struct
{
	UART_parity parity;
	UART_stopBits stopBit;
	UART_dataSize dataSize;
	uint32 buadRate;

}UART_configType;


void UART_init(const UART_configType * Config_Ptr);

void UART_sendByte(const uint8 Byte);

uint8 UART_receiveByte(void);


#endif /* UART_H_ */
