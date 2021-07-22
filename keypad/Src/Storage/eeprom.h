/*
 * eeprom.h
 *
 *  Created on: Jul 22, 2021
 *      Author: luk6xff
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include <stdint.h>
#include <stdbool.h>

#define NUM_OF_SUPPORTED_RADIOS 8
#define RADIO_CODE_SIZE 4


typedef struct
{
	uint32_t id;
	uint8_t code[RADIO_CODE_SIZE];
} radio_config;

/**
 * @brief EEPROM settings
 */
typedef struct __attribute__((packed, aligned(1)))
{
    uint32_t magic;
    uint32_t version;
    uint32_t dev_id;
    uint32_t num_of_radio_stored;
    radio_config radio_configs[NUM_OF_SUPPORTED_RADIOS];
    uint32_t crc;
} eeprom_data;

void eeprom_init();

const eeprom_data* eeprom_data_get_defaults();
const eeprom_data* eeprom_data_get_current();

bool eeprom_data_store(const eeprom_data* data);
bool eeprom_data_read();

void eeprom_data_print_current();


#endif /* EEPROM_H_ */
