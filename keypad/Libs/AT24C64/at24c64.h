/**
 *  @brief:   EEPROM AT24C64 library
 *  @author:  luk6xff
 *  @email:   lukasz.uszko@gmail.com
 *  @date:    2019-11-25
 *  @license: MIT
 */

#ifndef __AT24C64_H__
#define __AT24C64_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <string.h>


typedef enum
{
    AT24C64_NOERR = 0,
    AT24C64_ERR,
    AT24C64_OUT_OF_RANGE,
} AT24C64Status;

/**
 * @brief Initialize eeprom.
 *
 * @param  chip_addr Chip I2C address.
 * @param  chip_size  Number of bytes available on the chip.
 * @param  page_size  Number of bytes per page.
 */
void at24c64_init(uint8_t chipAddr, size_t chip_size, size_t page_size);

/**
 * @brief Deinitalize eeprom.
 *
 */
void at24c64_deinit(void);

/**
 * @brief Write a byte into memory.
 *
 * @param  addr  Start address.
 * @param  data  A byte to be written into memory.
 */
AT24C64Status at24c64_write_bytes(uint16_t addr, uint8_t data);

/**
 * @brief Read byte from memory.
 *
 * @param       addr  Start address.
 * @param [IN]  data  A byte to be read into memory.
 * @retval Status 
 */
AT24C64Status at24c64_read_byte(uint16_t addr, uint8_t* data);

/**
 * @brief Write page into memory.
 *
 * @param  addr     Start address.
 * @param  buf      Data to be written.
 * @param  buf_size  Number of bytes to be written (page_size bytes max).
 * @retval Status 
 */
AT24C64Status at24c64_write_page(uint16_t addr, uint8_t* buf, size_t buf_size);


/**
 * @brief Check if write or read operation can succeed.
 *
 * @param  addr     Start address.
 * @param  buf_size  Number of bytes to be written/read.
 * @retval Status value
 */
bool at24c64_check_space(uint16_t addr, size_t size);


//-----------------------------------------------------------------------------
// @brief HW dependent functions - must be defined for each platform
//-----------------------------------------------------------------------------
/**
 * @brief Init IO
 *
 * @retval Status value
 */
extern AT24C64Status at24c64_io_init();

/**
 * @brief Deinit IO
 *
 * @retval Status value
 */
extern AT24C64Status at24c64_io_deinit();

/**
 * @brief Write bytes into memory.
 *
 * @param       addr     Start address.
 * @param       buf      Data to be written
 * @param       buf_size  Number of bytes to be written (32 bytes max).
 * @retval Status value
 */
extern AT24C64Status at24c64_write_buffer(uint16_t addr, uint8_t* buf, size_t buf_size);

/**
 * @brief Read bytes from memory.
 *
 * @param       addr     Start address.
 * @param [IN]  buf      Buffer to fill with read bytes.
 * @param       buf_size  Number of bytes to read (32 bytes max).
 * @retval Status value
 */
extern AT24C64Status at24c64_read_buffer(uint16_t addr, uint8_t* buf, size_t buf_size);

/**
 * @brief Enable/Disable Write protection pin.
 *
 * @param enable  enables if true ot disables if false WriteProtectionPin
 */
extern void at24c64_enable_wp(bool enable);

/**
 * @brief Miliseconds delay.
 *
 * @param delay_ms: Delay in miliseconds.
 */
extern void at24c64_delay_ms(uint32_t delay_ms);


#ifdef __cplusplus
}
#endif

#endif /*__AT24C64_H__ */
