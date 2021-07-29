/*
 * utils.c
 *
 *  Created on: Jul 26, 2021
 *      Author: luk6xff
 */

#include "main.h"

extern I2C_HandleTypeDef hi2c2;
extern UART_HandleTypeDef huart1;

//------------------------------------------------------------------------------
void i2c_scanner()
{
	/*--[ I2C Scanning Start ]--*/
	uint8_t buf[25] = {0};
	uint8_t space[] = " - ";
	uint8_t start_msg[] = "<<< I2C Scanning Start! >>> \r\n";
	uint8_t end_msg[] = "<<< I2C Scanning - Done! >>> \r\n\r\n";
	uint8_t i = 0, ret;
	HAL_Delay(1000);

	/*-[ I2C Bus Scanning ]-*/
	HAL_UART_Transmit(&huart1, start_msg, sizeof(start_msg), 10000);
	for(i=1; i<128; i++)
	{
		ret = HAL_I2C_IsDeviceReady(&hi2c2, (uint16_t)(i<<1), 3, 5);
		if (ret != HAL_OK) /* No ACK Received At That Address */
		{
			HAL_UART_Transmit(&huart1, space, sizeof(space), 10000);
		}
		else if(ret == HAL_OK)
		{
			sprintf(buf, "0x%X", i);
			HAL_UART_Transmit(&huart1, buf, sizeof(buf), 10000);
		}
	}
	HAL_UART_Transmit(&huart1, end_msg, sizeof(end_msg), 10000);
	/*--[ I2C Scanning Stop ]--*/
}

//------------------------------------------------------------------------------
