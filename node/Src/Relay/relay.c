/*
 * relay.c
 *
 *  Created on: Jul 28, 2021
 *      Author: luk6xff
 */

#include "relay.h"
#include "main.h"

//------------------------------------------------------------------------------
void relay_enable()
{
	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_SET);
}

//------------------------------------------------------------------------------
void relay_disable()
{
	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET);
}

//------------------------------------------------------------------------------
void relay_toogle(uint32_t toogle_time_ms)
{
	HAL_GPIO_TogglePin(RELAY_GPIO_Port, RELAY_Pin);
	if (toogle_time_ms != 0)
	{
		HAL_Delay(toogle_time_ms);
		HAL_GPIO_TogglePin(RELAY_GPIO_Port, RELAY_Pin);
	}
}

//------------------------------------------------------------------------------
