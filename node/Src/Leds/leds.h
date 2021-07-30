/*
 * leds.h
 *
 *  Created on: Jul 22, 2021
 *      Author: luk6xff
 */

#ifndef LEDS_H_
#define LEDS_H_

#include <stdint.h>

void led_enable();

void led_disable();

void led_toogle(uint32_t toogle_time_ms);

#endif /* LEDS_H_ */
