#include "at24c64.h"


//-----------------------------------------------------------------------------
static uint8_t AT24C64_chip_addr= 0;
static size_t AT24C64_chip_size = 0; /*bytes*/
static size_t AT24C64_page_size = 0; /*bytes*/


//-----------------------------------------------------------------------------
void at24c64_init(uint8_t chipAddr, size_t chip_size, size_t page_size)
{
    AT24C64_chip_addr= chipAddr;
    AT24C64_chip_size = chip_size;
    AT24C64_page_size = page_size;

    at24c64_io_init();
}

//-----------------------------------------------------------------------------
void at24c64_deinit(void)
{
    at24c64_io_deinit();
}

//-----------------------------------------------------------------------------
AT24C64Status at24c64_write_bytes(uint16_t addr, uint8_t data)
{
    return at24c64_write_buffer(addr, &data, 1);
}

//-----------------------------------------------------------------------------
AT24C64Status at24c64_read_byte(uint16_t addr, uint8_t* data)
{ 
    AT24C64Status ret = AT24C64_ERR;
    if (data)
    {
        ret = at24c64_read_buffer(addr, data, 1);
    }
    return ret;
}

//-----------------------------------------------------------------------------
AT24C64Status at24c64_write_page(uint16_t addr, uint8_t* buf, size_t buf_size)
{
    AT24C64Status ret = AT24C64_ERR;
    // Write full page buffers.
    size_t bufferCount = buf_size / AT24C64_page_size;
    for (size_t i = 0; i < bufferCount; i++)
    {
        uint8_t offset = i * AT24C64_page_size;
        ret = at24c64_write_buffer(addr + offset, buf + offset, AT24C64_page_size);
        if (ret != AT24C64_NOERR)
        {
            return ret;
        }
    }

    // Write remaining bytes.
    uint8_t remainingBytes = buf_size % AT24C64_page_size;
    uint8_t offset = buf_size - remainingBytes;
    return at24c64_write_buffer(addr + offset, buf + offset, remainingBytes);  
}

//-----------------------------------------------------------------------------
bool at24c64_check_space(uint16_t addr, size_t size)
{
    // Only check if chip size is non-zero.
    if (AT24C64_chip_size > 0) {
        // Check that the address start in the chip and doesn't
        // extend too broad
        if ((addr >= AT24C64_chip_size) || ((addr + size) >= AT24C64_chip_size))
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
