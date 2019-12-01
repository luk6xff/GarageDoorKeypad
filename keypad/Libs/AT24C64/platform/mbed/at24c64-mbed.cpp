#include "at24c64-mbed.h"



/**
 * I2C Interface
 */
static I2C* _i2c;

/**
 * AT24Cxx Write protection pin
 */
static DigitalOut* _wp;



//-----------------------------------------------------------------------------
void AT24C64_MbedInit(PinName sda, PinName scl, PinName wp, 
                      uint8_t chipAddr, size_t chipSize, size_t pageSize)
{
    _i2c = new I2C(sda, scl);
    _wp = new DigitalOut(wp); 
    AT24C64_Init(chipAddr, chipSize, pageSize);
}

//-----------------------------------------------------------------------------
void AT24C64_MbedDeInit(void)
{
    AT24C64_DeInit();
}

//-----------------------------------------------------------------------------
AT24C64Status AT24C64_IoInit()
{
    // Set I2C frequency
    _i2c->frequency(400000);
}

//-----------------------------------------------------------------------------
AT24C64Status AT24C64_IoDeInit()
{
    // Empty
    return AT24C64_NOERR;
}

//-----------------------------------------------------------------------------
AT24C64Status AT24C64_WriteBuffer(uint16_t addr, uint8_t* buf, size_t bufSize)
{
    // Check space
    if (!AT24C64_CheckSpace(addr, bufSize))
    {
        return AT24C64_OUT_OF_RANGE;
    }

    int ack = _i2c->write((int)addr, (char*)buf, bufSize);
    if (ack != 0)
    {
        return AT24C64_ERR;
    }
    return AT24C64_NOERR;
}

//-----------------------------------------------------------------------------
AT24C64Status AT24C64_ReadBuffer(uint16_t addr, uint8_t* buf, size_t bufSize)
{
    int retVal = _i2c->read(addr, (char*)buf, bufSize);
    if (retVal != 0)
    {
        return AT24C64_ERR;
    }
    return AT24C64_NOERR;
}

//-----------------------------------------------------------------------------
void AT24C64_EnableWriteProtection(bool enable)
{
    if (enable)
    {
        *_wp = 1;
        return;
    }
    *_wp = 0;
}


//-----------------------------------------------------------------------------
void AT24C64_DelayMs(uint32_t delayMs)
{
    wait_us(delayMs*1000);
}

//-----------------------------------------------------------------------------
