/*
 * state_processing.c
 *
 *  Created on: Jul 22, 2021
 *      Author: luk6xff
 */

#include "state_programming.h"
#include "states_common.h"
#include "Storage/eeprom.h"
#include "Radio/radio.h"
#include "../KEYPAD_3x5/leds.h"
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
static uint8_t button_id_to_radio_config_id(KeypadButtonPressed btn)
{
	uint8_t radio_id = 0xFF;

	switch (btn)
	{
		case BUTTON_0:
			radio_id = 0;
			break;
		case BUTTON_1:
			radio_id = 1;
			break;
		case BUTTON_2:
			radio_id = 2;
			break;
		case BUTTON_3:
			radio_id = 3;
			break;
		case BUTTON_4:
			radio_id = 4;
			break;
		case BUTTON_5:
			radio_id = 5;
			break;
		case BUTTON_6:
			radio_id = 6;
			break;
		case BUTTON_7:
			radio_id = 7;
			break;
		case BUTTON_8:
			radio_id = 8;
			break;
		case BUTTON_9:
			radio_id = 9;
			break;
	}

	return radio_id;
}

//------------------------------------------------------------------------------
void state_programming(SmCtx *sm)
{
	if (sm->last_pressed_btn != BUTTON_NONE)
	{
		const uint32_t timestamp_ms = HAL_GetTick();
		if (last_button_pressed_timestamp > 0 && (timestamp_ms - last_button_pressed_timestamp) > k_max_time_between_consecutive_btns_ms)
		{
			printf("programming - RADIO code timeout expired, jump to Processing-> timestamp_ms:%d, last_button_pressed_timestamp:%d\r\n", timestamp_ms, last_button_pressed_timestamp);
			// Clear last provided radio code
			clear_radio_code();
			sm->current_state = Processing;
			printf("< processing >\r\n");
		}
		else
		{
			if (current_radio_code_id_button == BUTTON_NONE)
			{
				current_radio_code_id_button = sm->last_pressed_btn;
				last_button_pressed_timestamp = timestamp_ms;
			}

			else if (sm->last_pressed_btn != BUTTON_M)
			{
				current_radio_code[current_radio_code_idx++] = sm->last_pressed_btn;
				last_button_pressed_timestamp = timestamp_ms;
			}

			else if (sm->last_pressed_btn == BUTTON_M)
			{
				// Check if provided code is full and valid
				if (verify_radio_code(current_radio_code, current_radio_code_idx))
				{
					printf("programming - verify_radio_code success\r\n");
					// Try to store new code
					if (eeprom_store_new_radio_code(current_radio_code, sizeof(current_radio_code), button_id_to_radio_config_id(current_radio_code_id_button)))
					{
						// LU_TODO Send frame to a NODE and wait for response
						radio_msg msg;
						msg.msg_type = MSG_CODE_PROGRAM_REQ;
						memcpy(&(msg.radio_cfg), &(eeprom_data_get_current()->radio_configs[button_id_to_radio_config_id(current_radio_code_id_button)]), sizeof(msg.radio_cfg));
						printf("programming - Sending a new radio msg: MSG_CODE_PROGRAM_REQ...\r\n");
						radio_send_msg(&msg);
						const uint32_t start_ms = HAL_GetTick();
						const uint32_t timeout_ms = 3000;
						bool response_received = false;
						while ((HAL_GetTick() - start_ms) < timeout_ms)
						{
							if (radio_read_msg(&msg))
							{
								printf("programming - Radio response received! MSG_TYPE:%d\r\n", msg.msg_type);
								if (msg.msg_type == MSG_CODE_PROGRAM_RES)
								{
									response_received = true;
									break;
								}
							}
						}
						if (response_received)
						{
							printf("programming - Radio response MSG_CODE_PROGRAM_RES successfully received\r\n");
							led_toogle(LED_GREEN, 200);
							led_toogle(LED_GREEN, 200);
							led_toogle(LED_GREEN, 200);
						}
						else
						{
							printf("programming- No Radio response MSG_CODE_PROGRAM received during timeout\r\n");
							led_toogle(LED_RED, 200);
							led_toogle(LED_RED, 200);
							led_toogle(LED_RED, 200);
						}
					}
					else
					{
						printf("programming - eeprom_store_new_radio_code failed!\r\n");
					}
				}
				else
				{
					printf("programming - verify_radio_code failed!\r\n");
				}
				// Clear last provided radio code
				clear_radio_code();
				//sm->current_state = Processing;
				//printf("< processing >\r\n");
				sm->current_state = Sleeping;
				printf("< sleeping >\r\n");
			}
		}
	}
}

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
