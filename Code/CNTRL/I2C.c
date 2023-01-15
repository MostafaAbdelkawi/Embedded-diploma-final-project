/*
 * I2C.c
 *
 *  Created on: Nov 27, 2020
 *      Author: Mostafa
 */

#include "I2C.h"

/*************************************************************************
* NAME :	void I2C_init(const I2C_configType * I2C_Config_Ptr)
*
* DESCRIPTION :	initialize the I2C bit rate, prescaler and slave address
* 				specified by the user
*
* INPUTS :	pointer to struct of type I2C_configType
*
* OUTPUTS :	void
*
* NOTES :	none
***************************************************************************/
void I2C_init(const I2C_configType * I2C_Config_Ptr){



	TWBR = I2C_Config_Ptr->bitRate;   // set the bit rate

	TWSR = I2C_Config_Ptr->prescaler;     // TWI Bit Rate Prescaler

	TWAR = I2C_Config_Ptr->slaveModeAddress;  // Slave mode address

	TWCR = (1<<TWEN);    //enable the module

	PORTC |= (1<<PC0)|(1<<PC1);  // activate internal pull up on


}

/*************************************************************************
* NAME :	void I2C_start(void)
*
* DESCRIPTION :	sends start bit on the I2C bus
*
* INPUTS :	void
*
* OUTPUTS :	void
*
* NOTES :	none
***************************************************************************/
void I2C_start(void){

	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN); //Send START condition

	/*Wait for TWINT Flag set. This indicates that the START condition has been transmitted */
	while (BIT_IS_CLEAR(TWCR,TWINT));


}

/*************************************************************************
* NAME :	void I2C_sendData(uint8 data)
*
* DESCRIPTION :	sends a byte to the I2C bus
*
* INPUTS :	uint8 for the byte to be sent
*
* OUTPUTS :	void
*
* NOTES :	none
***************************************************************************/
void I2C_sendData(uint8 data){


	TWDR = data;

	TWCR = (1<<TWINT) | (1<<TWEN);

	/*Wait for TWINT Flag set. This indicates that the data has been transmitted
	 * and ACK/NACK has been received
	 */
	while (BIT_IS_CLEAR(TWCR,TWINT));


}


/*************************************************************************
* NAME :	uint8 I2C_receiveData(I2C_ACKorNACK ack)
*
* DESCRIPTION :	gets a byte from th I2C bus and sends ack or nack based on user selection
*
* INPUTS :	I2C_ACKorNACK  for selecting the desired ACK level
*
* OUTPUTS :	uint8 for the recieved data
*
* NOTES :	none
***************************************************************************/
uint8 I2C_receiveData(I2C_ACKorNACK ack){



	/*select receive with ACK or NACK based on user selection*/
	switch (ack){
		case NACK :
			TWCR = (1 << TWINT) | (1 << TWEN);
			break;
		case ACK :

			TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
			break;
	}

    /* Wait until data received successfully */
    while(BIT_IS_CLEAR(TWCR,TWINT));

    /* Read Data */
  //  *data = TWDR;
    return TWDR;

}


/*************************************************************************
* NAME :	uint8 I2C_getStatus(void)
*
* DESCRIPTION :	gets the I2C bus status and its used for error handling
*
* INPUTS :	void
*
* OUTPUTS :	uint8 for the current status
*
* NOTES :	none
***************************************************************************/
uint8 I2C_getStatus(void){

	/* status bits are the MS 5 bits , we have to mask the prescaler bits */
    return (TWSR & 0xF8);
}


/*************************************************************************
* NAME :	void I2C_stop(void)
*
* DESCRIPTION :	sends stop bit on the I2C bus
*
* INPUTS :	void
*
* OUTPUTS :	void
*
* NOTES :	none
***************************************************************************/
void I2C_stop(void){

	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);

}


/*************************************************************************
* NAME :	uint8 I2C_MTR_Seq(uint8 *data,uint8 SLA,I2C_RW R_W)
*
* DESCRIPTION :	it does the whole master transmit and receive sequence
*
* INPUTS :	pointer to the data to be sent or received
* 			uint8 for the slave address
* 			I2C_RW for specifying a read or write operation
*
* OUTPUTS :	uint8 for error handling
*
* NOTES :	this is used for MCU to MCU communications
* 			this is the sequence mentioned in the datasheet
***************************************************************************/
uint8 I2C_MTR_Seq(uint8 *data,uint8 SLA,I2C_RW R_W){

	uint8 SLA_R = (SLA<<1) |(0x01);
	uint8 SLA_W =(SLA<<1) & (0xFE) ;

	/* Start bit */
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	if ((TWSR & 0xF8) != START){

	return 1;

	}

	/*send the address to the bus*/
	switch(R_W){
	case R:
		TWDR = SLA_R;
		break;
	case W:
		TWDR = SLA_W;
		break;
	}

	TWCR = (1<<TWINT) | (1<<TWEN);

	while (!(TWCR & (1<<TWINT)));

	switch(R_W){
	case R:
		if ((TWSR & 0xF8) != MR_SLA_R_ACK){
			return 1;
		}

		break;
	case W:
		if ((TWSR & 0xF8) != MT_SLA_W_ACK){
			return 1;
		}

		break;
	}


	/* send or get  the data on the bus */
	switch(R_W){
	case R:
		*data = TWDR;
		break;
	case W:
		TWDR = *data;
		break;
	}

	TWCR = (1<<TWINT) | (1<<TWEN);

	while (!(TWCR & (1<<TWINT)));

	switch(R_W){
	case R:
		if ((TWSR & 0xF8) != MR_DATA_ACK){
			return 1;
		}
		break;
	case W:
		if ((TWSR & 0xF8) != MT_DATA_ACK){
			return 1;
		}
		break;
	}

	switch(R_W){
	case R:
		TWCR = (1 << TWINT) | (1 << TWEN);
		if ((TWSR & 0xF8) != MR_DATA_NACK){
			return 1;
		}
		break;
	case W:

		break;

	}

	/*stop bit*/
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);


	return 0;

}
