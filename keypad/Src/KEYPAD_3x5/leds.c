/*
 * leds.c
 *
 *  Created on: Jul 22, 2021
 *      Author: luk6xff
 */

#include "../KEYPAD_3x5/leds.h"

#include "main.h"


//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
void led_disable_all()
{
	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
}

//------------------------------------------------------------------------------
void led_toogle(Leds led, uint32_t toogle_time_ms)
{

	if (led == LED_GREEN)
	{
		HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
		HAL_Delay(toogle_time_ms);
		HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
	}
	else if (led == LED_RED)
	{
		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		HAL_Delay(toogle_time_ms);
		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
	}
}

//------------------------------------------------------------------------------
void led_toogle_loop(Leds led, uint32_t toogle_time_ms, uint32_t loops_num)
{
	// Read prev state
	GPIO_PinState prev_state = GPIO_PIN_RESET;
	if (led == LED_GREEN)
	{
		prev_state = HAL_GPIO_ReadPin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
	}
	else if (led == LED_RED)
	{
		prev_state = HAL_GPIO_ReadPin(LED_RED_GPIO_Port, LED_RED_Pin);
	}

	// Toogle in loop
	while (loops_num-- > 0)
	{
		if (led == LED_GREEN)
		{
			HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
			HAL_Delay(toogle_time_ms);
			HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
		}
		else if (led == LED_RED)
		{
			HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
			HAL_Delay(toogle_time_ms);
			HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		}
		HAL_Delay(toogle_time_ms);
	}

	// Set prev state
	if (led == LED_GREEN)
	{
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, prev_state);
	}
	else if (led == LED_RED)
	{
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, prev_state);
	}
}

//------------------------------------------------------------------------------
