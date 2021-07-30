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

#define NUM_OF_SUPPORTED_RADIOS 10
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
    radio_config radio_configs[NUM_OF_SUPPORTED_RADIOS];
    uint32_t crc;
} eeprom_data;

void eeprom_init();

const eeprom_data * const eeprom_data_get_defaults();
eeprom_data * const eeprom_data_get_current();

bool eeprom_data_store(const eeprom_data* data);
bool eeprom_data_read();

void eeprom_data_print_current();

// Radio code utils
/**
 * @brief Check if a given radio code exists in memory
 *
 * @returns radio_config code.id if exists, -1 otherwise;
 */
int eeprom_check_if_radio_code_exists(const uint8_t *radio_code);
bool eeprom_check_if_radio_code_id_exists(uint8_t new_code_id);
bool eeprom_store_new_radio_code(const uint8_t *new_code,
                                    const uint8_t new_code_len,
									uint8_t new_code_id);

#endif /* EEPROM_H_ */
