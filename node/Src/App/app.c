/*
 * app.c
 *
 *  Created on: Jul 22, 2021
 *      Author: luk6xff
 */

#ifndef APP_C_
#define APP_C_

#include "app.h"
#include "main.h"
#include "Storage/eeprom.h"
#include "Radio/radio.h"
#include "../Leds/leds.h"
#include "../Relay/relay.h"
#include <stdio.h>
#include <string.h>

//------------------------------------------------------------------------------
static const uint32_t k_relay_toogle_time_ms = 400;
static const uint32_t k_led_toogle_time_ms = 100;

//------------------------------------------------------------------------------
static void handle_code_program_request(const radio_msg *msg);
static void handle_code_request(const radio_msg *msg);

static void app_programming_mode();
static void app_normal_mode();


//------------------------------------------------------------------------------
// PUBLIC FUNCTIONS
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void app_init()
{
	// Storage
	eeprom_init();
	// Radio
	radio_init();
}

//------------------------------------------------------------------------------
void app_run()
{
	app_programming_mode();
	app_normal_mode();
}


//------------------------------------------------------------------------------
// STATIC FUNCTIONS
//------------------------------------------------------------------------------

static void handle_code_program_request(const radio_msg *msg)
{
	if (msg->msg_type == MSG_CODE_PROGRAM_REQ)
	{
		if (eeprom_store_new_radio_code(msg->radio_cfg.code, sizeof(msg->radio_cfg.code), msg->radio_cfg.id, false))
		{
			printf("programming - New radio_code successfully stored! id:%lu\r\n", msg->radio_cfg.id);
			radio_msg res;
			res.msg_type = MSG_CODE_PROGRAM_RES;
			radio_send_msg(&res);
			led_toogle_loop(k_led_toogle_time_ms, 6);
		}
	}
}

//------------------------------------------------------------------------------
static void handle_code_request(const radio_msg *msg)
{
	if (msg->msg_type == MSG_CODE_REQ)
	{
		// Check if radio_code exists in the eeprom
		const int radio_code_id = eeprom_check_if_radio_code_exists(msg->radio_cfg.code);
		if (radio_code_id != -1)
		{
			printf("eeprom_check_if_radio_code_exists - true, id:%d\r\n", radio_code_id);
			// Toogle relay
			relay_toogle(k_relay_toogle_time_ms);
			// Send a successful response
			printf("Sending a MSG_CODE_RES to keypad\r\n");
			radio_msg res;
			res.msg_type = MSG_CODE_RES;
			radio_send_msg(&res);
			led_toogle_loop(k_led_toogle_time_ms, 3);
		}
		else
		{
			printf("eeprom_check_if_radio_code_exists - false\r\n");
		}
	}
}

//------------------------------------------------------------------------------
static void app_programming_mode()
{
	const uint32_t programming_start_ms = HAL_GetTick();
	const uint32_t programming_timeout_ms = 60000; // 60 seconds after reset
	uint32_t last_time_ms = programming_start_ms;

	printf("programming - mode started for: %lu seconds.\r\n", programming_timeout_ms/1000);
	while ((HAL_GetTick() - programming_start_ms) < programming_timeout_ms)
	{
		radio_msg msg;
		if (radio_read_msg(&msg))
		{
			printf("programming - new radio msg received, MSG_TYPE:%lu\r\n", msg.msg_type);
			// Handle both program and code requests
			handle_code_program_request(&msg);
			handle_code_request(&msg);
		}

		if ((HAL_GetTick() - last_time_ms) > 1000)
		{
			last_time_ms = HAL_GetTick();
			led_toogle(0);
		}
	}
}

//------------------------------------------------------------------------------
static void app_normal_mode()
{
	// Go to normal mode
	printf("normal - mode started!\r\n");
	led_enable();
	while (true)
	{
		radio_msg msg;
		if (radio_read_msg(&msg))
		{
			printf("normal - new radio msg received, MSG_TYPE:%lu\r\n", msg.msg_type);
			// Handle only code requests
			handle_code_request(&msg);
		}
	}
}


//------------------------------------------------------------------------------

#endif /* APP_C_ */
