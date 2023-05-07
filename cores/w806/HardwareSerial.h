/**
 * @file HardwareSerial.h 
 *
 * @brief   HardwareSerial Module
 *
 * @author Huang Leilei
 *
 * Copyright (c) 2019 Winner Microelectronics Co., Ltd.
 */
#ifndef _HARDWARESERIAL_H_
#define _HARDWARESERIAL_H_

#include <Arduino.h>
#include "debug.h"
#include "Stream.h"

#define USE_SEM 0
#define IT_LEN 0     // 0 or greater,  0: the interrupt callback can be triggered after receiving variable length data; 
                     // greater than 0: the interrupt callback can be triggered only after receiving N length data
#define _UART_RX_BUF_SIZE 128

#ifdef __cplusplus 
extern "C" {
#endif

void HAL_UART_MspInit(UART_HandleTypeDef* huart);
void UART1_Init(int baud);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

class HardwareSerial : public Stream
{
public:
/**
 * @defgroup Serial Serial
 * @brief Serial
 */

/**
 * @addtogroup Serial
 * @{
 */

/**
 * @defgroup Serial Serial
 * @brief Serial
 */

/**
 * @addtogroup Serial
 * @{
 */

    /**
     * @brief       This constructor is used to init hardware serial.
     * @param[in] serial_no Specify serial_no
     *
     * @return 		None 
     * 
     * @note 
     */ 
    HardwareSerial(int serial_no);
    /**
     * @brief       This constructor is used to init hardware serial.
     * @param[in] serial_no Specify serial_no
     * @param[in] mul_flag Specify mul_flag
     *
     *
     * @return 		None 
     * 
     * @note 
     */ 
    HardwareSerial(int serial_no, bool mul_flag);
    /**
     * @brief       This constructor is used to init hardware serial.
     * @param[in] none
     * @return 		None 
     * 
     * @note 
     */ 
    //HardwareSerial() {}

    /**
     * @brief       Sets the data rate in bits per second (baud)
     *              for serial data transmission. For communicating
     *              with the computer, use one of these rates: 300,
     *              600, 1200, 2400, 4800, 9600, 14400, 19200, 28800,
     *              38400, 57600, or 115200. You can, however, specify
     *              other rates - for example, to communicate over pins
     *              0 and 1 with a component that requires a particular
     *              baud rate.
     *
     *              An optional second argument configures the data,
     *              parity, and stop bits. The default is 8 data bits,
     *              no parity, one stop bit.
     *
     * @param[in] None
     *
     * @return 		nothing 
     * 
     * @note 
     */ 
	void begin();
    /**
     * @brief       Sets the data rate in bits per second (baud)
     *              for serial data transmission. For communicating
     *              with the computer, use one of these rates: 300,
     *              600, 1200, 2400, 4800, 9600, 14400, 19200, 28800,
     *              38400, 57600, or 115200. You can, however, specify
     *              other rates - for example, to communicate over pins
     *              0 and 1 with a component that requires a particular
     *              baud rate.
     *
     *              An optional second argument configures the data,
     *              parity, and stop bits. The default is 8 data bits,
     *              no parity, one stop bit.
     *
     * @param[in] baud speed: in bits per second (baud) - long
     *
     * @return 		nothing 
     * 
     * @note 
     */ 
    void begin(unsigned long baud);
    /**
     * @brief       Sets the data rate in bits per second (baud)
     *              for serial data transmission. For communicating
     *              with the computer, use one of these rates: 300,
     *              600, 1200, 2400, 4800, 9600, 14400, 19200, 28800,
     *              38400, 57600, or 115200. You can, however, specify
     *              other rates - for example, to communicate over pins
     *              0 and 1 with a component that requires a particular
     *              baud rate.
     *
     *              An optional second argument configures the data,
     *              parity, and stop bits. The default is 8 data bits,
     *              no parity, one stop bit.
     *
     * @param[in] baud speed: in bits per second (baud) - long
     * @param[in] modeChoose Specify the mode.
     *
     * @return 		nothing 
     * 
     * @note 
     */ 
    void begin(unsigned long baud, int modeChoose);

    /**
     * @brief       Reads incoming serial data. read() inherits
     *              from the Stream utility class.
     * @param[in] None
     *
     * @return 		the first byte of incoming serial data 
     *              available (or -1 if no data is available) - int 
     * 
     * @note 
     */ 
    virtual int read(void);         // from Stream
    /**
     * @brief       Get the number of bytes (characters) available
     *              for reading from the serial port. This is data
     *              that's already arrived and stored in the serial
     *              receive buffer (which holds 64 bytes). available()
     *              inherits from the Stream utility class.
     * @param[in] none
     *
     * @return 		the number of bytes available to read 
     * 
     * @note 
     */ 
    virtual int available(void);    // from Stream
    /**
     * @brief         Returns the next byte (character) of incoming serial
     *                data without removing it from the internal serial buffer.
     *                That is, successive calls to peek() will return the same
     *                character, as will the next call to read(). peek() 
     *                inherits from the Stream utility class.
     * @param[in] None
     * @return 		the first byte of incoming serial data available (or -1 if
     *              no data is available) - int
     * 
     * @note 
     */ 
    virtual int peek();             // from Stream

/**
 * @}
 */

/**
 * @}
 */
public:
    /**
     * @brief       Writes binary data to the serial port. 
     *              This data is sent as a byte or series 
     *              of bytes; to send the characters representing
     *              the digits of a number use the print() function instead.
     * @param[in] c Specify the byte which will be sent to the console.
     * @return 		The length of sending to the console.
     * 
     * @note 
     */ 
    virtual size_t write(uint8_t c); // from Print
    
private:
    const int uart_num;
    bool _uart_mul;
    
    //uint8_t *_pbuf;
    //uint8_t _pbegin = 0; 
    //uint8_t _pend = 0;
#if USE_SEM
    tls_os_sem_t * _psem;
#endif
};

//extern HardwareSerial Serial;
//extern HardwareSerial Serial1;
//extern HardwareSerial SerialM1;
#ifdef __cplusplus 
}
#endif
#endif
