/**
 * @file WireBase.h
 * 
 * @author Dmitry DMitriev
 * @brief Wire library for Arduino.
 *        Provides a 'standard' interface to I2C 
 */


#ifndef _WIRE_H_
#define _WIRE_H_

#include "Arduino.h"
#include "HardwareI2C.h"
#include "SoftwareI2C.h"

#define BUFFER_LENGTH 64

#define Hard_I2C 0
#define Soft_I2C 1


class TwoWire {
protected:
    WireBase* i2c_dev = NULL;
    bool i2c_type = Hard_I2C;
    bool is_initialized = false;
    uint8_t addr;
   
    uint8_t rx_buf[BUFFER_LENGTH];      /* receive buffer */
    uint8_t rx_buf_st;               /* first unread idx in rx_buf */
    uint8_t rx_buf_end;               /* last unread byte in buf */

    uint8_t tx_buf[BUFFER_LENGTH];      /* transmit buffer */
    uint8_t tx_buf_idx;              // next idx available in tx_buf
    //bool tx_buf_overflow;

   
public:
    TwoWire();
    ~TwoWire();

    /*
     * Initialises the class interface
     */
    
    void begin(void);
    void begin(uint8_t Sda, uint8_t Scl);

    /*
     * Sets up the transmission message to be processed
     */
    void beginTransmission(uint8_t);
    void beginTransmission(int);

    /*
     * Process TX buffer
     */
    uint8_t endTransmission(bool);
    uint8_t endTransmission(void);

    /*
     * Request bytes from a slave device and process the request,
     * storing into the receiving buffer.
     */
    uint8_t requestFrom(uint8_t, int);
    uint8_t requestFrom(int, int);
    // added for compatibility
    uint8_t requestFrom(int addr, int len, int stop)  {
        UNUSED(stop);
        return requestFrom(addr, len);
    }
    /*
     * Stack up bytes to be sent when transmitting
     */
    
    uint8_t write(uint8_t);
    uint8_t write(uint8_t*, int);
    uint8_t write(int);

    // added for compatibility
    uint8_t write(const uint8_t* data, int len)  {
        uint8_t* dt = (uint8_t*)data;
        return write(dt, len);
    }
    

    /*
     * Stack up bytes from a string to be sent when transmitting
     */
    uint8_t write(char*);

    /*
     * Return the amount of bytes that is currently in the receiving buffer
     */
    uint8_t available();

    /*
     * Return the value of byte in the receiving buffer that is currently being
     * pointed to
     */
    int read();

    void setClock(int clockFrequency);

    /*
     * Deinitialize the Wire interface.
     * Do nothing yet. Added for compatibility.
     * 
     */
    void end() {}

};

extern TwoWire Wire;
#endif // _WIRE_H_
