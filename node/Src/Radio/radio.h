/*
 * radio.h
 *
 *  Created on: Jul 22, 2021
 *      Author: luk6xff
 */

#ifndef RADIO_RADIO_H_
#define RADIO_RADIO_H_

#include <stdint.h>
#include <stdbool.h>

#include "radio_msg.h"


/**
 * @brief Initializes radio and all its components.
 */
void radio_init();

/**
 * @brief Deinit IOs and puts radio into sleep mode.
 */
void radio_deinit();

/**
 * @brief Sends data to the node
 */
bool radio_send_data(const uint16_t recv_dev_id, const uint8_t *data, const uint32_t data_len);

/**
 * @brief Reads data received from the node
 */
bool radio_read_data(const uint16_t recv_dev_id, uint8_t *data, const uint32_t data_len);

/**
 * @brief Sends radio message to the node
 */
bool radio_send_msg(const radio_msg *msg);

/**
 * @brief Reads radio message from the node.
 */
bool radio_read_msg(radio_msg *msg);

#endif /* RADIO_RADIO_H_ */
