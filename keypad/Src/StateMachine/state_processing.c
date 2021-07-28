/*
 * state_processing.c
 *
 *  Created on: Jul 22, 2021
 *      Author: luk6xff
 */

#include "state_processing.h"
#include "states_common.h"
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
void state_processing(SmCtx *sm)
{
	if (sm->last_pressed_btn != BUTTON_NONE)
	{
		const uint32_t timestamp_ms = HAL_GetTick();
		if (last_button_pressed_timestamp > 0 && (timestamp_ms - last_button_pressed_timestamp) > k_max_time_between_consecutive_btns_ms)
		{
			printf("processing - RADIO code timeout expired-> timestamp_ms:%d, last_button_pressed_timestamp:%d\r\n", timestamp_ms, last_button_pressed_timestamp);
			// Clear last provided radio code
			clear_radio_code();
		}
		else
		{
//			printf("RADIO code button pressed:%d\r\n", sm->last_pressed_btn);
//			printf("RADIO code current_radio_code_idx=%d\r\n", current_radio_code_idx);
			current_radio_code[current_radio_code_idx++] = sm->last_pressed_btn;
			last_button_pressed_timestamp = timestamp_ms;
			// Check if provided code is full and valid
			if (verify_radio_code(current_radio_code, current_radio_code_idx))
			{
				const int radio_code_id = eeprom_check_if_radio_code_exists(current_radio_code);
				if (radio_code_id != -1)
				{
					printf("processing - eeprom_check_if_radio_code_exists - true\r\n");
					radio_msg msg;
					msg.msg_type = MSG_CODE_REQ;
					memcpy(&(msg.radio_code), &(eeprom_data_get_current()->radio_configs[radio_code_id]), sizeof(msg.radio_code));
					printf("processing - Sending a new radio msg: MSG_CODE_REQ...\r\n");
					radio_send_msg(&msg);
					// LU_TODO Wait for response
					//........
				}
			}

			if (current_radio_code_idx >= RADIO_CODE_SIZE)
			{
				clear_radio_code();
			}
		}
	}

	if (sm->last_pressed_btn == BUTTON_P)
	{
		// Clear last provided radio code
		clear_radio_code();
		sm->current_state = Programming;
		printf("< programming >\r\n");
	}
}
