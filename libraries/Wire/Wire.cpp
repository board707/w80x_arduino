#include "Wire.h"

TwoWire Wire;

TwoWire::TwoWire() {}

TwoWire::~TwoWire() {}

void TwoWire::begin(void)
{
    if (i2c_dev != NULL) delete i2c_dev; //free(i2c_dev);
    HardwareI2C *r_dev = new HardwareI2C(); //(HardwareI2C*) malloc(sizeof(HardwareI2C));
    i2c_type = Hard_I2C;
    r_dev->begin();
    i2c_dev = r_dev;
    is_initialized = true;
}

void TwoWire::begin(uint8_t Sda, uint8_t Scl)
{
    if (i2c_dev != NULL) delete i2c_dev;
    SoftwareI2C *r_dev = new SoftwareI2C(); //(SoftwareI2C*) malloc(sizeof(SoftwareI2C));
    i2c_type = Soft_I2C;
    r_dev->begin(Sda, Scl);
    i2c_dev = r_dev;
    is_initialized = true;
}

void TwoWire::beginTransmission(uint8_t _addr)
{

    if (!is_initialized)
    {
        // ERROR
        return;
    }
    addr = _addr;
    rx_buf_st = 0;
    rx_buf_end = 0;
    tx_buf_idx = 0;
}

/*
 * Allow only 8 bit addresses to be used
 */
void TwoWire::beginTransmission(int _addr)
{
    beginTransmission((uint8_t)_addr);
}

/*
 * Call the process function to process the message if the TX
 * buffer has not overflowed.
 */
uint8_t TwoWire::endTransmission(bool flag)
{
    if (!is_initialized)
    {
        // ERROR
        return 1;
    }
    
    if (GETNAK == i2c_dev->beginTransmission(addr))
            return 1;
    
    
    if (tx_buf_idx)
    {
        for (uint8_t i = 0; i < tx_buf_idx; ++i)
        {
            if (!i2c_dev->write(tx_buf[i]))
            {
                tx_buf_idx = 0;
                return 4;
            }
        }
        
        
        tx_buf_idx = 0;
    }
    
    i2c_dev->endTransmission();
    return tx_buf_idx;
}

uint8_t TwoWire::endTransmission(void)
{
    return endTransmission(true);
}

/*
 * Request bytes from a slave device and process the request,
 * storing into the receiving buffer.
 */
uint8_t TwoWire::requestFrom(uint8_t _addr, int len)
{

    if (len > BUFFER_LENGTH)
    {
        len = BUFFER_LENGTH;
    }
    rx_buf_st = 0;
    rx_buf_end = 0;

    if ((!is_initialized) || (len == 0))
    {
        // ERROR
        return 0;
    }
    uint8_t result = i2c_dev->requestFrom(_addr, len);
    if (result == GETNAK)
        return 0;

    while (len)
    {
        rx_buf[rx_buf_end] = i2c_dev->read();
        rx_buf_end++;
        len--;
    }
    i2c_dev->endTransmission();
    return (rx_buf_end - rx_buf_st);
}

/*
 * Allow only 8 bit addresses to be used when requesting bytes
 */
uint8_t TwoWire::requestFrom(int _addr, int len)
{

    return requestFrom((uint8_t)_addr, len);
}

/*
 * Stack up bytes to be sent when transmitting
 */
uint8_t TwoWire::write(uint8_t data)
{
    if (!is_initialized)
    {
        // ERROR
        return 0;
    }
    if (tx_buf_idx == BUFFER_LENGTH)
    {
        return 0;
    }
    tx_buf[tx_buf_idx++] = data;
    return 1;
}

/*
 * Stack up bytes from the array to be sent when transmitting
 */
uint8_t TwoWire::write(uint8_t *data, int len)
{
    for (uint8_t i = 0; i < len; ++i)
    {
        if (!write(data[i]))
        {
            return i;
        }
    }
    return len;
}

/*
 * Ensure that a sending data will only be 8-bit bytes
 */
uint8_t TwoWire::write(int data)
{
    return write((uint8_t)data);
}

/*
 * Stack up bytes from a string to be sent when transmitting
 */
uint8_t TwoWire::write(char *data)
{
    uint8_t i = 0;
    while (data[i])
    {
        if (!write((uint8_t)data[i]))
        {
            return i;
        }
        i++;
    }
    i += write((uint8_t)data[i]);
    return i;
}

/*
 * Return the amount of bytes that is currently in the receiving buffer
 */
uint8_t TwoWire::available()
{
    return (rx_buf_end - rx_buf_st);
}

/*
 * Return the value of byte in the receiving buffer that is currently being
 * pointed to
 */
int TwoWire::read()
{
    if (available())
    {
        return rx_buf[rx_buf_st++];
    }
    else
        return -1;
}

 void TwoWire::setClock(int clockFrequency)
 {

    i2c_dev->setClock(clockFrequency);
 }
