
/**
 *  @brief:  Implementation of a NRF24L01 platform dependent [MBED] radio functions
 *  @author: luk6xff
 *  @email:  lukasz.uszko@gmail.com
 *  @date:   2019-11-15
 */

#include "nrf24l01-mbed.h"

/**
 * SPI Interface
 */
static SPI* _spi; // mosi, miso, sclk
static DigitalOut* _csn;

/**
 * NRF24L01 pins
 */
static DigitalOut* _ce;
static InterruptIn* _irq;

//------------------------------------------------------------------------------
void nrf24l01_mbed_init(PinName mosi, PinName miso, PinName sck, 
                        PinName csn, PinName ce, PinName irq);
{
    _spi = new SPI(mosi, miso, sclk);
    _csn = new DigitalOut(csn);
    _ce = new DigitalOut(ce);
    _irq = new InterruptIn(irq);

    // Init SPI
    *_csn = 1;    
    _spi->format(8,0);   
    _spi->frequency(4000000);
    nrf24l01_delay_ms(10);

    nrf24l01_init();
}

//------------------------------------------------------------------------------
void nrf24l01_mbed_deinit()
{
    // IO
    delete(_spi);
    delete(_csn);
    delete(_ce);
    delete(_irq);
}

//------------------------------------------------------------------------------
// void nrf24l01_irq_init(IrqHandler *irq_handler)
// {
//     // dio0
//     _irq->rise(mbed::callback(irqHandler));
// }

//------------------------------------------------------------------------------
void nrf24l01_set_ce_pin(bool enable)
{
    if (enable)
    {
        *_ce = 1;
        return;
    }
    *_ce = 0;
}


//------------------------------------------------------------------------------
void nrf24l01_set_csn_pin(bool enable)
{
    if (enable)
    {
        *_csn = 1;
        return;
    }
    *_csn = 0;
}

//------------------------------------------------------------------------------
uint8_t nrf24l01_spi_write(uint8_t data)
{
    return _spi->write(data);
}

//------------------------------------------------------------------------------
void nrf24l01_delay_ms(int ms)
{
    wait_us(ms*1000);
}

//------------------------------------------------------------------------------
void nrf24l01_print_all_regs()
{
    uint8_t reg_val;
    debug("\r\n<<<NRF24L01 REGISTERS>>>\r\nADDR - HEX\r\n");
    for (uint8_t reg_addr = 1; reg_addr <= 0x4F; reg_addr++)
    {
        reg_val = nrf24l01_read_reg(reg_addr);
        debug("0x%x", reg_addr);
        debug(" - ");
        debug("0x%x\r\n", reg_val);
    }
}

//-----------------------------------------------------------------------------
