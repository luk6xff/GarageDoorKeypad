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
 * @param  chipAddr  Chip I2C address.
 * @param  chipSize  Number of bytes available on the chip.
 * @param  pageSize  Number of bytes per page.
 */
void AT24C64_Init(uint8_t chipAddr, size_t chipSize, size_t pageSize);

/**
 * @brief Deinitalize eeprom.
 *
 */
void AT24C64_DeInit(void);

/**
 * @brief Write a byte into memory.
 *
 * @param  addr  Start address.
 * @param  data  A byte to be written into memory.
 */
AT24C64Status AT24C64_WriteByte(uint16_t addr, uint8_t data);

/**
 * @brief Read byte from memory.
 *
 * @param       addr  Start address.
 * @param [IN]  data  A byte to be read into memory.
 * @retval Status 
 */
AT24C64Status AT24C64_ReadByte(uint16_t addr, uint8_t* data);

/**
 * @brief Write page into memory.
 *
 * @param  addr     Start address.
 * @param  buf      Data to be written.
 * @param  bufSize  Number of bytes to be written (pageSize bytes max).
 * @retval Status 
 */
AT24C64Status AT24C64_WritePage(uint16_t addr, uint8_t* buf, size_t bufSize);


/**
 * @brief Check if write or read operation can succeed.
 *
 * @param  addr     Start address.
 * @param  bufSize  Number of bytes to be written/read.
 * @retval Status value
 */
bool AT24C64_CheckSpace(uint16_t addr, size_t size);


//-----------------------------------------------------------------------------
// @brief HW dependent functions - must be defined for each platform
//-----------------------------------------------------------------------------
/**
 * @brief Init IO
 *
 * @retval Status value
 */
extern AT24C64Status AT24C64_IoInit();

/**
 * @brief Deinit IO
 *
 * @retval Status value
 */
extern AT24C64Status AT24C64_IoDeInit();

/**
 * @brief Write bytes into memory.
 *
 * @param       addr     Start address.
 * @param       buf      Data to be written
 * @param       bufSize  Number of bytes to be written (32 bytes max).
 * @retval Status value
 */
extern AT24C64Status AT24C64_WriteBuffer(uint16_t addr, uint8_t* buf, size_t bufSize);

/**
 * @brief Read bytes from memory.
 *
 * @param       addr     Start address.
 * @param [IN]  buf      Buffer to fill with read bytes.
 * @param       bufSize  Number of bytes to read (32 bytes max).
 * @retval Status value
 */
extern AT24C64Status AT24C64_ReadBuffer(uint16_t addr, uint8_t* buf, size_t bufSize);

/**
 * @brief Enable/Disable Write protection pin.
 *
 * @param enable  enables if true ot disables if false WriteProtectionPin
 */
extern void AT24C64_EnableWriteProtection(bool enable);

/**
 * @brief Miliseconds delay.
 *
 * @param delayMs: Delay in miliseconds.
 */
extern void AT24C64_DelayMs(uint32_t delayMs);


#ifdef __cplusplus
}
#endif

#endif /*__AT24C64_H__ */
