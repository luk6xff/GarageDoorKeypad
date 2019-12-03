
/**
 *  @brief:  Implementation of a NRF24L01 platform dependent [MBED] radio functions
 *  @author: luk6xff
 *  @email:  lukasz.uszko@gmail.com
 *  @date:   2019-12-02
 */


#ifndef __NRF24L01_MBED_H__
#define __NRF24L01_MBED_H__

#include "mbed.h"
#include "../../nrf24l01.h"


void nrf24l01_mbed_init(PinName mosi, PinName miso, PinName sck, 
                        PinName csn, PinName ce, PinName irq);

void nrf24l01_mbed_deinit();

#endif // __NRF24L01_MBED_H__
