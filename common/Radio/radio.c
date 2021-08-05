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
extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart1;

//------------------------------------------------------------------------------
const uint8_t k_nrf24l01_connected[] = "NRF24L01+ module connected properly\r\n";
const uint8_t k_nrf24l01_not_connected[] = "NRF24L01+ module NOT connected!!!\r\n";
// Radio pipe
const uint8_t k_radio_pipe_num = NRF24L01P_PIPE_P0;

//------------------------------------------------------------------------------
void radio_init()
{
	// NRF24LO1
	nrf24l01_cube_init(&hspi2, &huart1, NRF24L01_CE_GPIO_Port, NRF24L01_CE_Pin, NRF24L01_CSN_GPIO_Port, NRF24L01_CSN_Pin);

	nrf24l01_power_up();
	nrf24l01_enable();

	if (nrf24l01_is_connected())
	{
	  printf("%s", k_nrf24l01_connected);
	}
	else
	{
	  printf("%s", k_nrf24l01_not_connected);
	}

	//nrf24l01_set_rf_tx_power(NRF24L01P_TX_PWR_MINUS_12_DB);
	nrf24l01_set_transfer_size(sizeof(radio_msg), k_radio_pipe_num);
	// Set a proper mode
	nrf24l01_set_rx_mode();
	// Print all the regs
	nrf24l01_print_all_regs();

   // Display the (default) setup of the nRF24L01+ chip
	nrf24l01_print_chip_info();
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
    if (nrf24l01_available((uint8_t)recv_dev_id))
    {
        if (nrf24l01_read((uint8_t)recv_dev_id, data, data_len) > 0)
        {
        	printf("nrf24l01 data received successfully!\r\n");
        	msg_received = true;
        }
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
	return radio_send_data(k_radio_pipe_num, (const uint8_t*)msg, sizeof(radio_msg));
}

//------------------------------------------------------------------------------
bool radio_read_msg(radio_msg *msg)
{
	if (!msg)
	{
		return false;
	}

	return radio_read_data(k_radio_pipe_num, (uint8_t*)msg, sizeof(radio_msg));
}
