/*
 * state_processing.c
 *
 *  Created on: Jul 22, 2021
 *      Author: luk6xff
 */

#include "state_programming.h"
#include "../Storage/eeprom.h"
#include "../Radio/radio.h"
#include "main.h"
#include <stdio.h>

//------------------------------------------------------------------------------

// Last button pressed timestamp
static uint32_t last_button_pressed_timestamp = 0;

// Radio code
static KeypadButtonPressed current_radio_code[RADIO_CODE_SIZE] = {BUTTON_NONE};
static KeypadButtonPressed current_radio_code_id_button = BUTTON_NONE;
static uint8_t current_radio_code_idx = 0;


//------------------------------------------------------------------------------
static void clear_radio_code()
{
	//printf("RADIO code clearing\r\n");
	current_radio_code_id_button = BUTTON_NONE;
	memset(current_radio_code, BUTTON_NONE, sizeof(current_radio_code));
	current_radio_code_idx = 0;
	last_button_pressed_timestamp = 0;
}

//------------------------------------------------------------------------------
static bool verify_radio_code()
{
	bool ret = true;

	if (current_radio_code_idx == RADIO_CODE_SIZE)
	{
		for (size_t i = 0; i < RADIO_CODE_SIZE; ++i)
		{
			if (current_radio_code[i] == BUTTON_P || \
				current_radio_code[i] == BUTTON_M ||
				current_radio_code[i] == BUTTON_ESC	||
				current_radio_code[i] == BUTTON_ARROW_UP	||
				current_radio_code[i] == BUTTON_ARROW_DOWN	||
				current_radio_code[i] == BUTTON_NONE)
			{
				printf("RADIO code provided does not contain only digits [0-9]");
				ret = false;
			}
		}
		printf("RADIO code provided: [0]:%d, [1]:%d, [2]:%d, [3]:%d\r\n", \
				current_radio_code[0], current_radio_code[1], current_radio_code[2], current_radio_code[3]);
	}
	else
	{
		ret = false;
	}
	return ret;
}

//------------------------------------------------------------------------------
void state_programming(SmCtx *sm)
{
	if (sm->last_pressed_btn != BUTTON_NONE)
	{
		const uint32_t timestamp_ms = HAL_GetTick();
		if (last_button_pressed_timestamp > 0 && (timestamp_ms - last_button_pressed_timestamp) > k_max_time_between_consecutive_btns_ms)
		{
			printf("Programming RADIO code timeout expired, jump to Processing-> timestamp_ms:%d, last_button_pressed_timestamp:%d\r\n", timestamp_ms, last_button_pressed_timestamp);
			// Clear last provided radio code
			clear_radio_code();
			sm->current_state = Processing;
		}
		else
		{
			if (current_radio_code_id_button == BUTTON_NONE)
			{
				current_radio_code_id_button = sm->last_pressed_btn;
			}
			else
			{
				current_radio_code[current_radio_code_idx++] = sm->last_pressed_btn;
			}
//			printf("RADIO code button pressed:%d\r\n", sm->last_pressed_btn);
//			printf("RADIO code current_radio_code_idx=%d\r\n", current_radio_code_idx);
			last_button_pressed_timestamp = timestamp_ms;
		}
	}

	if (sm->last_pressed_btn == BUTTON_M)
	{
		// Check if provided code is full and valid
		if (verify_radio_code())
		{
			// Try to store new code
			if (eeprom_store_new_radio_code(current_radio_code, sizeof(current_radio_code), current_radio_code_id_button))
			{
				// LU_TODO Send frame to a NODE and wait for response
				radio_send_data(0, current_radio_code, sizeof(current_radio_code));
			}
		}
		// Clear last provided radio code
		clear_radio_code();
		sm->current_state = Processing;
	}
}

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
