/*
 * radio_msg.h
 *
 *  Created on: Jul 28, 2021
 *      Author: luk6xff
 */

#ifndef RADIO_RADIO_MSG_H_
#define RADIO_RADIO_MSG_H_

#include "../Storage/eeprom.h" // For radio_config

typedef enum
{
	MSG_CODE_REQ = 0,
	MSG_CODE_RES,
	MSG_CODE_PROGRAM_REQ,
	MSG_CODE_PROGRAM_RES,
} radio_msg_type;

typedef struct
{
	uint32_t msg_type; // radio_msg_type
	radio_config radio_code;
} radio_msg;

#endif /* RADIO_RADIO_MSG_H_ */
