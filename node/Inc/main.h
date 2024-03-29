/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define prescalerValue 7999
#define RELAY_Pin GPIO_PIN_7
#define RELAY_GPIO_Port GPIOA
#define NRF24L01_CSN_Pin GPIO_PIN_12
#define NRF24L01_CSN_GPIO_Port GPIOB
#define NRF24L01_SCK_Pin GPIO_PIN_13
#define NRF24L01_SCK_GPIO_Port GPIOB
#define NRF24L01_MISO_Pin GPIO_PIN_14
#define NRF24L01_MISO_GPIO_Port GPIOB
#define NRF24L01_MOSI_Pin GPIO_PIN_15
#define NRF24L01_MOSI_GPIO_Port GPIOB
#define NRF24L01_CE_Pin GPIO_PIN_6
#define NRF24L01_CE_GPIO_Port GPIOC
#define NRF24L01_IRQ_Pin GPIO_PIN_7
#define NRF24L01_IRQ_GPIO_Port GPIOC
#define TR1_Pin GPIO_PIN_8
#define TR1_GPIO_Port GPIOC
#define TR2_Pin GPIO_PIN_9
#define TR2_GPIO_Port GPIOC
#define LED_RED_Pin GPIO_PIN_10
#define LED_RED_GPIO_Port GPIOA
#define AT24Cx_WP_Pin GPIO_PIN_12
#define AT24Cx_WP_GPIO_Port GPIOA
#define AT24Cx_SCL_Pin GPIO_PIN_6
#define AT24Cx_SCL_GPIO_Port GPIOF
#define AT24Cx_SDA_Pin GPIO_PIN_7
#define AT24Cx_SDA_GPIO_Port GPIOF
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
