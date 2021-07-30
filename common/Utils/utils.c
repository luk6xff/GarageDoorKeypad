/*
 * utils.c
 *
 *  Created on: Jul 26, 2021
 *      Author: luk6xff
 */

#include "main.h"
#include <stdio.h>

extern I2C_HandleTypeDef hi2c2;

//------------------------------------------------------------------------------
void i2c_scanner()
{
	/*--[ I2C Scanning Start ]--*/
	uint8_t space[] = " - ";
	uint8_t start_msg[] = "<<< I2C Scanning Start! >>> \r\n";
	uint8_t end_msg[] = "<<< I2C Scanning - Done! >>> \r\n\r\n";
	uint8_t i = 0, ret;
	HAL_Delay(1000);

	/*-[ I2C Bus Scanning ]-*/
	printf("%s", start_msg);
	for(i=1; i<128; i++)
	{
		ret = HAL_I2C_IsDeviceReady(&hi2c2, (uint16_t)(i<<1), 3, 5);
		if (ret != HAL_OK) /* No ACK Received At That Address */
		{
			printf("%s", space);
		}
		else if(ret == HAL_OK)
		{
			printf("0x%X", i);
		}
	}
	printf("%s", end_msg);
	/*--[ I2C Scanning Stop ]--*/
}

//------------------------------------------------------------------------------
