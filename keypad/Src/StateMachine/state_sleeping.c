/*
 * state_processing.c
 *
 *  Created on: Jul 22, 2021
 *      Author: luk6xff
 */

#include "state_sleeping.h"
#include "main.h"
#include "Radio/radio.h"
#include <stdio.h>


//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void state_sleeping(SmCtx *sm)
{
	/* Disable Radio */
	radio_deinit();

	/* Disable Unused GPIOs */
	__HAL_RCC_GPIOB_CLK_DISABLE();
	__HAL_RCC_GPIOC_CLK_DISABLE();
	__HAL_RCC_GPIOD_CLK_DISABLE();
	__HAL_RCC_GPIOF_CLK_DISABLE();

	/* Disable SysTick. */
	HAL_SuspendTick();
	/* Request to enter STOP mode */
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

	/* WakeUp happens here */
	/* Enable SysTick. */
	HAL_ResumeTick();

	/* Clear PWR wake up Flag */
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

	/* ReInit GPIOs */
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	/* Resume radio from the sleep mode */
	radio_resume();

	/* Go to processing state */
	sm->current_state = Processing;
	printf("< processing >\r\n");
}

