#ifndef __KEYPAD3x5_H
#define __KEYPAD3x5_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef enum
{
	BUTTON_INPUT_A 			= 1<<0,
	BUTTON_INPUT_B 			= 1<<1,
	BUTTON_INPUT_C 			= 1<<2,
	BUTTON_INPUT_D 			= 1<<3,
	BUTTON_INPUT_E 			= 1<<4,
	BUTTON_INPUT_F 			= 1<<5,
	BUTTON_INPUT_G 			= 1<<6,
	BUTTON_INPUT_H 			= 1<<7,
    BUTTON_UNKNOWN_ERROR 	= 0xFF
} KeypadButtonInputs;

typedef enum
{
	BUTTON_0 = 			(BUTTON_INPUT_C | BUTTON_INPUT_E),
	BUTTON_1 = 			(BUTTON_INPUT_A | BUTTON_INPUT_E),
	BUTTON_2 = 			(BUTTON_INPUT_A | BUTTON_INPUT_G),
	BUTTON_3 = 			(BUTTON_INPUT_A | BUTTON_INPUT_F),
	BUTTON_4 = 			(BUTTON_INPUT_B | BUTTON_INPUT_G),
	BUTTON_5 = 			(BUTTON_INPUT_B | BUTTON_INPUT_F),
	BUTTON_6 = 			(BUTTON_INPUT_A | BUTTON_INPUT_H),
	BUTTON_7 = 			(BUTTON_INPUT_C | BUTTON_INPUT_F),
	BUTTON_8 = 			(BUTTON_INPUT_B | BUTTON_INPUT_H),
	BUTTON_9 = 			(BUTTON_INPUT_B | BUTTON_INPUT_E),
	BUTTON_P = 			(BUTTON_INPUT_D | BUTTON_INPUT_E),
	BUTTON_M = 			(BUTTON_INPUT_D | BUTTON_INPUT_G),
	BUTTON_ESC        = (BUTTON_INPUT_D | BUTTON_INPUT_F), //NOT WORK
	BUTTON_ARROW_UP   = (BUTTON_INPUT_C | BUTTON_INPUT_H),
	BUTTON_ARROW_DOWN = (BUTTON_INPUT_C | BUTTON_INPUT_G) //NOT WORK
} KeypadButtonPressed;

/**
 * @brief Keypad init function
 */
void keypad3x5_init(void);

#ifdef __cplusplus
}
#endif

#endif /*__KEYPAD3x5_H */
