#ifndef __KEYBOARD4X4_H
#define __KEYBOARD4X4_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <string.h>


typedef enum
{
	BUTTON_INPUT_A=0,
	BUTTON_INPUT_B=2,
	BUTTON_INPUT_C=17,
	BUTTON_INPUT_D=29,
	BUTTON_INPUT_E=36,
	BUTTON_INPUT_F=45,
	BUTTON_INPUT_G=58,
	BUTTON_INPUT_H=69,
    BUTTON_UNKNOWN_ERROR=99
} EButtonInputs;


typedef enum
{
	BUTTON_0=(BUTTON_INPUT_C+BUTTON_INPUT_E),
	BUTTON_1=(BUTTON_INPUT_A+BUTTON_INPUT_E),
	BUTTON_2=(BUTTON_INPUT_A+BUTTON_INPUT_G),
	BUTTON_3=(BUTTON_INPUT_A+BUTTON_INPUT_F),
	BUTTON_4=(BUTTON_INPUT_B+BUTTON_INPUT_G),
	BUTTON_5=(BUTTON_INPUT_B+BUTTON_INPUT_F),
	BUTTON_6=(BUTTON_INPUT_A+BUTTON_INPUT_H),
	BUTTON_7=(BUTTON_INPUT_C+BUTTON_INPUT_F),
	BUTTON_8=(BUTTON_INPUT_B+BUTTON_INPUT_H),
	BUTTON_9=(BUTTON_INPUT_B+BUTTON_INPUT_E),
	BUTTON_P=(BUTTON_INPUT_D+BUTTON_INPUT_E),
	BUTTON_M=(BUTTON_INPUT_D+BUTTON_INPUT_G),
	BUTTON_ESC=(BUTTON_INPUT_D+BUTTON_INPUT_F),
	BUTTON_ARROW_UP=(BUTTON_INPUT_C+BUTTON_INPUT_H),
	BUTTON_ARROW_DOWN=(BUTTON_INPUT_C+BUTTON_INPUT_G)
} EButtonPressed;

#ifdef __cplusplus
}
#endif

#endif /*__KEYBOARD4X4_H */
