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
static const uint32_t k_relay_toogle_time_ms = 200;

//------------------------------------------------------------------------------
static void app_programming_mode()
{
	const uint32_t programming_start_ms = HAL_GetTick();
	const uint32_t programming_timeout_ms = 30000; // 30 seconds after reset
	uint32_t last_time_ms = programming_start_ms;

	printf("programming - mode started for: %d secconds.\r\n", programming_timeout_ms);
	while ((HAL_GetTick() - programming_start_ms) < programming_timeout_ms)
	{
		radio_msg msg;
		if (radio_read_msg(&msg))
		{
			printf("New radio msg received, MSG_TYPE:%d\r\n", msg.msg_type);
			if (msg.msg_type == MSG_CODE_PROGRAM_REQ)
			{
				if (eeprom_store_new_radio_code(msg.radio_cfg.code, sizeof(msg.radio_cfg.code), msg.radio_cfg.id, false))
				{
					printf("programming - New radio_code succesfully stored! id:%d\r\n", msg.radio_cfg.id);
					msg.msg_type = MSG_CODE_PROGRAM_RES;
					radio_send_msg(&msg);
				}
				break;
			}
		}

		if ((HAL_GetTick() - last_time_ms) > 500)
		{
			last_time_ms = HAL_GetTick();
			led_toogle(LED_RED, 0);
		}
	}
}

//------------------------------------------------------------------------------
static void app_normal_mode()
{
	// Go to normal mode
	printf("normal - mode started!\r\n");
	led_enable(LED_GREEN);
	while (true)
	{
		if (radio_read_msg(&msg))
		{
			printf("New radio msg received, MSG_TYPE:%d\r\n", msg.msg_type);
			if (msg.msg_type == MSG_CODE_REQ)
			{
				// Check if radio_code exists in the eeprom
				const int radio_code_id = eeprom_check_if_radio_code_exists(msg.radio_cfg.code);
				if (radio_code_id != -1)
				{
					printf("eeprom_check_if_radio_code_exists - true, id:%d\r\n", radio_code_id);
					// Toogle relay
					relay_toogle(k_relay_toogle_time_ms);
					// Send a successful response
					printf("Sending a MSG_CODE_RES to keypad\r\n");
					msg.msg_type = MSG_CODE_RES;
					radio_send_msg(&msg);
				}
				else
				{
					printf("eeprom_check_if_radio_code_exists - false\r\n");
				}

			}
		}
	}
}


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

#endif /* APP_C_ */
