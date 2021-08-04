/*
 * state_processing.c
 *
 *  Created on: Jul 22, 2021
 *      Author: luk6xff
 */

#include "state_processing.h"
#include "states_common.h"
#include "Storage/eeprom.h"
#include "Radio/radio.h"
#include "../KEYPAD_3x5/leds.h"
#include "main.h"
#include <stdio.h>


//------------------------------------------------------------------------------
// Go to sleep counter
static const uint32_t k_go_to_sleep_timeout_ms = 3000;//20000; // 20[s]
static uint32_t go_to_sleep_counter_start_ms = 0;

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
	if (sm->last_pressed_btn == BUTTON_NONE)
	{
		if (go_to_sleep_counter_start_ms == 0)
		{
			go_to_sleep_counter_start_ms = HAL_GetTick();
		}

		if ((HAL_GetTick() - go_to_sleep_counter_start_ms) > k_go_to_sleep_timeout_ms)
		{
			go_to_sleep_counter_start_ms = 0;
			clear_radio_code();
			sm->current_state = Sleeping;
			printf("< sleeping >\r\n");
		}
	}
	else  // if (sm->last_pressed_btn != BUTTON_NONE)
	{
		// Clear go to sleeping timer
		go_to_sleep_counter_start_ms = 0;
		// Blink LED if valid button pressed
		led_toogle(LED_GREEN, 20);

		const uint32_t timestamp_ms = HAL_GetTick();
		if (last_button_pressed_timestamp > 0 && (timestamp_ms - last_button_pressed_timestamp) > k_max_time_between_consecutive_btns_ms)
		{
			printf("processing - RADIO code timeout expired-> timestamp_ms:%lu, last_button_pressed_timestamp:%lu\r\n", timestamp_ms, last_button_pressed_timestamp);
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
					memcpy(&(msg.radio_cfg), &(eeprom_data_get_current()->radio_configs[radio_code_id]), sizeof(msg.radio_cfg));
					printf("processing - Sending a new radio msg: MSG_CODE_REQ...\r\n");
					radio_send_msg(&msg);
					led_toogle_loop(LED_GREEN, k_led_toogle_time_ms, 1);
					// Wait for a response from node
					const uint32_t start_ms = HAL_GetTick();
					const uint32_t timeout_ms = k_wait_for_response_from_node_timeout_ms;
					bool response_received = false;
					while ((HAL_GetTick() - start_ms) < timeout_ms)
					{
						if (radio_read_msg(&msg))
						{
							printf("processing - Radio response received! MSG_TYPE:%lu\r\n", msg.msg_type);
							if (msg.msg_type == MSG_CODE_RES)
							{
								response_received = true;
								break;
							}
						}
					}

					if (response_received)
					{
						printf("processing - Radio response MSG_CODE_RES successfully received\r\n");
						led_toogle_loop(LED_GREEN, k_led_toogle_time_ms, 3);
					}
					else
					{
						printf("processing - No Radio MSG_CODE_RES response received during timeout\r\n");
						led_toogle_loop(LED_RED, k_led_toogle_time_ms, 3);
					}
				}
				else
				{
					printf("processing - Non existing radio code provided\r\n");
					led_toogle_loop(LED_RED, k_led_toogle_time_ms, 3);
				}
			}

			if (current_radio_code_idx >= RADIO_CODE_SIZE)
			{
				// Check if clear code has been applied
				verify_and_clear_radio_codes(current_radio_code, current_radio_code_idx);
				clear_radio_code();
			}
		}

		// If BUTTON_P, go to programming session.
		if (sm->last_pressed_btn == BUTTON_P)
		{
			// Clear last provided radio code
			clear_radio_code();
			sm->current_state = Programming;
			printf("< programming >\r\n");
		}
	}
}
