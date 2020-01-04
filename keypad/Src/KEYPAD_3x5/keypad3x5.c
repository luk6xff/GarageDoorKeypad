#include "keypad3x5.h"
#include "main.h"


//------------------------------------------------------------------------------
#define KEYPAD_TIMER_ONE_COUNT_PERIOD 	10  // ms
#define TIME_TO_NEXT_BUTTON_PRESS     	100 // ms
#define TIME_TO_NEXT_BUTTON_PRESS_VAL	(TIME_TO_NEXT_BUTTON_PRESS/KEYPAD_TIMER_ONE_COUNT_PERIOD)

// Types
//------------------------------------------------------------------------------
typedef struct
{
	uint8_t abcd_input;
	uint8_t efgh_input;
} KeypadInputsPressed;

// Leds
typedef enum
{
	GREEN_LED,
	RED_LED
} KeypadLeds;

// Private variables
//------------------------------------------------------------------------------
static volatile uint32_t keypad_timer_counter = 0;

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
static void enable_keypad_timer(void)
{
	TIM_HandleTypeDef htim3;
	htim3.Instance = TIM3;
	HAL_TIM_Base_Start_IT(&htim3);
	keypad_timer_counter = 0;
}

//------------------------------------------------------------------------------
static void disable_keypad_timer(void)
{
	TIM_HandleTypeDef htim3;
	htim3.Instance = TIM3;
	HAL_TIM_Base_Stop_IT(&htim3);
}

//------------------------------------------------------------------------------
/**
 * @brief Sets ABCD pins as pulled-up output, and EFGH to input state.
 */
static void set_pins_to_read_mode(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* Disable EXTI interrupts*/
	HAL_NVIC_DisableIRQ(EXTI0_1_IRQn);
	HAL_NVIC_DisableIRQ(EXTI2_3_IRQn);

	/* Configure GPIO EFGH pin Output Level to LOW (reset) state */
	HAL_GPIO_WritePin(GPIOA, KEY_E_Pin|KEY_F_Pin|KEY_G_Pin|KEY_H_Pin, GPIO_PIN_RESET);

	/* Configure GPIO pins : KEY_A_Pin KEY_B_Pin KEY_C_Pin KEY_D_Pin as OUTPUTS*/
	GPIO_InitStruct.Pin = KEY_A_Pin|KEY_B_Pin|KEY_C_Pin|KEY_D_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* Configure GPIO pins : KEY_E_Pin KEY_F_Pin KEY_G_Pin KEY_H_Pin as INPUTS*/
	GPIO_InitStruct.Pin = KEY_E_Pin|KEY_F_Pin|KEY_G_Pin|KEY_H_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}



/**
 * @brief Sets ABCD pins as External input source (EXTI), and EFGH as OUTPUTS.
 */
static void set_pins_default_mode(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* Configure GPIO EFGH pin Output Level to HIGH state */
	HAL_GPIO_WritePin(GPIOA, KEY_E_Pin|KEY_F_Pin|KEY_G_Pin|KEY_H_Pin, GPIO_PIN_SET);

	/* Configure GPIO pins : KEY_E_Pin KEY_F_Pin KEY_G_Pin KEY_H_Pin as OUTPUTS*/
	GPIO_InitStruct.Pin = KEY_E_Pin|KEY_F_Pin|KEY_G_Pin|KEY_H_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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
	disable_keypad_timer();

	KeypadInputsPressed current_inputs_pressed = { BUTTON_UNKNOWN_ERROR, BUTTON_UNKNOWN_ERROR };
	set_pins_to_read_mode();
	if (GPIO_Pin == KEY_A_Pin)
	{
		current_inputs_pressed.abcd_input = BUTTON_INPUT_A;
	}
	else if (GPIO_Pin == KEY_B_Pin)
	{
		current_inputs_pressed.abcd_input = BUTTON_INPUT_B;
	}
	else if (GPIO_Pin == KEY_C_Pin)
	{
		current_inputs_pressed.abcd_input = BUTTON_INPUT_C;
	}
	else if (GPIO_Pin == KEY_D_Pin)
	{
		current_inputs_pressed.abcd_input = BUTTON_INPUT_D;
	}
	else
	{
		current_inputs_pressed.abcd_input = BUTTON_UNKNOWN_ERROR;
	}

	if (current_inputs_pressed.abcd_input != BUTTON_UNKNOWN_ERROR)
	{
		//greenLedOn();
		// Set HIGH state on a given pin of ABCD row
		HAL_GPIO_WritePin(GPIOA, GPIO_Pin, GPIO_PIN_SET);
		current_inputs_pressed.efgh_input = read_input_state_from_EFGH();
		// Set LOW state on a given pin of ABCD row
		HAL_GPIO_WritePin(GPIOA, GPIO_Pin, GPIO_PIN_RESET);
		if (current_inputs_pressed.efgh_input != BUTTON_UNKNOWN_ERROR)
		{
			printf("ABCD_INPUT:0x%x, EFGH_INPUT:0x%x, SUM:%d,0x%x, %s\r\n",
				   current_inputs_pressed.abcd_input, current_inputs_pressed.efgh_input,
				   (KeypadButtonPressed)(current_inputs_pressed.abcd_input|current_inputs_pressed.efgh_input),
				   (KeypadButtonPressed)(current_inputs_pressed.abcd_input|current_inputs_pressed.efgh_input),
				   decode_keyboard_button(current_inputs_pressed.abcd_input|current_inputs_pressed.efgh_input));
		}
	}
	// Enable timer for next pin readout
	enable_keypad_timer();
}

//------------------------------------------------------------------------------
/**
 * @brief Re-implemented EXTI Callback from stm32f0xx_hal_tim.c
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//printf("HAL_TIM_PeriodElapsedCallback: %d,\r\n", HAL_GetTick());
	keypad_timer_counter++;
	if (keypad_timer_counter == TIME_TO_NEXT_BUTTON_PRESS_VAL)
	{
		// Reset pins to default state, enable pin button interrupt
		set_pins_default_mode();
	}
}

//inline static void keyboardLedsConfig(void) {
//	GPIOA->MODER |= (GPIO_MODER_MODER10_0 | GPIO_MODER_MODER11_0); //RED PA10, GREEN PA11  OUTPUTs
//	GPIOA->PUPDR |= (GPIO_PUPDR_PUPDR10_0 | GPIO_PUPDR_PUPDR11_0); //PULL UP
//}
//
//inline static void redLedOn(void) {
//	GPIOA->BSRR = GPIO_BSRR_BS_10;
//}
//inline static void redLedOff(void) {
//	GPIOA->BSRR = GPIO_BSRR_BR_10;
//}
//inline static void greenLedOn(void) {
//	GPIOA->BSRR = GPIO_BSRR_BS_11;
//}
//inline static void greenLedOff(void) {
//	GPIOA->BSRR = GPIO_BSRR_BR_11;
//}

//------------------------------------------------------------------------------
void keypad3x5_init(void)
{
	set_pins_default_mode();
	// Enable timer 3 interrupts
	//HAL_TIM_Base_Start_IT(&htim3);
}


