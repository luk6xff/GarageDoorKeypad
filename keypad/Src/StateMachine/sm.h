/*
 * state_machine.h
 *
 *  Created on: Jul 22, 2021
 *      Author: luk6xff
 */

#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include "../KEYPAD_3x5/keypad3x5.h"

static const uint32_t k_max_time_between_consecutive_btns_ms = 3000;

typedef enum
{
	Processing,
	Sleeping,
	Programming
} State;

typedef struct
{
	KeypadButtonPressed last_pressed_btn;
	State previous_state;
	State current_state;
} SmCtx;


void sm_init();

void sm_run(const KeypadButtonPressed button);

#endif /* STATE_MACHINE_H_ */
