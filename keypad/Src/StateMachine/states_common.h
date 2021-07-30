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

// COMMON CONST
static const uint32_t k_wait_for_response_from_node_timeout_ms = 2000;
static const uint32_t k_led_toogle_time_ms = 100;

bool verify_radio_code(const uint8_t *radio_code, const uint8_t radio_code_size);


#endif /* STATEMACHINE_STATES_COMMON_H_ */
