/*
 * states_common.c
 *
 *  Created on: Jul 28, 2021
 *      Author: luk6xff
 */

#include "states_common.h"
#include "Storage/eeprom.h"
#include "../KEYPAD_3x5/keypad3x5.h"
#include "main.h"
#include <stdio.h>


//------------------------------------------------------------------------------
extern WWDG_HandleTypeDef hwwdg;

//------------------------------------------------------------------------------
// Clean all radio codes hidden option. You have to press the following combination of buttons: [M ^ M ^]
static const uint8_t k_clear_radio_codes[RADIO_CODE_SIZE] = {BUTTON_M, BUTTON_ARROW_UP, BUTTON_M, BUTTON_ARROW_UP};

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
		printf("RADIO code provided: [0]:0x%x, [1]:0x%x, [2]:0x%x, [3]:0x%x\r\n", \
				radio_code[0], radio_code[1], radio_code[2], radio_code[3]);
	}
	else
	{
		ret = false;
	}
	return ret;
}

//------------------------------------------------------------------------------
void verify_and_clear_radio_codes(const uint8_t *radio_code, const uint8_t radio_code_size)
{
	if (radio_code && radio_code_size == RADIO_CODE_SIZE)
	{
		if (memcmp(radio_code, k_clear_radio_codes, RADIO_CODE_SIZE) == 0)
		{
			printf("Clear radio codes code applied!\r\n");
			printf("Clearing radio codes...\r\n");
			memset(eeprom_data_get_current()->radio_configs, 0xFF, sizeof(eeprom_data_get_current()->radio_configs));
			eeprom_data_store(eeprom_data_get_current());
		}
	}
}

//------------------------------------------------------------------------------
void kick_watchdog()
{
	/* Refresh watchdog, no more than 2[s] must expire before next refresh */
	HAL_WWDG_Refresh(&hwwdg);
}

//------------------------------------------------------------------------------

