/*
 * relay.h
 *
 *  Created on: Jul 28, 2021
 *      Author: luk6xff
 */

#ifndef RELAY_H_
#define RELAY_H_

#include <stdint.h>

void relay_enable();

void relay_disable();

void relay_toogle(uint32_t toogle_time_ms);

#endif /* RELAY_H_ */
