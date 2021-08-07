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
// Go to sleep counter
static const uint32_t k_go_to_processing_timeout_ms = 20000; // 20[s]
static uint32_t go_to_processing_counter_start_ms = 0;

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
	// In programming state, keep LED_RED light on;
	led_enable(LED_RED);

	if (sm->last_pressed_btn == BUTTON_NONE)
	{
		if (go_to_processing_counter_start_ms == 0)
		{
			go_to_processing_counter_start_ms = HAL_GetTick();
		}

		if ((HAL_GetTick() - go_to_processing_counter_start_ms) > k_go_to_processing_timeout_ms)
		{
			go_to_processing_counter_start_ms = 0;
			clear_radio_code();
			sm->current_state = Processing;
			led_disable_all();
			printf("< processing >\r\n");
		}
	}
	else  // if (sm->last_pressed_btn != BUTTON_NONE)
	{
		// Clear go to processing counter
		go_to_processing_counter_start_ms = 0;
		// Blink LED if valid button pressed
		led_toogle(LED_GREEN, 20);

		const uint32_t timestamp_ms = HAL_GetTick();
		if (last_button_pressed_timestamp > 0 && (timestamp_ms - last_button_pressed_timestamp) > k_max_time_between_consecutive_btns_ms)
		{
			printf("programming - RADIO code timeout expired, jump to Processing-> timestamp_ms:%lu, last_button_pressed_timestamp:%lu\r\n", timestamp_ms, last_button_pressed_timestamp);
			clear_radio_code();
			sm->current_state = Processing;
			led_disable_all();
			printf("< processing >\r\n");
		}
		else
		{
			if (current_radio_code_id_button == BUTTON_NONE)
			{
				current_radio_code_id_button = sm->last_pressed_btn;
				last_button_pressed_timestamp = timestamp_ms;
			}

			else if (sm->last_pressed_btn != BUTTON_M && sm->last_pressed_btn != BUTTON_ARROW_UP)
			{
				current_radio_code[current_radio_code_idx++] = sm->last_pressed_btn;
				last_button_pressed_timestamp = timestamp_ms;
			}

			else if (sm->last_pressed_btn == BUTTON_M || sm->last_pressed_btn == BUTTON_ARROW_UP)
			{
				// Check if provided code is full and valid
				if (verify_radio_code(current_radio_code, current_radio_code_idx))
				{
					printf("programming - verify_radio_code success\r\n");
					// Check if new code already exists, if so, let's fail
					if (eeprom_check_if_radio_code_exists(current_radio_code) == -1)
					{
						radio_msg msg;
						msg.msg_type = MSG_CODE_PROGRAM_REQ;
						msg.radio_cfg.id = button_id_to_radio_config_id(current_radio_code_id_button);
						memcpy(msg.radio_cfg.code, current_radio_code, sizeof(msg.radio_cfg.code));

						/**
						 * @note If save was done by BUTTON_M, you have to check if code id already exist before sending message.
						 * If you want to skip this check run storing by pressing BUTTON_ARROW_UP
						 */
						if (sm->last_pressed_btn == BUTTON_M)
						{
							if (eeprom_check_if_radio_code_id_exists(msg.radio_cfg.id))
							{
								printf("eeprom - There is already a radio_code id:%lu registered. Stopping registering a new one!\r\nUse a BUTTON_ARROW_UP to overwrite the old one\r\n", msg.radio_cfg.id);
								led_toogle_loop(LED_RED, k_led_toogle_time_ms, 4);
								goto exit;
							}
						}

						printf("programming - Sending a new radio msg: MSG_CODE_PROGRAM_REQ...\r\n");
						radio_send_msg(&msg);
						led_toogle_loop(LED_GREEN, k_led_toogle_time_ms, 1);
						// Wait for a response from the NODE
						const uint32_t start_ms = HAL_GetTick();
						const uint32_t timeout_ms = k_wait_for_response_from_node_timeout_ms;
						bool response_received = false;
						while ((HAL_GetTick() - start_ms) < timeout_ms)
						{
							kick_watchdog();
							if (radio_read_msg(&msg))
							{
								printf("programming - Radio response received! MSG_TYPE:%lu\r\n", msg.msg_type);
								if (msg.msg_type == MSG_CODE_PROGRAM_RES)
								{
									response_received = true;
									break;
								}
							}
						}
						// Any response from the NODE received
						if (response_received)
						{
							printf("programming - Radio response MSG_CODE_PROGRAM_RES successfully received, storing new code!\r\n");
							if (eeprom_store_new_radio_code(current_radio_code, sizeof(current_radio_code), button_id_to_radio_config_id(current_radio_code_id_button)))
							{
								printf("programming - eeprom_store_new_radio_code success!\r\n");
								led_toogle_loop(LED_GREEN, k_led_toogle_time_ms, 3);
							}
							else
							{
								printf("programming - eeprom_store_new_radio_code failed!\r\n");
								led_toogle_loop(LED_RED, k_led_toogle_time_ms, 4);
							}
						}
						else
						{
							printf("programming- No Radio response MSG_CODE_PROGRAM_RES received during timeout\r\n");
							led_toogle_loop(LED_RED, k_led_toogle_time_ms, 3);
						}
					}
					else
					{
						printf("programming- This radio code already exists! Cannot store it!\r\n");
						led_toogle_loop(LED_RED, k_led_toogle_time_ms, 5);
					}
				}
				else
				{
					printf("programming - verify_radio_code failed!\r\n");
					led_toogle_loop(LED_RED, k_led_toogle_time_ms, 2);
				}
exit:
				// Clear last provided radio code
				clear_radio_code();
				sm->current_state = Processing;
				// Disable leds
				led_disable_all();
				printf("< processing >\r\n");
			}
		}

		// If BUTTON_ESC, clean all and go to processing
		if (sm->last_pressed_btn == BUTTON_ESC)
		{
			// Clear last provided radio code
			clear_radio_code();
			led_toogle_loop(LED_RED, k_led_toogle_time_ms, 3);
			// Disable leds
			led_disable_all();
			sm->current_state = Processing;
			printf("< processing >\r\n");
		}
	}
}

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
