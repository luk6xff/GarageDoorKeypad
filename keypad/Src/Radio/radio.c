/*
 * radio.c
 *
 *  Created on: Jul 22, 2021
 *      Author: luk6xff
 */


#include "radio.h"
#include "NRF24L01+/platform/stm32cube/nrf24l01-cube.h"
#include "main.h"
#include <stdio.h>

//------------------------------------------------------------------------------
const uint8_t nrf24l01_connected[] = "NRF24L01+ module connected properly\r\n";
const uint8_t nrf24l01_not_connected[] = "NRF24L01+ module NOT connected!!!\r\n";

extern SPI_HandleTypeDef hspi2;

extern UART_HandleTypeDef huart1;
//------------------------------------------------------------------------------
void radio_init()
{
	// NRF24LO1
	nrf24l01_cube_init(&hspi2, &huart1, NRF24L01_CE_GPIO_Port, NRF24L01_CE_Pin, NRF24L01_CSN_GPIO_Port, NRF24L01_CSN_Pin);
	nrf24l01_enable();
	if (nrf24l01_is_connected())
	{
	  printf("%s", nrf24l01_connected);
	}
	else
	{
	  printf("%s", nrf24l01_not_connected);
	}
	//nrf24l01_print_all_regs();
	nrf24l01_power_down();
}

//------------------------------------------------------------------------------
void radio_deinit()
{
	nrf24l01_power_down();
	nrf24l01_disable();
}

//------------------------------------------------------------------------------
bool radio_send_data(const uint16_t recv_dev_id, const uint8_t *data,
                    const uint32_t data_len)
{
    int ret = nrf24l01_write((uint8_t)recv_dev_id, data, data_len);
    if (ret > 0)
    {
  	  printf("nrf24l01 data sent succesfully!\r\n");
    }
    return ret;
}

//------------------------------------------------------------------------------
bool radio_read_data(const uint16_t recv_dev_id, uint8_t *data, const uint32_t data_len)
{
    bool msg_received = false;

    // Check for any message
    if (nrf24l01_read((uint8_t)recv_dev_id, data, data_len) > 0)
    {
    	printf("nrf24l01 data received succesfully!\r\n");
    }

	return msg_received;
}


//------------------------------------------------------------------------------
bool radio_send_msg(const radio_msg *msg)
{
	if (!msg)
	{
		return false;
	}
	return radio_send_data(0, (const uint8_t*)msg, sizeof(radio_msg));
}

//------------------------------------------------------------------------------
bool radio_read_msg(radio_msg *msg)
{
	if (!msg)
	{
		return false;
	}

	return radio_read_data(0, (uint8_t*)msg, sizeof(radio_msg));
}
