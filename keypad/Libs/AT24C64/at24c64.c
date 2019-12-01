#include "at24c64.h"


//-----------------------------------------------------------------------------
static uint8_t AT24C64_ChipAddr = 0;
static size_t AT24C64_ChipSize = 0; /*bytes*/
static size_t AT24C64_PageSize = 0; /*bytes*/


//-----------------------------------------------------------------------------
void AT24C64_Init(uint8_t chipAddr, size_t chipSize, size_t pageSize)
{
    AT24C64_ChipAddr = chipAddr;
    AT24C64_ChipSize = chipSize;
    AT24C64_PageSize = pageSize;

    AT24C64_IoInit();
}

//-----------------------------------------------------------------------------
void AT24C64_DeInit(void)
{
    AT24C64_IoDeInit();
}

//-----------------------------------------------------------------------------
AT24C64Status AT24C64_WriteByte(uint16_t addr, uint8_t data)
{
    return AT24C64_WriteBuffer(addr, &data, 1);
}

//-----------------------------------------------------------------------------
AT24C64Status AT24C64_ReadByte(uint16_t addr, uint8_t* data)
{ 
    AT24C64Status ret = AT24C64_ERR;
    if (data)
    {
        ret = AT24C64_ReadBuffer(addr, data, 1);
    }
    return ret;
}

//-----------------------------------------------------------------------------
AT24C64Status AT24C64_WritePage(uint16_t addr, uint8_t* buf, size_t bufSize)
{
    AT24C64Status ret = AT24C64_ERR;
    // Write full page buffers.
    size_t bufferCount = bufSize / AT24C64_PageSize;
    for (size_t i = 0; i < bufferCount; i++)
    {
        uint8_t offset = i * AT24C64_PageSize;
        ret = AT24C64_WriteBufferr(addr + offset, buf + offset, AT24C64_PageSize);
        if (ret != AT24C64_NOERR)
        {
            return ret;
        }
    }

    // Write remaining bytes.
    uint8_t remainingBytes = bufSize % AT24C64_PageSize;
    uint8_t offset = bufSize - remainingBytes;
    return AT24C64_WriteBuffer(addr + offset, buf + offset, remainingBytes);  
}

//-----------------------------------------------------------------------------
bool AT24C64_CheckSpace(uint16_t addr, size_t size)
{
    // Only check if chip size is non-zero.
    if (AT24C64_ChipSize > 0) {
        // Check that the address start in the chip and doesn't
        // extend too broad
        if ((addr >= AT24C64_ChipSize) || ((addr + size) >= AT24C64_ChipSize))
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    return false;  
}