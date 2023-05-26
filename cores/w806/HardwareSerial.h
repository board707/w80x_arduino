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

#include <Common.h>
#include "debug.h"
#include "Stream.h"
#include "wm_fifo.h"

#define UART_COUNT 6

// UART RX interrupt settings
#define IT_LEN 0 // 0 or greater,  0: the interrupt callback can be triggered after receiving variable length data;
                 // greater than 0: the interrupt callback can be triggered only after receiving N length data
#define _UART_RX_BUF_SIZE 128

#if USE_IRQ_UART_TX
#define _UART_TX_BUF_SIZE 128
#endif

#define SERIAL_PRINTF_BUFFER_SIZE 64 // Automatically expands on longer output

#define REMAP_TX_RX 1

// Define config for Serial.begin(baud, config);
// Note. w80x doesn't support as many different Serial modes as AVR or SAM cores.

#define SERIAL_8N1 (UART_STOPBITS_1 | UART_PARITY_NONE)
#define SERIAL_8N2 (UART_STOPBITS_2 | UART_PARITY_NONE)

#define SERIAL_8E1 (UART_STOPBITS_1 | UART_PARITY_EVEN)
#define SERIAL_8E2 (UART_STOPBITS_2 | UART_PARITY_EVEN)

#define SERIAL_8O1 (UART_STOPBITS_1 | UART_PARITY_ODD)
#define SERIAL_8O2 (UART_STOPBITS_2 | UART_PARITY_ODD)

#ifdef __cplusplus
extern "C"
{
#endif
#include "./include/driver/wm_uart.h"
    void HAL_UART_MspInit(UART_HandleTypeDef *huart);
    // void UART1_Init(int baud);
    void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
    void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
    void uart_pins_init(UART_HandleTypeDef *huart);

    class HardwareSerial : public Stream
    {
    public:
        using Stream::write;
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

        HardwareSerial(uint8_t serial_no);

        /**
         * @brief       This constructor is used to init hardware serial.
         * @param[in] serial_no Specify serial_no
         * @param[in] mul_flag  Flag to use alternative RX TX pins
         *
         *
         * @return 		None
         *
         * @note
         */
        HardwareSerial(uint8_t serial_no, bool mul_flag);

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
        virtual int read(void); // from Stream

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
        virtual int available(void); // from Stream

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
        virtual int peek(); // from Stream

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
        virtual size_t write(uint8_t c) override; // from Print
        virtual size_t write(const uint8_t *buffer, size_t size) override;

        /**
         * @brief         Prints formatted output
         * @param[in]     Format string and variables
         *
         * @return 		  number of characters printed or -1 in fail
         *
         * @note
         */
        int printf(const char *fmt, ...);

        /**
         * @brief         Receiver IRQ callback function to copy
         *                UART FIFO data to the receiver buffer
         * @param[in]
         *
         * @return 		  nothing
         *
         * @note
         */
        void process_rx(uint8_t *data, int size);
#if USE_IRQ_UART_TX
        void process_tx();
#endif

        bool _uart_mul;

    private:
        void uart_init(unsigned long baud, int uart_mode);

        const uint8_t uart_num;
        UART_HandleTypeDef *huart_handle;

        uint8_t _pbegin = 0;
        uint8_t _pend = 0;
        unsigned char _pbuf[_UART_RX_BUF_SIZE] = {0};
        uint8_t _hal_buf[32] = {0}; // must be greater than or equal to 32 bytes

#if USE_IRQ_UART_TX
        _fifo_str tx_fifo;
        uint8_t _tx_buf[_UART_TX_BUF_SIZE] = {0};
        uint8_t _hal_tx_buf[32] = {0}; // must be lower than or equal to 32 bytes
#endif
        
    };

    extern HardwareSerial Serial;
    // extern HardwareSerial Serial1;

#ifdef __cplusplus
}
extern UART_HandleTypeDef huart0;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;

extern UART_HandleTypeDef *uart_devices[UART_COUNT];
#endif
#endif
