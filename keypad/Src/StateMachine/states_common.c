/*
 * states_common.c
 *
 *  Created on: Jul 28, 2021
 *      Author: luk6xff
 */

#include "states_common.h"
#include "../Storage/eeprom.h"
#include "../KEYPAD_3x5/keypad3x5.h"
#include <stdio.h>

//------------------------------------------------------------------------------
bool verify_radio_code(const uint8_t *radio_code, const uint8_t radio_code_size)
{
	bool ret = true;

	if (radio_code && radio_code_size == RADIO_CODE_SIZE)
	{
		for (size_t i = 0; i < RADIO_CODE_SIZE; ++i)
		{
			if (radio_code[i] == BUTTON_P || \
				radio_code[i] == BUTTON_M ||
				radio_code[i] == BUTTON_ESC	||
				radio_code[i] == BUTTON_ARROW_UP	||
				radio_code[i] == BUTTON_ARROW_DOWN	||
				radio_code[i] == BUTTON_NONE)
			{
				printf("RADIO code provided does not contain only digits [0-9]\r\n");
				ret = false;
			}
		}
		printf("RADIO code provided is valid: [0]:%d, [1]:%d, [2]:%d, [3]:%d\r\n", \
				radio_code[0], radio_code[1], radio_code[2], radio_code[3]);
	}
	else
	{
		ret = false;
	}
	return ret;
}
