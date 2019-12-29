
/**
 *  @brief:  Implementation of a NRF24L01 platform dependent [STM32CUBE] radio functions
 *  @author: luk6xff
 *  @email:  lukasz.uszko@gmail.com
 *  @date:   2019-12-02
 */


#ifndef __NRF24L01_CUBE_H__
#define __NRF24L01_CUBE_H__

#include "stm32f0xx_hal.h"
#include "../../nrf24l01.h"


void nrf24l01_cube_init(SPI_HandleTypeDef* spi, UART_HandleTypeDef* uart,
						GPIO_TypeDef* GPIO_CE_Port, uint16_t GPIO_CE_Pin,
						GPIO_TypeDef* GPIO_CSN_Port, uint16_t GPIO_CSN_Pin);

void nrf24l01_cube_deinit();

#endif // __NRF24L01_CUBE_H__
