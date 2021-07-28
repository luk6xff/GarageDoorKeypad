/*
 * state_processing.c
 *
 *  Created on: Jul 22, 2021
 *      Author: luk6xff
 */

#include "state_processing.h"
#include "main.h"

void state_sleeping(SmCtx *sm)
{
	/*#### Disable all used wakeup sources: WKUP pin ###########################*/
	HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);
	HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN2);

	/*#### Clear all related wakeup flags ######################################*/
	/* Clear PWR wake up Flag */
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

	/* Enable WKUP pin */
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN2);

	/* Request to enter STANDBY mode */
	HAL_PWR_EnterSTANDBYMode();
}
