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
static uint8_t current_radio_code_idx = 0;


//------------------------------------------------------------------------------
static void clear_radio_code()
{
	//printf("RADIO code clearing\r\n");
	memset(current_radio_code, BUTTON_NONE, sizeof(current_radio_code));
	current_radio_code_idx = 0;
	last_button_pressed_timestamp = 0;
}

//------------------------------------------------------------------------------
static void verify_radio_code()
{
	if (current_radio_code_idx == RADIO_CODE_SIZE)
	{
		printf("RADIO code provided: [0]:%d, [1]:%d, [2]:%d, [3]:%d\r\n", \
				current_radio_code[0], current_radio_code[1], current_radio_code[2], current_radio_code[3]);
		clear_radio_code();
	}
}

//------------------------------------------------------------------------------
void state_programming(SmCtx *sm)
{
	if (sm->last_pressed_btn != BUTTON_NONE)
	{
		const uint32_t timestamp_ms = HAL_GetTick();
		if (last_button_pressed_timestamp > 0 && (timestamp_ms - last_button_pressed_timestamp) > k_max_time_between_consecutive_btns_ms)
		{
			printf("Programming RADIO code timeout expired-> timestamp_ms:%d, last_button_pressed_timestamp:%d\r\n", timestamp_ms, last_button_pressed_timestamp);
			// Clear last provided radio code
			clear_radio_code();
		}
		else
		{
//			printf("RADIO code button pressed:%d\r\n", sm->last_pressed_btn);
//			printf("RADIO code current_radio_code_idx=%d\r\n", current_radio_code_idx);
			current_radio_code[current_radio_code_idx++] = sm->last_pressed_btn;
			last_button_pressed_timestamp = timestamp_ms;
			verify_radio_code();
		}
	}

	if (sm->last_pressed_btn == BUTTON_M)
	{
		// Clear last provided radio code
		uint8_t data[] = {1,2,3,4,5,6};
		radio_send_data(0, data, sizeof(data));
		clear_radio_code();
		sm->current_state = Processing;
	}
}

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
