#include "keypad3x5.h"
#include "main.h"
#include <stdio.h>

//------------------------------------------------------------------------------
#define KEYPAD_TIMER_ONE_COUNT_PERIOD 	10  // ms
#define TIME_TO_NEXT_BUTTON_PRESS     	50 // ms
#define TIME_TO_NEXT_BUTTON_PRESS_VAL	(TIME_TO_NEXT_BUTTON_PRESS/KEYPAD_TIMER_ONE_COUNT_PERIOD)

// Types
//------------------------------------------------------------------------------
typedef struct
{
	uint8_t abcd_input;
	uint8_t efgh_input;
} KeypadInputsPressed;

// Private variables
//------------------------------------------------------------------------------
static volatile uint32_t keypad_timer_counter = 0;
static volatile bool keypad_button_pressed = false;
static volatile bool keypad_button_read = false;
static volatile KeypadInputsPressed keypad_last_inputs_state;

// Private functions
//------------------------------------------------------------------------------
static char* decode_keyboard_button(KeypadButtonPressed button)
{
	switch (button)
	{
		case BUTTON_0:
			return "BUTTON_0";

		case BUTTON_1:
			return "BUTTON_1";

		case BUTTON_2:
			return "BUTTON_2";

		case BUTTON_3:
			return "BUTTON_3";

		case BUTTON_4:
			return "BUTTON_4";

		case BUTTON_5:
			return "BUTTON_5";

		case BUTTON_6:
			return "BUTTON_6";

		case BUTTON_7:
			return "BUTTON_7";

		case BUTTON_8:
			return "BUTTON_8";

		case BUTTON_9:
			return "BUTTON_9";

		case BUTTON_P:
			return "BUTTON_P";

		case BUTTON_M:
			return "BUTTON_M";

		case BUTTON_ESC:
			return "BUTTON_ESC";

		case BUTTON_ARROW_UP:
			return "BUTTON_ARROW_UP";

		case BUTTON_ARROW_DOWN:
			return "BUTTON_ARROW_DOWN";

		default:
			return "INVALID_BUTTON";
	}
}

// Keypad timer
//------------------------------------------------------------------------------
static void reset_keypad_timer(void)
{
	keypad_timer_counter = 0;
}

//------------------------------------------------------------------------------
/**
 * @brief Sets A,B,C or D pins as pulled-up output, and EFGH to input state.
 */
static void set_efgh_pins_to_read_mode(KeypadButtonInputs active_abcd_pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* Disable EXTI interrupts*/
	HAL_NVIC_DisableIRQ(EXTI0_1_IRQn);
	HAL_NVIC_DisableIRQ(EXTI2_3_IRQn);

	/* Configure GPIO EFGH pin Output Level to LOW (reset) state */
	HAL_GPIO_WritePin(GPIOA, KEY_E_Pin|KEY_F_Pin|KEY_G_Pin|KEY_H_Pin, GPIO_PIN_RESET);

	/* Configure GPIO pins : KEY_A_Pin KEY_B_Pin KEY_C_Pin KEY_D_Pin as OUTPUTS*/
	GPIO_InitStruct.Pin = active_abcd_pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	/* Configure GPIO A,B,C or D active pin Output Level to HIGH state */
	HAL_GPIO_WritePin(GPIOA, active_abcd_pin, GPIO_PIN_SET);

	/* Configure GPIO pins : KEY_E_Pin KEY_F_Pin KEY_G_Pin KEY_H_Pin as INPUTS*/
	GPIO_InitStruct.Pin = KEY_E_Pin|KEY_F_Pin|KEY_G_Pin|KEY_H_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}


/**
 * @brief Sets ABCD pins as External input source - (EXTI) FALLING EDGE, and EFGH as OUTPUTS.
 */
static void set_pins_release_mode(KeypadButtonInputs active_abcd_pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* Configure GPIO pins : KEY_E_Pin KEY_F_Pin KEY_G_Pin KEY_H_Pin as OUTPUTS*/
	GPIO_InitStruct.Pin = KEY_E_Pin|KEY_F_Pin|KEY_G_Pin|KEY_H_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	/* Configure GPIO EFGH pin Output Level to HIGH state */
	HAL_GPIO_WritePin(GPIOA, KEY_E_Pin|KEY_F_Pin|KEY_G_Pin|KEY_H_Pin, GPIO_PIN_SET);

	/* Configure GPIO pins : active_abcd_pin as EXTI INPUTS*/
	GPIO_InitStruct.Pin = active_abcd_pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI0_1_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);

	HAL_NVIC_SetPriority(EXTI2_3_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);
}


/**
 * @brief Sets ABCD pins as External input source (EXTI) RISING EDGE, and EFGH as OUTPUTS.
 */
static void set_pins_default_mode(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* Configure GPIO pins : KEY_E_Pin KEY_F_Pin KEY_G_Pin KEY_H_Pin as OUTPUTS*/
	GPIO_InitStruct.Pin = KEY_E_Pin|KEY_F_Pin|KEY_G_Pin|KEY_H_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	/* Configure GPIO EFGH pin Output Level to HIGH state */
	HAL_GPIO_WritePin(GPIOA, KEY_E_Pin|KEY_F_Pin|KEY_G_Pin|KEY_H_Pin, GPIO_PIN_SET);

	/* Configure GPIO ABCD pin input Level to LOW (reset) state */
	HAL_GPIO_WritePin(GPIOA, KEY_A_Pin|KEY_B_Pin|KEY_C_Pin|KEY_D_Pin, GPIO_PIN_RESET);
	/* Configure GPIO pins : KEY_A_Pin KEY_B_Pin KEY_C_Pin KEY_D_Pin as EXTI INPUTS*/
	GPIO_InitStruct.Pin = KEY_A_Pin|KEY_B_Pin|KEY_C_Pin|KEY_D_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI0_1_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);

	HAL_NVIC_SetPriority(EXTI2_3_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);
}


/**
 * @brief Reads ABCD input state after button pressed
 */
static KeypadButtonInputs read_input_state_from_ABCD(void)
{

	if (HAL_GPIO_ReadPin(KEY_A_GPIO_Port, KEY_A_Pin) == GPIO_PIN_SET)
	{
		return BUTTON_INPUT_A;
	}
	else if (HAL_GPIO_ReadPin(KEY_B_GPIO_Port, KEY_B_Pin) == GPIO_PIN_SET)
	{
		return BUTTON_INPUT_B;
	}
	else if (HAL_GPIO_ReadPin(KEY_C_GPIO_Port, KEY_C_Pin) == GPIO_PIN_SET)
	{
		return BUTTON_INPUT_C;
	}
	else if (HAL_GPIO_ReadPin(KEY_D_GPIO_Port, KEY_D_Pin) == GPIO_PIN_SET)
	{
		return BUTTON_INPUT_D;
	}
	else
	{
		return BUTTON_UNKNOWN_ERROR;
	}
}

/**
 * @brief Reads EFGH input state after button pressed
 */
static KeypadButtonInputs read_input_state_from_EFGH(void)
{

	if (HAL_GPIO_ReadPin(KEY_E_GPIO_Port, KEY_E_Pin) == GPIO_PIN_SET)
	{
		return BUTTON_INPUT_E;
	}
	else if (HAL_GPIO_ReadPin(KEY_F_GPIO_Port, KEY_F_Pin) == GPIO_PIN_SET)
	{
		return BUTTON_INPUT_F;
	}
	else if (HAL_GPIO_ReadPin(KEY_G_GPIO_Port, KEY_G_Pin) == GPIO_PIN_SET)
	{
		return BUTTON_INPUT_G;
	}
	else if (HAL_GPIO_ReadPin(KEY_H_GPIO_Port, KEY_H_Pin) == GPIO_PIN_SET)
	{
		return BUTTON_INPUT_H;
	}
	else
	{
		return BUTTON_UNKNOWN_ERROR;
	}
}


/**
 * @brief Re-implemented EXTI Callback from stm32f0xx_hal_gpio.c
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	KeypadInputsPressed current_inputs_pressed = { BUTTON_UNKNOWN_ERROR, BUTTON_UNKNOWN_ERROR };

	if (!keypad_button_pressed)
	{
		keypad_last_inputs_state.abcd_input = read_input_state_from_ABCD();
		//printf("keypad_last_inputs_state.abcd_input == %d,\r\n", keypad_last_inputs_state.abcd_input);
		keypad_button_pressed = true;
		// Reset timer for next pin readout
		reset_keypad_timer();
	}
	else // if (keypad_button_pressed)
	{
		set_pins_default_mode();
		keypad_button_pressed = false;
	}
}

//------------------------------------------------------------------------------
/**
 * @brief Re-implemented EXTI Callback from stm32f0xx_hal_tim.c
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (keypad_button_pressed && keypad_timer_counter++ == TIME_TO_NEXT_BUTTON_PRESS_VAL)
	{
		KeypadInputsPressed current_inputs_pressed = { BUTTON_UNKNOWN_ERROR, BUTTON_UNKNOWN_ERROR };
		current_inputs_pressed.abcd_input = read_input_state_from_ABCD();
		//printf("current_inputs_pressed.abcd_input == %d,\r\n", current_inputs_pressed.abcd_input);
		if (current_inputs_pressed.abcd_input != BUTTON_UNKNOWN_ERROR && current_inputs_pressed.abcd_input == keypad_last_inputs_state.abcd_input)
		{
			// Set EFGH pins as inputs
			set_efgh_pins_to_read_mode(current_inputs_pressed.abcd_input);
			current_inputs_pressed.efgh_input = read_input_state_from_EFGH();
			//printf("current_inputs_pressed.efgh_input == %d,\r\n", current_inputs_pressed.efgh_input);

			if (current_inputs_pressed.efgh_input != BUTTON_UNKNOWN_ERROR)
			{
				memcpy(&keypad_last_inputs_state, &current_inputs_pressed, sizeof(keypad_last_inputs_state));
				keypad_button_read = true;
			}
		}
		// Swithc active_abcd input into FALLING_EDGE EXTI
		set_pins_release_mode(current_inputs_pressed.abcd_input);
	}
}

//------------------------------------------------------------------------------
void keypad3x5_init(void)
{
	set_pins_default_mode();
}

//------------------------------------------------------------------------------
KeypadButtonPressed keypad_get_last_state_if_changed()
{
	KeypadButtonPressed button_pressed = BUTTON_NONE;
	if (keypad_button_read)
	{
		button_pressed = (KeypadButtonPressed)(keypad_last_inputs_state.abcd_input|keypad_last_inputs_state.efgh_input);
		printf("ABCD_INPUT:0x%x, EFGH_INPUT:0x%x, SUM:%d,0x%x, %s\r\n",
				keypad_last_inputs_state.abcd_input, keypad_last_inputs_state.efgh_input,
				button_pressed, button_pressed, decode_keyboard_button(button_pressed));

		keypad_button_read = false;
	}
	return button_pressed;
}

//------------------------------------------------------------------------------
