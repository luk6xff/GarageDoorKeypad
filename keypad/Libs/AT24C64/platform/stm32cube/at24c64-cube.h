/**
 *  @brief:   EEPROM AT24C64 library - stm32cube platform
 *  @author:  luk6xff
 *  @email:   lukasz.uszko@gmail.com
 *  @date:    2019-12-01
 *  @license: MIT
 */

#ifndef __AT24C64_CUBE_H__
#define __AT24C64_CUBE_H__

#include "../../at24c64.h"

/**
 * @brief Initialize eeprom with STM32 CUBE dependent parameters.
 *
 */
void at24c64_cube_init(I2C_HandleTypeDef* i2c, uint8_t chipAddr, size_t chip_size, size_t page_size);

/**
 * @brief Deinitalize eeprom.
 *
 */
void at24c64_cube_deinit(void);


#endif /*__AT24C64_CUBE_H__ */
