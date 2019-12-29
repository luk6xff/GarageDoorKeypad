/**
 *  @brief:   NRF24L01+ radio module library
 *  @author:  luk6xff
 *  @email:   lukasz.uszko@gmail.com
 *  @date:    2019-12-01
 *  @license: MIT
 */

#ifndef __NRF24l01H__
#define __NRF24l01H__

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf24l01_registers.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>



/**
 * Defines
 */
#define NRF24L01P_TX_PWR_ZERO_DB         0
#define NRF24L01P_TX_PWR_MINUS_6_DB     -6
#define NRF24L01P_TX_PWR_MINUS_12_DB   -12
#define NRF24L01P_TX_PWR_MINUS_18_DB   -18
 
#define NRF24L01P_DATARATE_250_KBPS    250
#define NRF24L01P_DATARATE_1_MBPS     1000
#define NRF24L01P_DATARATE_2_MBPS     2000
 
#define NRF24L01P_CRC_NONE               0
#define NRF24L01P_CRC_8_BIT              8
#define NRF24L01P_CRC_16_BIT            16
 
#define NRF24L01P_MIN_RF_FREQUENCY    2400
#define NRF24L01P_MAX_RF_FREQUENCY    2525
 
#define NRF24L01P_PIPE_P0                0
#define NRF24L01P_PIPE_P1                1
#define NRF24L01P_PIPE_P2                2
#define NRF24L01P_PIPE_P3                3
#define NRF24L01P_PIPE_P4                4
#define NRF24L01P_PIPE_P5                5
 
// DEFAULT VALUES
#define DEFAULT_NRF24L01P_ADDRESS       ((uint64_t) 0xE7E7E7E7E7 )
#define DEFAULT_NRF24L01P_ADDRESS_WIDTH  5
#define DEFAULT_NRF24L01P_CRC            NRF24L01P_CRC_8_BIT
#define DEFAULT_NRF24L01P_RF_FREQUENCY  (NRF24L01P_MIN_RF_FREQUENCY + 2)
#define DEFAULT_NRF24L01P_DATARATE       NRF24L01P_DATARATE_1_MBPS
#define DEFAULT_NRF24L01P_TX_PWR         NRF24L01P_TX_PWR_ZERO_DB
#define DEFAULT_NRF24L01P_TRANSFER_SIZE  4


//------------------------------------------------------------------------------
//  FUNCTIONS
//------------------------------------------------------------------------------
/**
 * @brief Module init function
 */
void nrf24l01_init(void);

/**
 * @brief Module check SPI connection function
 */
bool nrf24l01_is_connected();

/**
* @brief Set the RF frequency.
*
* @param frequency the frequency of RF transmission in MHz (2400..2525).
*/
bool nrf24l01_set_rf_freq(int frequency);

/**
 * @brief Get the RF frequency.
 *
 * @return the frequency of RF transmission in MHz (2400..2525).
 */
int nrf24l01_get_rf_freq(void);

/**
 * @brief Set Power Amplifier (PA) level to one of four levels
 *
 * @param power the RF output power level in dBm: (0, -6, -12 or -18).
 */
void nrf24l01_set_rf_tx_power(int power);

/**
 * @brief Get the RF Power Amplifier (PA) level.
 *
 * @return The RF output power level in dBm (0, -6, -12 or -18).
 */
int nrf24l01_get_rf_tx_power(void);

/**
 * @brief Set the transmission data rate.
 *
 * @param rate The transmission data rate in kbps (250, 1M or 2M).
 */
void nrf24l01_set_data_rate(int rate);

/**
 * @brief Get the transmission data rate.
 *
 * @return The transmission data rate in kbps (250, 1M or 2M).
 */
int nrf24l01_get_data_rate(void);

/**
 * @brief Set the CRC length.
 *
 * @param length the number of bits for the CRC (0, 8 or 16).
 */
void nrf24l01_set_crc_length(int length);

/**
 * @brief Get the CRC length.
 *
 * @return the number of bits for the CRC (0, 8 or 16).
 */
int nrf24l01_get_crc_length(void);

/**
 * @brief Set the Receive address.
 *
 * @param address address associated with the particular pipe_num
 * @param length length of the address in bytes (3..5)
 * @param pipe_num pipe_num to associate the address with (0..5, default 0)
 *
 * @note Pipes 0 & 1 have 3, 4 or 5 byte addresses,
 *       while Pipes 2..5 only use the lowest byte (bits 7..0) of the
 *       address provided here, and use 2, 3 or 4 bytes from Pipe 1's address.
 *       The length parameter is ignored for Pipes 2..5.
 */
void nrf24l01_set_rx_addr(uint64_t addr, int length, uint8_t pipe_num);

/**
 * @brief Set the Transmit address.
 *
 * @param address address for transmission
 * @param length length of the address in bytes (3..5)
 *
 * @note The address length is shared with the Receive pipes,
 *       so a change to that address length affect transmissions.
 */
void nrf24l01_set_tx_addr(uint64_t addr, int length);

/**
 * @brief Get the Receive address.
 *
 * @param pipe_num pipe_num to get the address from (0..5, default 0)
 * @return the address associated with the particular pipe_num
 */
uint64_t nrf24l01_get_rx_addr(uint8_t pipe_num);

/**
 * @brief Get the Transmit address.
 *
 * @return address address for transmission
 */
uint64_t nrf24l01_get_tx_addr(void);

/**
 * @brief Set the transfer size.
 *
 * @param size the size of the transfer, in bytes (1..32)
 * @param pipe_num pipe_num for the transfer (0..5, default 0)
 */
void nrf24l01_set_transfer_size(uint8_t size, uint8_t pipe_num);

/**
 * @brief Get the transfer size.
 *
 * @return the size of the transfer, in bytes (1..32).
 */
int nrf24l01_get_transfer_size(uint8_t pipe_num);


/**
 * @brief Get the RPD (Received Power Detector) state.
 *
 * @return true if the received power exceeded -64dBm
 */
bool nrf24l01_get_rpd(void);

/**
 * @brief Put the nRF24L01+ into Receive mode
 */
void nrf24l01_set_rx_mode(void);

/**
 * @brief Put the nRF24L01+ into Transmit mode
 */
void nrf24l01_set_tx_mode(void);

/**
 * @brief Leave low-power mode - required for normal radio operation after calling nrf24l01_power_down()
 *
 * @note To return to low power mode, call nrf24l01_power_down().
 * @note This will take up to 5ms for maximum compatibility
 */
void nrf24l01_power_up(void);

/**
 * @brief Enter low-power mode
 *
 * @note To return to normal power mode, call nrf24l01_power_up().
 *
 * @note After calling startListening(), a basic radio will consume about 13.5mA
 * 		 at max PA level.
 * 		 During active transmission, the radio will consume about 11.5mA, but this will
 * 		 be reduced to 26uA (.026mA) between sending.
 * 		 In full nrf24l01_power_down mode, the radio will consume approximately 900nA (.0009mA)
 */
void nrf24l01_power_down(void);

/**
 * @brief Enable the nRF24L01+ to Receive or Transmit (using the CE pin)
 */
void nrf24l01_enable(void);

/**
 * @brief Disable the nRF24L01+ to Receive or Transmit (using the CE pin)
 */
void nrf24l01_disable(void);

/**
 * @brief Transmit data
 *
 * @param pipe_num is ignored (included for consistency with file nrf24l01_write routine)
 * @param data pointer to an array of bytes to nrf24l01_write
 * @param len the number of bytes to send (1..32)
 * @return the number of bytes actually written, or -1 for an error
 */
int nrf24l01_write(uint8_t pipe_num, uint8_t *data, int len);

/**
 * @brief Receive data
 *
 * @param pipe_num the receive pipe_num to get data from
 * @param data pointer to an array of bytes to store the received data
 * @param len the number of bytes to receive (1..32)
 * @return the number of bytes actually received, 0 if none are received, or -1 for an error
 */
int nrf24l01_read(uint8_t pipe_num, uint8_t *data, int len);

/**
 * @brief Determine if there is data available to nrf24l01_read
 *
 * @param pipe_num the receive pipe_num to check for data
 * @return true if the is data waiting in the given pipe_num
 */
bool nrf24l01_available(uint8_t pipe_num);

/**
 * @brief Disable all receive pipes
 *
 * @note receive pipes are nrf24l01_enabled when their address is set.
 */
void nrf24l01_disable_all_rx_pipes(void);

/**
 * @brief Disable AutoAcknowledge function
 */
void nrf24l01_disable_auto_ack(void);

/**
 * @brief Enable AutoAcknowledge function
 *
 * @param pipe_num the receive pipe_num
 */
void nrf24l01_enable_auto_ack(uint8_t pipe_num);

/**
 * @brief Disable AutoRetransmit function
 */
void nrf24l01_disable_auto_retransmit(void);

/**
 * @brief Enable AutoRetransmit function
 *
 * @param delay the delay between restransmits, in uS (250uS..4000uS)
 * @param len number of retransmits before generating an error (1..15)
 */
void nrf24l01_enable_auto_retransmit(int delay, int len);

/**
 * @brief Get the content of an addressable register.
 *
 * @param reg_addr address of the register
 * @return the content of the register
 */
int nrf24l01_read_reg(uint8_t reg_addr);

/**
 * @brief Set the content of an addressable register.
 *
 * @param reg_addr address of the register
 * @param reg_data data to nrf24l01_write to the register
 */
void nrf24l01_write_reg(uint8_t reg_addr, uint8_t reg_data);

/**
 * @brief Get the content of the status register.
 *
 * @return The content of the status register
 */
int nrf24l01_get_status_reg(void);


//-----------------------------------------------------------------------------
// @brief HW dependent functions - must be defined for each platform
//-----------------------------------------------------------------------------
/**
 * @brief write bytes into memory via SPI.
 *
 * @param  data   A byte of data to be written
 * @retval Read byte of data from SPI
 */
extern uint8_t nrf24l01_spi_write(uint8_t data);

/**
 * @brief Enable/Disable CE (chip enable) pin.
 *
 * @param enable  Set ot HIGH if true, LOW otherwise.
 */
extern void nrf24l01_set_ce_pin(bool enable);

/**
 * @brief Return CE (chip enable) pin state.
 *
 * @return true-HIGH, false-LOW.
 */
extern bool nrf24l01_get_ce_pin();

/**
 * @brief Enable/Disable CSN (chip enable) pin.
 *
 * @param enable  Set ot HIGH if true, LOW otherwise.
 */
extern void nrf24l01_set_csn_pin(bool enable);

/**
 * @brief Miliseconds delay.
 *
 * @param delay_ms: Delay in miliseconds.
 */
extern void nrf24l01_delay_ms(uint32_t delay_ms);

/**
 * @brief Prints the content of all registers.
 *
 */
extern void nrf24l01_print_all_regs();


#ifdef __cplusplus
}
#endif

#endif /*__NRF24l01H__ */
