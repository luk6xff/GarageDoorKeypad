#include "at24cxx-cube.h"


//-------------------------------------------------------------------------------
void at24cxx_cube_init(at24cxx* const dev, at24cxx_cube* const cube_dev)
{
    dev->platform_dev = cube_dev;
    at24cxx_init(dev);
}

//------------------------------------------------------------------------------
void at24cxx_cube_deinit(at24cxx* const dev)
{
    at24cxx_io_deinit(dev);
}

//------------------------------------------------------------------------------
at24cxx_status at24cxx_io_init(at24cxx* const dev)
{
	// Empty
    return AT24CXX_NOERR;
}

//------------------------------------------------------------------------------
at24cxx_status at24cxx_io_deinit(at24cxx* const dev)
{
    // Empty
    return AT24CXX_NOERR;
}

//------------------------------------------------------------------------------
at24cxx_status at24cxx_write_buffer(const at24cxx* const dev, uint32_t addr,
                                    const uint8_t* buf, size_t buf_size)
{
    // Check space
	if (!at24cxx_check_space(dev, addr, buf_size))
    {
        return AT24CXX_OUT_OF_RANGE;
    }

    const at24cxx_cube* const pd = (at24cxx_cube*)dev->platform_dev;

    uint8_t tmp[2+buf_size];
    tmp[0] = addr >> 8;
    tmp[1] = addr;
    memcpy(&tmp[2], buf, buf_size);

    int ack = HAL_I2C_Master_Transmit(pd->i2c, dev->addr, tmp, sizeof(tmp), 1000);
    if (ack != 0)
    {
        return AT24CXX_ERR;
    }
    return AT24CXX_NOERR;
}

//------------------------------------------------------------------------------
at24cxx_status at24cxx_read_buffer(const at24cxx* const dev, uint32_t addr,
                                   uint8_t* buf, size_t buf_size)
{
    const at24cxx_cube* const pd = (at24cxx_cube*)dev->platform_dev;

    const uint8_t addr_len = 2; //at24cxx_devices[dev->type].word_addr_len; // TODO!!!
    uint8_t address[addr_len];
    address[0] = addr >> 8;
    address[1] = addr;

    // Write addr
    int ack = HAL_I2C_Master_Transmit(pd->i2c, dev->addr, (char*)address, addr_len, 1000);
    if (ack != 0)
    {
        return AT24CXX_I2C_ERR;
    }

    // Sequential Read
    int retVal = HAL_I2C_Master_Receive(pd->i2c, dev->addr, buf, buf_size, 1000);
    if (retVal != 0)
    {
        return AT24CXX_ERR;
    }
    return AT24CXX_NOERR;
}

//------------------------------------------------------------------------------
void at24cxx_enable_wp(at24cxx* const dev, bool enable)
{
    const at24cxx_cube* const pd = (at24cxx_cube*)dev->platform_dev;
    if (enable)
    {
    	HAL_GPIO_WritePin(pd->gpio_wp_port, pd->gpio_wp_pin, GPIO_PIN_SET);
        return;
    }
	HAL_GPIO_WritePin(pd->gpio_wp_port, pd->gpio_wp_pin, GPIO_PIN_RESET);
}


//------------------------------------------------------------------------------
void at24cxx_delay_ms(uint32_t delay_ms)
{
	  uint32_t tickstart_ms = HAL_GetTick();
	  while((HAL_GetTick()-tickstart_ms) < delay_ms);
}

//------------------------------------------------------------------------------
