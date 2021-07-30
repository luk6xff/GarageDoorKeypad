/*
 * state_processing.c
 *
 *  Created on: Jul 22, 2021
 *      Author: luk6xff
 */

#include "state_sleeping.h"
#include "main.h"
#include "Radio/radio.h"

void state_sleeping(SmCtx *sm)
{
	// Disable Radio
	radio_deinit();

	/*#### Disable all used wakeup sources: WKUP pin ###########################*/
	HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);
	HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN2);

	__HAL_RCC_GPIOC_CLK_DISABLE();
	__HAL_RCC_GPIOF_CLK_DISABLE();
	__HAL_RCC_GPIOA_CLK_DISABLE();
	__HAL_RCC_GPIOB_CLK_DISABLE();
	__HAL_RCC_GPIOD_CLK_DISABLE();

	/*#### Clear all related wakeup flags ######################################*/
	/* Clear PWR wake up Flag */
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

	/* Enable WKUP pin */
	//HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
	//HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN2);

	/* Enable Ultra low power mode */
	//HAL_PWREx_EnableUltraLowPower();

	/* Enable the fast wake up from Ultra low power mode */
	//HAL_PWREx_EnableFastWakeUp();

	/* Request to enter STANDBY mode */
	HAL_PWR_EnterSTANDBYMode();
}
