/*
 * UART.c
 *
 *  Created on: Nov 27, 2020
 *      Author: Mostafa
 */
#include "UART.h"


/*************************************************************************
* NAME :	void UART_init(const UART_configType * UART_Config_Ptr)
*
* DESCRIPTION :	initialize the UART baud rate, parity, data size and number of stop bits
*
* INPUTS :	pointer to struct of type UART_configType
*
* OUTPUTS :	void
*
* NOTES :	none
***************************************************************************/
void UART_init(const UART_configType * UART_Config_Ptr){


	uint16 BuadPrescaler = ((F_CPU / (UART_Config_Ptr->buadRate)) - 1);
	/* use double speed mode */
	SET_BIT(UCSRA,U2X);

	/* enable the rx and tx */
	SET_BIT(UCSRB,RXEN);
	SET_BIT(UCSRB,TXEN);

	/* URSEL must be 1 while writing to UCSRC */
	SET_BIT(UCSRC,URSEL);

	/* selecting Async mode */
	CLEAR_BIT(UCSRC,UMSEL);

	/* selecting frame options */
	UCSRC = 0;
		/*selecting Parity */
	UCSRC = (UCSRC & 0xCF) | ( (UART_Config_Ptr->parity) << UPM0 ); // UPM0 is bit 4
	/*selecting Data length */
		/* Not a 9bit data mode */
	CLEAR_BIT(UCSRB,UCSZ2);
	/* 5,6,7 or 8 data length mode */
	UCSRC = (UCSRC & 0xF9) | ( (UART_Config_Ptr->dataSize) << UCSZ0 ); //UCSZ0 is bit 1
	/*selecting stop bits */
	UCSRC = (UCSRC & 0xF7) | ( (UART_Config_Ptr->stopBit) << USBS ); //USBS is bit 3


	/* URSEL must be 0 while writing to UBRR */
	//CLEAR_BIT(UCSRC,URSEL);
	/*Selecting BuadRate : datasheet example on page 150*/
	UBRRH = BuadPrescaler>>8;
	UBRRL = BuadPrescaler;
}


/*************************************************************************
* NAME :	void UART_sendByte(const uint8 Byte)
*
* DESCRIPTION :	sends a byte to the UART bus
*
* INPUTS :	uint8 for the data to be sent
*
* OUTPUTS :	void
*
* NOTES :	none
***************************************************************************/
void UART_sendByte(const uint8 Byte){

	while(BIT_IS_CLEAR(UCSRA,UDRE)){}
	/* Put the required data in the UDR register and it also clear the UDRE flag as
	 * the UDR register is not empty now */
	UDR = Byte;

}


/*************************************************************************
* NAME :	uint8 UART_receiveByte(void)
*
* DESCRIPTION :	gets a byte from the UART bus
*
* INPUTS :	void
*
* OUTPUTS :	uint8 for the data to be received
*
* NOTES :	none
***************************************************************************/
uint8 UART_receiveByte(void){


	/* Read the received data from the Rx buffer (UDR) and the RXC flag
	   will be cleared after read this data */
	while(BIT_IS_CLEAR(UCSRA,RXC)){}

    return UDR;
}
