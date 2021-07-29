/*
 * leds.c
 *
 *  Created on: Jul 22, 2021
 *      Author: luk6xff
 */

#include "leds.h"
#include "main.h"


void led_enable(Leds led)
{
	if (led == LED_GREEN)
	{
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
	}
	else if (led == LED_RED)
	{
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
	}
}

void led_disable(Leds led)
{
	if (led == LED_GREEN)
	{
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
	}
	else if (led == LED_RED)
	{
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
	}
}

void led_toogle(Leds led, uint32_t toogle_time_ms)
{

	if (led == LED_GREEN)
	{
		HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
		if (toogle_time_ms != 0)
		{
			HAL_Delay(toogle_time_ms);
			HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
		}
	}
	else if (led == LED_RED)
	{
		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		if (toogle_time_ms != 0)
		{
			HAL_Delay(toogle_time_ms);
			HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
		}
	}


}
