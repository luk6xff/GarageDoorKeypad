/*
 * leds.c
 *
 *  Created on: Jul 22, 2021
 *      Author: luk6xff
 */

#include "leds.h"
#include "main.h"

//------------------------------------------------------------------------------
void led_enable()
{
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
}

//------------------------------------------------------------------------------
void led_disable()
{
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
}

//------------------------------------------------------------------------------
void led_toogle(uint32_t toogle_time_ms)
{
	HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
	if (toogle_time_ms != 0)
	{
		HAL_Delay(toogle_time_ms);
		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
	}
}

//------------------------------------------------------------------------------
void led_toogle_loop(uint32_t toogle_time_ms, uint32_t loops_num)
{
	GPIO_PinState prev_state = HAL_GPIO_ReadPin(LED_RED_GPIO_Port, LED_RED_Pin);
	while (loops_num-- > 0)
	{
		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		HAL_Delay(toogle_time_ms);
		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		HAL_Delay(toogle_time_ms);
	}
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, prev_state);
}

//------------------------------------------------------------------------------

