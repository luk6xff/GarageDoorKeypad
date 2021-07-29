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
#include <stdio.h>
#include <string.h>

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
	radio_msg msg;
	const uint32_t programming_start_ms = HAL_GetTick();
	const uint32_t programming_timeout_ms = 30000; // 30 seconds after reset
	uint32_t last_time_ms = programming_start_ms;

	printf("programming - mode started for: %d secconds.\r\n", programming_timeout_ms);
	while ((HAL_GetTick() - programming_start_ms) < programming_timeout_ms)
	{
		if (radio_read_msg(&msg))
		{
			printf("New radio msg received, MSG_TYPE:%d\r\n", msg.msg_type);
			if (msg.msg_type == MSG_CODE_PROGRAM_REQ)
			{
				memcpy(&(eeprom_data_get_current()->radio_configs[msg.radio_cfg.id]), &(msg.radio_cfg), sizeof(msg.radio_cfg));
				if (eeprom_store_new_radio_code(msg.radio_cfg.code, sizeof(msg.radio_cfg.code), msg.radio_cfg.id))
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
				// LU_TODO Check if radio_code exists
				msg.msg_type = MSG_CODE_RES;
				printf("Sending a MSG_CODE_RES to keypad\r\n");
				radio_send_msg(&msg);
			}
		}
	}
}

//------------------------------------------------------------------------------

#endif /* APP_C_ */
