
/**
 *  @brief:  Implementation of a NRF24L01 platform dependent [STM32CUBE] radio functions
 *  @author: luk6xff
 *  @email:  lukasz.uszko@gmail.com
 *  @date:   2019-12-01
 */

#include "nrf24l01-cube.h"


// SPI handle
static SPI_HandleTypeDef* _spi;
// GPIO
static GPIO_TypeDef* _ce_port;
static GPIO_TypeDef* _csn_port;
static uint16_t _ce_pin;
static uint16_t _csn_pin;
// UART
UART_HandleTypeDef* _uart;

//------------------------------------------------------------------------------
void nrf24l01_cube_init(SPI_HandleTypeDef* spi, UART_HandleTypeDef* uart,
						GPIO_TypeDef* GPIO_CE_Port, uint16_t GPIO_CE_Pin,
						GPIO_TypeDef* GPIO_CSN_Port, uint16_t GPIO_CSN_Pin)
{
	_spi = spi;
	_uart = uart;
	_ce_port = GPIO_CE_Port;
	_ce_pin = GPIO_CE_Pin;
	_csn_port = GPIO_CSN_Port;
	_csn_pin = GPIO_CSN_Pin;

    nrf24l01_init();
}

//------------------------------------------------------------------------------
void nrf24l01_cube_deinit()
{
	// Empty
}

//------------------------------------------------------------------------------
void nrf24l01_set_ce_pin(bool enable)
{
    if (enable)
    {
    	HAL_GPIO_WritePin(_ce_port, _ce_pin, GPIO_PIN_SET);
        return;
    }
	HAL_GPIO_WritePin(_ce_port, _ce_pin, GPIO_PIN_RESET);
}


//------------------------------------------------------------------------------
bool nrf24l01_get_ce_pin()
{
	return HAL_GPIO_ReadPin(_ce_port, _ce_pin);
}

//------------------------------------------------------------------------------
void nrf24l01_set_csn_pin(bool enable)
{
    if (enable)
    {
    	HAL_GPIO_WritePin(_csn_port, _csn_pin, GPIO_PIN_SET);
        return;
    }
	HAL_GPIO_WritePin(_csn_port, _csn_pin, GPIO_PIN_RESET);
}

//------------------------------------------------------------------------------
uint8_t nrf24l01_spi_write(uint8_t data)
{
	uint8_t rx_data;
	HAL_SPI_TransmitReceive(_spi, &data, &rx_data, 1, 100);
    return rx_data;
}

//------------------------------------------------------------------------------
void nrf24l01_delay_ms(uint32_t delay_ms)
{
	uint32_t tickstart_ms = HAL_GetTick();
	while((HAL_GetTick()-tickstart_ms) < delay_ms);
}

//------------------------------------------------------------------------------
void nrf24l01_print_all_regs()
{
    uint8_t reg_val;
    printf("\r\n<<<NRF24L01 REGISTERS>>>\r\nADDR - HEX\r\n");
    for (uint8_t reg_addr = 0; reg_addr <= 0x1D; reg_addr++)
    {
        reg_val = nrf24l01_read_reg(reg_addr);
        printf("0x%02x", reg_addr);
        printf(" - ");
        printf("0x%02x\r\n", reg_val);
    }
}

//-----------------------------------------------------------------------------
