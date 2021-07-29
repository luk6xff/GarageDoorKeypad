/*
 * app.c
 *
 *  Created on: Jul 22, 2021
 *      Author: luk6xff
 */

#ifndef APP_C_
#define APP_C_

#include "app.h"
#include "../Storage/eeprom.h"
#include "../Radio/radio.h"
#include <stdio.h>

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
			printf("New radio msg received, MSG_TYPE:%d", msg.msg_type);
		}
	}
}

//------------------------------------------------------------------------------

#endif /* APP_C_ */
