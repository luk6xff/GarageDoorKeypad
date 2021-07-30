/*
 * leds.h
 *
 *  Created on: Jul 22, 2021
 *      Author: luk6xff
 */

#ifndef KEYPAD_3X5_LEDS_H_
#define KEYPAD_3X5_LEDS_H_

#include <stdint.h>

typedef enum
{
	LED_RED,
	LED_GREEN,
} Leds;

void led_enable(Leds led);

void led_disable(Leds led);

void led_disable_all();

void led_toogle(Leds led, uint32_t toogle_time_ms);

void led_toogle_loop(Leds led, uint32_t toogle_time_ms, uint32_t loops_num);

#endif /* KEYPAD_3X5_LEDS_H_ */
