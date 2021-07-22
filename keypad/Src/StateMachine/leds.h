/*
 * leds.h
 *
 *  Created on: Jul 22, 2021
 *      Author: luk6xff
 */

#ifndef STATEMACHINE_LEDS_H_
#define STATEMACHINE_LEDS_H_

#include <stdint.h>

typedef enum
{
	LED_GREEN,
	LED_RED
} Leds;

void led_enable(Leds led);

void led_disable(Leds led);

void led_toogle(Leds led, uint32_t toogle_time_ms);

#endif /* STATEMACHINE_LEDS_H_ */
