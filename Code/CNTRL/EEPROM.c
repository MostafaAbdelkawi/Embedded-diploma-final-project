/*
 * EEPROM.c
 *
 *  Created on: Nov 27, 2020
 *      Author: Mostafa
 */


#include "eeprom.h"


/*************************************************************************
* NAME :	void EEPROM_write(uint16 address, uint8 data)
*
* DESCRIPTION :	writes the given data to the given address
*
* INPUTS :	uint16 for the address
* 			uint8 for the data
*
* OUTPUTS :	Void
*
*
*
* NOTES :	the write operation is done using the I2C driver functions
***************************************************************************/
void EEPROM_write(uint16 address, uint8 data){

	I2C_start();

    I2C_sendData((uint8)(0xA0 | ((address & 0x0700)>>7)));

    I2C_sendData((uint8)(address));

    I2C_sendData(data);

    I2C_stop();

}

/*************************************************************************
* NAME :	void EEPROM_read(uint16 address,uint8 *data)
*
* DESCRIPTION :	gets  data from the given address
*
* INPUTS :	uint16 for the address
* 			pointer to uint8 for the data
*
* OUTPUTS :	Void
*
* NOTES :	the read operation is done using the I2C driver functions
***************************************************************************/
void EEPROM_read(uint16 address,uint8 *data){


	I2C_start();

    I2C_sendData((uint8)((0xA0) | ((address & 0x0700)>>7)));

    I2C_sendData((uint8)(address));

	I2C_start();

	I2C_sendData((uint8)((0xA0) | ((address & 0x0700)>>7) | 1));

	*data = I2C_receiveData(NACK /*,data*/);

    I2C_stop();


}
