/*
 * state_machine.c
 *
 *  Created on: Jul 22, 2021
 *      Author: luk6xff
 */

#ifndef STATE_MACHINE_C_
#define STATE_MACHINE_C_

#include "sm.h"
#include "states.h"
#include "states_common.h"
#include "Storage/eeprom.h"
#include "Radio/radio.h"
#include <stdio.h>

//------------------------------------------------------------------------------
static SmCtx sm_ctx;

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void sm_init()
{
	/// Init libs
	// Keypad 3x5
	keypad3x5_init();
	// Storage
	eeprom_init();
	// Watchdog
	kick_watchdog();
	// Radio
	radio_init();

	// Clear SM ctx
	sm_ctx.last_pressed_btn = BUTTON_NONE;
	sm_ctx.previous_state = Processing;
	sm_ctx.current_state = Processing;
	printf("< processing >\r\n");
}

//------------------------------------------------------------------------------
void sm_run(const KeypadButtonPressed button)
{
	sm_ctx.previous_state = sm_ctx.current_state;
	sm_ctx.last_pressed_btn = button;

	kick_watchdog();

	switch (sm_ctx.current_state)
	{
		case Processing:
		{
			state_processing(&sm_ctx);
			break;
		}

		case Sleeping:
		{
			state_sleeping(&sm_ctx);
			break;
		}

		case Programming:
		{
			state_programming(&sm_ctx);
			break;
		}

		default:
		{
			break;
		}
	}
}

//------------------------------------------------------------------------------

#endif /* STATE_MACHINE_C_ */
