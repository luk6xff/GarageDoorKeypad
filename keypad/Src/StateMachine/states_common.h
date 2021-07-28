/*
 * states_common.h
 *
 *  Created on: Jul 28, 2021
 *      Author: luk6xff
 */

#ifndef STATEMACHINE_STATES_COMMON_H_
#define STATEMACHINE_STATES_COMMON_H_

#include <stdint.h>
#include <stdbool.h>

bool verify_radio_code(const uint8_t *radio_code, const uint8_t radio_code_size);


#endif /* STATEMACHINE_STATES_COMMON_H_ */
