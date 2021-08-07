/*
 * radio.c
 *
 *  Created on: Jul 22, 2021
 *      Author: luk6xff
 */


#include "radio.h"
#include "NRF24L01+/platform/stm32cube/nrf24l01p-cube.h"
#include "main.h"
#include <stdio.h>


//------------------------------------------------------------------------------
extern SPI_HandleTypeDef hspi2;
//------------------------------------------------------------------------------
const uint8_t k_nrf24l01p_connected[] = "NRF24L01+ module connected properly\r\n";
const uint8_t k_nrf24l01p_not_connected[] = "NRF24L01+ module NOT connected!!!\r\n";
// Radio pipe
const uint8_t k_radio_pipe_num = NRF24L01P_PIPE_P0;

//------------------------------------------------------------------------------
static nrf24l01p_cube dev_cube =
{
	.spi = &hspi2,
	.ce_port = NRF24L01_CE_GPIO_Port,
	.ce_pin = NRF24L01_CE_Pin,
	.csn_port = NRF24L01_CSN_GPIO_Port,
	.csn_pin = NRF24L01_CSN_Pin,
};

static nrf24l01p dev;

//------------------------------------------------------------------------------
void radio_init()
{
	// NRF24LO1
	nrf24l01p_cube_init(&dev, &dev_cube);

	nrf24l01p_power_up(&dev);
	nrf24l01p_enable(&dev);

	if (nrf24l01p_is_connected(&dev))
	{
	  printf("%s", k_nrf24l01p_connected);
	}
	else
	{
	  printf("%s", k_nrf24l01p_not_connected);
	}

	// Set a lowest data rate = 250 KBPS
	nrf24l01p_set_data_rate(&dev, NRF24L01P_DATARATE_250_KBPS);
	// Set a transfer size,
	nrf24l01p_set_transfer_size(&dev, sizeof(radio_msg), k_radio_pipe_num);
	// Set a proper mode
	nrf24l01p_set_rx_mode(&dev);
	// Print all the regs
	nrf24l01p_print_all_regs(&dev);

	// Display the (default) setup of the nRF24L01+ chip
	nrf24l01p_print_chip_info(&dev);
}

//------------------------------------------------------------------------------
void radio_deinit()
{
	nrf24l01p_power_down(&dev);
	nrf24l01p_disable(&dev);
}

//------------------------------------------------------------------------------
void radio_resume()
{
	// Enable radio
	nrf24l01p_enable(&dev);
	// Set an RX mode
	nrf24l01p_set_rx_mode(&dev);
}

//------------------------------------------------------------------------------
bool radio_send_data(const uint16_t recv_dev_id, const uint8_t *data,
                    const uint32_t data_len)
{
    int ret = nrf24l01p_write(&dev, (uint8_t)recv_dev_id, data, data_len);
    if (ret > 0)
    {
    	printf("nrf24l01 data sent successfully!\r\n");
    }
    return ret;
}

//------------------------------------------------------------------------------
bool radio_read_data(const uint16_t recv_dev_id, uint8_t *data, const uint32_t data_len)
{
    bool msg_received = false;

    // Check for any message
    if (nrf24l01p_available(&dev, (uint8_t)recv_dev_id))
    {
        if (nrf24l01p_read(&dev, (uint8_t)recv_dev_id, data, data_len) > 0)
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
