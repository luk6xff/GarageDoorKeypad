/**
 *  @brief:   EEPROM AT24C64 library
 *  @author:  luk6xff
 *  @email:   lukasz.uszko@gmail.com
 *  @date:    2019-11-25
 *  @license: MIT
 */

#ifndef __AT24C64_MBED_H__
#define __AT24C64_MBED_H__

#include "mbed.h"
#include "../../at24c64.h"


/**
 * @brief Initialize eeprom with MBED dependent parameters.
 *
 */
void AT24C64_MbedInit(PinName sda, PinName scl, PinName wp, 
                      uint8_t chipAddr, size_t chipSize, size_t pageSize);

/**
 * @brief Deinitalize eeprom.
 *
 */
void AT24C64_MbedDeInit(void);


#endif /*__AT24C64_MBED_H__ */
