#include "keypad3x5.h"
#include "main.h"

#define TIME_TO_NEXT_BUTTON_PRESS    100//ms
#define TIME_TO_CANCEL_BUTTON_PRESS_SEQUENCE    5000//ms
#define BINARY_SEMAPHORE_LENGTH	1

volatile uint16_t tickCounterForPressingButtonEvent = 0;

typedef struct
{
	uint8_t abcd_input;
	uint8_t efgh_input;
} KeypadInputsPressed;

uint8_t codeToSend[4];
// private methods
//leds

typedef enum
{
	GREEN_LED,
	RED_LED
} KeypadLeds;


/**
 * @brief Sets ABCD pins as pulled-up output, and EFGH to input state.
 */
static void set_pins_to_read_mode(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* Disable EXTI interrupts*/
	HAL_NVIC_DisableIRQ(EXTI0_1_IRQn);
	HAL_NVIC_DisableIRQ(EXTI2_3_IRQn);

	/*Configure GPIO pins : KEY_A_Pin KEY_B_Pin KEY_C_Pin KEY_D_Pin as OUTPUTS*/
	GPIO_InitStruct.Pin = KEY_A_Pin|KEY_B_Pin|KEY_C_Pin|KEY_D_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : KEY_E_Pin KEY_F_Pin KEY_G_Pin KEY_H_Pin as INPUTS*/
	GPIO_InitStruct.Pin = KEY_E_Pin|KEY_F_Pin|KEY_G_Pin|KEY_H_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* ABCD to HIGH state */
	HAL_GPIO_WritePin(GPIOA, KEY_A_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, KEY_B_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, KEY_C_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, KEY_D_Pin, GPIO_PIN_SET);
}



/**
 * @brief Sets ABCD pins as External input source (EXTI), and EFGH as OUTPUTS.
 */
static void set_pins_default_mode(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/*Configure GPIO pins : KEY_A_Pin KEY_B_Pin KEY_C_Pin KEY_D_Pin as EXTI INPUTS*/
	GPIO_InitStruct.Pin = KEY_A_Pin|KEY_B_Pin|KEY_C_Pin|KEY_D_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : KEY_E_Pin KEY_F_Pin KEY_G_Pin KEY_H_Pin as OUTPUTS*/
	GPIO_InitStruct.Pin = KEY_E_Pin|KEY_F_Pin|KEY_G_Pin|KEY_H_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* EFGH to HIGH state */
	HAL_GPIO_WritePin(GPIOA, KEY_E_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, KEY_F_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, KEY_G_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, KEY_H_Pin, GPIO_PIN_SET);

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
	//disableInterruptsForInputButtons();
	//disableTimer3();
	//enableTimer3(); //after first number was received , fire up the counter that counts to
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
		if (current_inputs_pressed.efgh_input != BUTTON_UNKNOWN_ERROR)
		{
			printf("ABCD_INPUT:0x%x, EFGH_INPUT:0x%x, BUTTON:%d,0x%x\r\n",
					current_inputs_pressed.abcd_input, current_inputs_pressed.efgh_input,
					(KeypadButtonPressed)(current_inputs_pressed.abcd_input|current_inputs_pressed.efgh_input),
					(KeypadButtonPressed)(current_inputs_pressed.abcd_input|current_inputs_pressed.efgh_input));
		}
	}

	// Reset pins to default state
	set_pins_default_mode();
}

/**
 * @brief Re-implemented EXTI Callback from stm32f0xx_hal_tim.c
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	printf("HAL_TIM_PeriodElapsedCallback: %d,\r\n", HAL_GetTick());
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

//timers
//static void enableTimer3(void)
//{
//	TIM3->CR1 |= TIM_CR1_CEN; //enable counter
//	TIM3->CNT = 0;
//	tickCounterForPressingButtonEvent = 0;
//	NVIC_SetPriority(TIM3_IRQn, 2);
//	NVIC_EnableIRQ(TIM3_IRQn);
//}

//static void disableTimer3(void) {
//	TIM3->CR1 &= ~(TIM_CR1_CEN);
//	NVIC_DisableIRQ(TIM3_IRQn);
//}
//
//static void Timer3Configuration(void) {
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
//	TIM3->ARR = 100; //value that counter counts up to :1ms
//	TIM3->BDTR = 0;
//	TIM3->PSC = 9; // 1ms  //The counter clock frequency (CK_CNT) is equal to fCK_PSC / (PSC[15:0] + 1) // here fCK_PSC =1MHz  -> 100000 Hz
//	TIM3->CR1 = TIM_CR1_ARPE; // ARPE bufferd , upcounter
//	TIM3->DIER = TIM_DIER_UIE; //
//
//}

//static void Timer3Init(void) {
//	xTimer3Semphr = xSemaphoreCreateBinary();
//	configASSERT(xTimer3Semphr);
//	Timer3Configuration();
//}


//
//static void KeyboardInit(void) {
//
//	xKeyboardQueueSet = xQueueCreateSet(
//			sizeof(InputsPressed) + BINARY_SEMAPHORE_LENGTH);
//	configASSERT(xKeyboardQueueSet);
//	xKeyboardReceivedQueue = xQueueCreate(1,sizeof(InputsPressed));
//	configASSERT(xKeyboardReceivedQueue);
//	xReceivedCorrectDataCodeFromKeyboard = xQueueCreate(1,sizeof(codeToSend));
//	configASSERT( xReceivedCorrectDataCodeFromKeyboard);
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2ENR_SYSCFGEN, ENABLE); // Enable SYSCFG clock
//	keyboardLedsConfig();
//	setABCDToInputStateDefaultState();
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,
//			EXTI_PinSource0 | EXTI_PinSource1 | EXTI_PinSource2
//					| EXTI_PinSource3);
//	EXTI->IMR |= EXTI_IMR_MR0 | EXTI_IMR_MR1 | EXTI_IMR_MR2 | EXTI_IMR_MR3; //Interrupt request from these pins is not masked
//	EXTI->RTSR |= EXTI_RTSR_TR0 | EXTI_RTSR_TR1 | EXTI_RTSR_TR2 | EXTI_RTSR_TR3; // Rising trigger edge
//	Timer3Init();
//	xQueueAddToSet(xKeyboardReceivedQueue, xKeyboardQueueSet);
//	xQueueAddToSet(xTimer3Semphr, xKeyboardQueueSet);
//
//	enableInterruptsForInputButtons();
//
//}
//

//
//void TIM3_IRQHandler(void) {
//	if (TIM3->SR & TIM_SR_UIF) {
//		TIM3->SR &= ~(TIM_SR_UIF);
//		tickCounterForPressingButtonEvent++;
//		if (tickCounterForPressingButtonEvent == TIME_TO_NEXT_BUTTON_PRESS) {
//			//DEBUG("=");
//			//DEBUG
//			enableInterruptsForInputButtons();
//		} else if (tickCounterForPressingButtonEvent
//				== TIME_TO_CANCEL_BUTTON_PRESS_SEQUENCE) {
//			xSemaphoreGiveFromISR(xTimer3Semphr, pdFALSE);
//			disableTimer3();
//		}
//	}
//}
//
//static void blinkGivenLed(eKeyboardLeds ledColour, uint8_t nrOfBlinks,
//		uint16_t delayBetweenBlinks_ms) {
//
//	void (*ledOnCallback)(void);
//	void (*ledOffCallback)(void);
//	switch (ledColour) {
//	case GREEN_LED:
//		ledOnCallback = greenLedOn;
//		ledOffCallback = greenLedOff;
//		break;
//	case RED_LED:
//		ledOnCallback = redLedOn;
//		ledOffCallback = redLedOff;
//		break;
//	default:
//		return;
//	}
//	for (uint8_t i = 0; i < nrOfBlinks; i++) {
//		(*ledOnCallback)();
//		vTaskDelay(delayBetweenBlinks_ms / portTICK_RATE_MS);
//		(*ledOffCallback)();
//		vTaskDelay(delayBetweenBlinks_ms / portTICK_RATE_MS);
//	}
//}
//
//static void vTaskKeyboard(void* pvParameters) {
//
//	KeyboardInit();
//	InputsPressed receivedInputState;
//	const uint8_t nrOfDatatowaitFor = 4;
//	uint8_t receivedDataBuffer[nrOfDatatowaitFor];
//	static uint8_t numberOfReceivedFrames;
//	portTickType xLastFlashTime;
//	xLastFlashTime = xTaskGetTickCount();
//	for (;;) {
//		xKeyboardQueueSetMemberHandle = xQueueSelectFromSet(xKeyboardQueueSet,
//				10 / portTICK_RATE_MS);
//
//		if (xKeyboardQueueSetMemberHandle == xTimer3Semphr) {
//			//	DEBUG("*");
//			if (xTimer3Semphr != NULL) {
//				if (xSemaphoreTake(xTimer3Semphr,(portTickType)0)) {
//					numberOfReceivedFrames = 0;
//					blinkGivenLed(GREEN_LED, 3, 100);
//					memset(receivedDataBuffer, 0, sizeof(receivedDataBuffer));
//				}
//			}
//		} else if (xKeyboardQueueSetMemberHandle == xKeyboardReceivedQueue) {
//			xQueueReceive(xKeyboardReceivedQueue, &receivedInputState, 0);
//			//GPIOC->ODR ^= (GPIO_ODR_9); //DEBUG
//			receivedDataBuffer[numberOfReceivedFrames] =
//					(receivedInputState.abcd_input
//							+ receivedInputState.efgh_input);
//			greenLedOff();
//			numberOfReceivedFrames++;
//			if (numberOfReceivedFrames == nrOfDatatowaitFor) {
//				numberOfReceivedFrames = 0;
//				//here checking if in eeprom such value exists
//				blinkGivenLed(RED_LED, 3, 300);
//				//memcpy(codeToSend, receivedDataBuffer,
//				//		sizeof(nrOfDatatowaitFor));
//				//codeToSend[nrOfDatatowaitFor] = '\0';
//				//	if(strcmp(codeToSend,))
//			}
//		} else { /*do nothing just wait 10ms  for any data*/
//		}
//
//	}
//}
//
//void vStartKeyboardTask(unsigned portBASE_TYPE uxPriority) {
//	xTaskCreate(vTaskKeyboard, (signed const char*)"KEY", 160, NULL, uxPriority,
//			NULL);
//
//switch (receivedDataBuffer[numberOfReceivedFrames]) { //FOR DEBUG
// case BUTTON_0:
// //DEBUG("0");
// break;
// case BUTTON_1:
// //DEBUG("1");
// break;
// case BUTTON_2:
// //DEBUG("2");
// break;
// case BUTTON_3:
// //DEBUG("3");
// break;
// case BUTTON_4:
// //DEBUG("4");
// break;
// case BUTTON_5:
// //DEBUG("5");
// break;
// case BUTTON_6:
// //DEBUG("6");
// break;
// case BUTTON_7:
// //DEBUG("7");
// break;
// case BUTTON_8:
// //DEBUG("8");
// break;
// case BUTTON_9:
// //DEBUG("9");
// break;
// case BUTTON_P:
// //DEBUG("P");
// break;
// case BUTTON_M:
// //DEBUG("M");
// break;
// case BUTTON_ESC:
// //DEBUG("ES");
// break;
// case BUTTON_ARROW_UP:
// //DEBUG("AU");
// break;
// case BUTTON_ARROW_DOWN:
// //DEBUG("AD");
// break;
// default:
// DEBUG("GOW");
// break;
//
// }
//}
