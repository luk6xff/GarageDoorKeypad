/*
 * eeprom.c
 *
 *  Created on: Jul 22, 2021
 *      Author: luk6xff
 */

#ifndef EEPROM_C_
#define EEPROM_C_

#include "eeprom.h"
#include "AT24CXX/platform/stm32cube/at24cxx-cube.h"
#include "main.h"
#include <stdio.h>

//------------------------------------------------------------------------------
// DEFINES, EXTERNS
//------------------------------------------------------------------------------
#define AT24C64_I2C_ADDR_PINS 0x00   /* |1|0|1|0|A2|A1|A0|R/W| - A2=0, A1=0, A0=0 */
extern I2C_HandleTypeDef hi2c2;

//------------------------------------------------------------------------------
// STATIC LOCAL Functions
//------------------------------------------------------------------------------
/**
 * @brief Computes a simple checksum, used for eeprom_data validation
 */
static uint32_t compute_checksum(const uint8_t *data, const uint8_t data_len);


//------------------------------------------------------------------------------
// STATIC LOCAL OBJECTS
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
static const eeprom_data eeprom_data_default =
{
	.magic = 0x12345678,
	.version = 0x00000002,
	.dev_id = 0x66BCDE77,
	.num_of_radio_stored = 0,
	.crc = 0x000000000
};

//------------------------------------------------------------------------------
static eeprom_data eeprom_data_current;

//------------------------------------------------------------------------------
// AT24C64 EEPROM
static at24cxx_cube at24c64_cube =
{
	.i2c = &hi2c2,
	.gpio_wp_port = AT24Cx_WP_GPIO_Port,
	.gpio_wp_pin = AT24Cx_WP_Pin,
};

static at24cxx at24c64 =
{
	.type = AT24C64,
};



//------------------------------------------------------------------------------
// FUNCTION DEFINITIONS
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
const eeprom_data* eeprom_data_get_defaults()
{
	return &eeprom_data_default;
}

//------------------------------------------------------------------------------
const eeprom_data* eeprom_data_get_current()
{
	return &eeprom_data_current;
}

//------------------------------------------------------------------------------
void eeprom_init()
{
	// AT24C64 EEPROM init
	at24cxx_cube_init(&at24c64, &at24c64_cube, AT24C64_I2C_ADDR_PINS);

    if (eeprom_data_read())
    {
        // Print content
    	eeprom_data_print_current();

        // Check if settings are valid
        if (eeprom_data_get_current()->magic == eeprom_data_get_defaults()->magic && \
        	eeprom_data_get_current()->version == eeprom_data_get_defaults()->version)
        {
            printf("eeprom_data: Read eeprom settings looks ok\r\n");
            // Check if values are ok
        }
        else
        {
            printf("eeprom_data: Read eeprom settings are invalid, updating with defaults\r\n");
            if (!eeprom_data_store(eeprom_data_get_defaults()))
            {
                printf("eeprom_data: Updating eeprom with defaults failed!, setting current as defaults\r\n");
                eeprom_data_current = eeprom_data_default;
            }
            else
            {
                printf("eeprom_data: Updating eeprom with defaults succeed!\r\n");
            }
        }
    }
    else
    {
        printf("eeprom_data: Read eeprom settings failed!, setting current as defaults\r\n");
        eeprom_data_current = eeprom_data_default;
    }
}

//------------------------------------------------------------------------------
bool eeprom_data_store(const eeprom_data *data)
{
	if (at24cxx_write(&at24c64, 0, (const uint8_t*)data, sizeof(eeprom_data)) == AT24CXX_NOERR)
	{
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
bool eeprom_data_read()
{
	if (at24cxx_read(&at24c64, 0, (uint8_t*)&eeprom_data_current, sizeof(eeprom_data)) == AT24CXX_NOERR)
	{
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
void eeprom_data_print_current()
{
	printf("eeprom_data: <<CURRENT EEPROM DATA>>\r\n");
	printf(".magic:0x%x\r\n", eeprom_data_get_current()->magic);
	printf(".version:0x%x\r\n", eeprom_data_get_current()->version);
	printf(".dev_id:0x%x\r\n", eeprom_data_get_current()->dev_id);
	printf(".num_of_radio_stored:%d\r\n", eeprom_data_get_current()->num_of_radio_stored);
	printf(".crc:0x%x\r\n", eeprom_data_get_current()->crc);
}

//------------------------------------------------------------------------------
// STATIC LOCAL FUNCTIONS DEFINITIONS
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
static uint32_t compute_checksum(const uint8_t *data, const uint8_t data_len)
{
    uint8_t i;
    uint32_t sum = 0;

    for (i = 0; i < data_len; i++)
    {
    	sum = sum ^ data[i];
    }
    return sum;
}

#endif /* EEPROM_C_ */
