/*
 * leds.c
 *
 *  Created on: Jul 22, 2021
 *      Author: luk6xff
 */

#include "../KEYPAD_3x5/leds.h"

#include "main.h"

//extern UART_HandleTypeDef huart1;
//const uint8_t green_led[] = {"GREEN_LED\r\n"};
//const uint8_t red_led[] = {"RED_LED\r\n"};
//HAL_UART_Transmit(&huart1, green_led, sizeof(green_led), 100);


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
