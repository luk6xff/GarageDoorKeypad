/**
 *  @brief:   NRF24L01+ radio module library
 *  @author:  luk6xff
 *  @email:   lukasz.uszko@gmail.com
 *  @date:    2019-12-01
 *  @license: MIT
 */

#ifndef __nrf24l01H__
#define __nrf24l01H__

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf24l01registers.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

//CONSTANTS - you can change it depending on your needs
#define NUMBER_OF_BYTES_IN_FIFO 10 			// default value of how many bytes you can send/receive to/from the FIFO (the same value ought to be set in RX_PW_Px registers )				//the same value has to be set up in SETUP_AW REGISTER
static const BYTE RX_ADDR_P0[DATA_PIPE_LENGTH] =
		{ 0x35, 0x36, 0x37, 0x38, 0x01 }; 		//0x0A  // write the number of bytes defined in SETUP_AW (AW) // LSB written first
static const BYTE RX_ADDR_P1[DATA_PIPE_LENGTH] =
		{ 0x15, 0x16, 0x17, 0x18, 0x01 }; 		//0x0B
static const BYTE RX_ADDR_P2[] = { 0xA3 };
static const BYTE RX_ADDR_P3[] = { 0xA4 };
static const BYTE RX_ADDR_P4[] = { 0xA5 };
static const BYTE RX_ADDR_P5[] = { 0xA6 };
static const BYTE TX_ADDR[DATA_PIPE_LENGTH] = { 0x35, 0x36, 0x37, 0x38, 0x01 }; //0x10 // write the number of bytes defined in SETUP_AW (AW) // LSB written first

//------------------------------------------------------------------------------
//  FUNTIONS
//------------------------------------------------------------------------------


/**
 * @brief Module init function
 */
void nrf24l01_init(void);

/* Post transmission functions */
uint8_t nrf24l01_last_msg_status();
uint8_t nrf24l01_retransmission_count();

/* Returns the payload length */
uint8_t nrf24l01_payload_length();

/* state check functions */
uint8_t nrf24l01_data_ready();
uint8_t nrf24l01_is_sending();
uint8_t nrf24l01_read_status(void);
void nrf24l01_flush_status(void);
uint8_t nrf24l01_rxFifoEmpty();

void nrf24l01_setTxMode(void);
void nrf24l01_setRxMode(void);
void nrf24l01_flushTx(void);
void nrf24l01_flushRx(void) ;
void nrf24l01_powerUp(void);
void nrf24l01_powerDown(void);
void nrf24l01_nop(void);

void nrf24l01_read_config(void);
void nrf24l01_send_data(uint8_t*data);
void nrf24l01_set_ce_pin(void);
void nrf24l01_reset_ce_pin(void);


//-----------------------------------------------------------------------------
// @brief HW dependent functions - must be defined for each platform
//-----------------------------------------------------------------------------
/**
 * @brief Init IO
 *
 * @retval Status value
 */
extern void  nrf24l01_io_init();

/**
 * @brief Deinit IO
 *
 * @retval Status value
 */
extern void nrf24l01_io_deinit();

/**
 * @brief Write bytes into memory.
 *
 * @param       addr      Register address.
 * @param       buf       Data to be written
 * @param       buf_size  Number of bytes to be written (32 bytes max).
 * @retval Status value
 */
extern bool nrf24l01_write_buffer(uint16_t reg, uint8_t* buf, size_t buf_size);

/**
 * @brief Read bytes from memory.
 *
 * @param       reg       Register address.
 * @param [IN]  buf       Buffer to fill with read bytes.
 * @param       buf_size  Number of bytes to read (32 bytes max).
 * @retval Status value
 */
extern bool nrf24l01_read_buffer(uint16_t reg, uint8_t* buf, size_t buf_size);

/**
 * @brief Enable/Disable Write protection pin.
 *
 * @param enable  enables if true ot disables if false WriteProtectionPin
 */
extern void nrf24l01_enable_wp(bool enable);

/**
 * @brief Miliseconds delay.
 *
 * @param delay_ms: Delay in miliseconds.
 */
extern void nrf24l01_delay_ms(uint32_t delay_ms);


#ifdef __cplusplus
}
#endif

#endif /*__nrf24l01H__ */
