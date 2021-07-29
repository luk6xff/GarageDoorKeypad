/*
 * app.c
 *
 *  Created on: Jul 22, 2021
 *      Author: luk6xff
 */

#ifndef APP_C_
#define APP_C_

#include "app.h"
#include "Storage/eeprom.h"
#include "Radio/radio.h"
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
	while (true)
	{
		radio_msg msg;
		if (radio_read_msg(&msg))
		{
			printf("New radio msg received, MSG_TYPE:%d\r\n", msg.msg_type);
		}

		HAL_Delay(1000);
		msg.msg_type = MSG_CODE_RES;
		memcpy(&(msg.radio_code), &(eeprom_data_get_current()->radio_configs[0]), sizeof(msg.radio_code));
		printf("Sending a new radio msg: MSG_CODE_RES...\r\n");
		radio_send_msg(&msg);
	}
}

//------------------------------------------------------------------------------

#endif /* APP_C_ */
