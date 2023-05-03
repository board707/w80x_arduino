/**
 * @file HardwareSerial.cpp
 *
 * @brief   HardwareSerial Module
 *
 * @author Huang Leilei
 *
 * Copyright (c) 2019 Winner Microelectronics Co., Ltd.
 */

//#include <stdio.h>

//#include "pins_arduino.h"
#include "HardwareSerial.h"


extern "C" {
#include "wm_uart.h"

}
/*
HardwareSerial Serial(0);
HardwareSerial Serial1(1, false);
HardwareSerial SerialM1(1, true);

unsigned char _serial_buf[TLS_UART_RX_BUF_SIZE] = {0};
unsigned char _serial1_buf[TLS_UART_RX_BUF_SIZE] = {0};
int _s_buf_begin = 0;
int _s_buf_end = 0;
int _s1_buf_begin = 0;
int _s1_buf_end = 0;

#define TEST_DEBUG  0

extern "C" {
extern void tls_uart_tx_callback_register(uint16_t uart_no,
int16_t (*tx_callback) (struct tls_uart_port * port));
extern int16_t uart_tx_sent_callback(struct tls_uart_port *port);
extern struct tls_uart *tls_uart_open(uint32_t uart_no, TLS_UART_MODE_T uart_mode);
}
*/
extern "C" int sendchar1(int ch);
extern "C" void uart1_serial_init(int bandrate);
/*
extern "C" signed short uart_rx_cb(uint16_t uart_no, unsigned short len,
    unsigned char * pbuf, int *pend)
{
    int ret = 0;
    int _len = 0;
    do
    {
        ret = tls_uart_read(uart_no, pbuf + *pend, 1);
        if (ret > 0)
            (*pend) = *pend + ret;
        _len += ret;
    } while (ret != 0);
}

extern "C" signed short uart1_rx_cb(unsigned short len)
{
    return uart_rx_cb(TLS_UART_1, len, _serial1_buf, &_s1_buf_end);
}

extern "C" signed short uart0_rx_cb(unsigned short len)
{
    return uart_rx_cb(TLS_UART_0, len, _serial_buf, &_s_buf_end);
}

int _read_byte(unsigned char *buf, int *begin, int *end)
{
    int c = 0;
    if (*begin < TLS_UART_RX_BUF_SIZE
        && *begin < *end)
    {
        c = (int)(buf[*begin]);
    }
    return c;
}
*/
/**
 * @brief       This constructor is used to init hardware serial.
 * @param[in] serial_no Specify serial_no
 *
 * @return      None 
 * 
 * @note 
 */ 
HardwareSerial::HardwareSerial(int serial_no)
{
    HardwareSerial(serial_no, false);
}

/**
 * @brief       This constructor is used to init hardware serial.
 * @param[in] serial_no Specify serial_no
 * @param[in] mul_flag Specify mul_flag
 *
 *
 * @return      None 
 * 
 * @note 
 */ 
HardwareSerial::HardwareSerial(int serial_no, bool mul_flag)
{
    _uart_no = serial_no;
    _uart1_mul = mul_flag;
    /*
#if USE_SEM
    tls_os_sem_create(&_psem, _uart_no);
#endif

    if (TLS_UART_0 == _uart_no)
    {
        _pbegin = &_s_buf_begin;
        _pend = &_s_buf_end;
        _pbuf = _serial_buf;
    } else if (TLS_UART_1 == _uart_no) {
        _pbegin = &_s1_buf_begin;
        _pend = &_s1_buf_end;
        _pbuf = _serial1_buf;
    }*/
}

//unsigned int init_uart1_pin_cfg = 0;
//#define INIT_UART1_DEF  (1 << 0)
//#define INIT_UART1_MUL  (1 << 1)

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
 * @return      nothing 
 * 
 * @note 
 */ 

/*int init_uart1_cfg(int uart_no, bool uart1_mul)
{
    unsigned int reg = 0;
    
    
    if (TLS_UART_1 == uart_no 
        && true == uart1_mul
        && ~(INIT_UART1_MUL & init_uart1_pin_cfg))
    {
        delay(10);  // SHOULD delay 0.010 second.
        reg = tls_reg_read32(HR_GPIOB_AFSEL);
        reg &= ~(BIT(11) | BIT(12));
        tls_reg_write32(HR_GPIOB_AFSEL, reg);
        wm_uart1_rx_config(TLS_UART1_MUL_RX);
        wm_uart1_tx_config(TLS_UART1_MUL_TX);
    } else if (TLS_UART_1 == uart_no
        && false == uart1_mul
        && ~(INIT_UART1_DEF & init_uart1_pin_cfg))
    {
        delay(10);  // SHOULD delay 0.010 second.
        reg = tls_reg_read32(HR_GPIOB_AFSEL);
        reg &= ~(BIT(17) | BIT(18));
        tls_reg_write32(HR_GPIOB_AFSEL, reg);
        wm_uart1_rx_config(WM_IO_PB_11);
        wm_uart1_tx_config(WM_IO_PB_12);
        init_uart1_pin_cfg = INIT_UART1_DEF;
    }
}
*/
void HardwareSerial::begin(unsigned long baud, int modeChoose)
{

    if (_uart_no == 1)  {
      uart1_serial_init(baud);
    }
/*#if USE_SEM
    if (TLS_UART_0 == _uart_no)
        tls_os_sem_create(&_psem, 1);
    else if (TLS_UART_1 == _uart_no && NULL != _psem)
        tls_os_sem_create(&_psem, 1);
#endif
    init_uart1_cfg(_uart_no, _uart1_mul);

    tls_uart_options_t opt;
    opt.charlength = TLS_UART_CHSIZE_8BIT;
    opt.flow_ctrl = TLS_UART_FLOW_CTRL_NONE;
    opt.paritytype = TLS_UART_PMODE_DISABLED;
    opt.stopbits = TLS_UART_ONE_STOPBITS;
    opt.baudrate = baud;
    
    tls_uart_port_init(_uart_no, &opt, modeChoose);
    if (TLS_UART_1 == _uart_no)
    {
        tls_uart_tx_callback_register(TLS_UART_1, uart_tx_sent_callback);
        tls_uart_rx_callback_register(TLS_UART_1, uart1_rx_cb);
        tls_uart_open(TLS_UART_1, TLS_UART_MODE_INT);
    }
    else if (TLS_UART_0 == _uart_no)
    {
        tls_uart_tx_callback_register(TLS_UART_0, uart_tx_sent_callback);
        tls_uart_rx_callback_register(TLS_UART_0, uart0_rx_cb);
        tls_uart_open(TLS_UART_0, TLS_UART_MODE_INT);
    }*/
}

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
 * @return      nothing 
 * 
 * @note 
 */ 
void HardwareSerial::begin()
{
    begin(115200);
}

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
 * @return      nothing 
 * 
 * @note 
 */ 
void HardwareSerial::begin(unsigned long baud)
{
    begin(baud, 0);
}

/**
 * @brief       Reads incoming serial data. read() inherits
 *              from the Stream utility class.
 * @param[in] None
 *
 * @return      the first byte of incoming serial data 
 *              available (or -1 if no data is available) - int 
 * 
 * @note 
 */ 
int HardwareSerial::read(void)
{
    int len = 0;
    int c = 0;
/*
    c = _read_byte(_pbuf, _pbegin, _pend);
    if (0 != c)
    {
        (*_pbegin) = (*_pbegin) + 1;
    }*/
    return c;
}

/**
 * @brief         Returns the next byte (character) of incoming serial
 *                data without removing it from the internal serial buffer.
 *                That is, successive calls to peek() will return the same
 *                character, as will the next call to read(). peek() 
 *                inherits from the Stream utility class.
 * @param[in] None
 * @return      the first byte of incoming serial data available (or -1 if
 *              no data is available) - int
 * 
 * @note 
 */ 
int HardwareSerial::peek()
{
    int len = 0;
    int c = 0;

   // c = _read_byte(_pbuf, _pbegin, _pend);
    
    return c;
}

/**
 * @brief       Writes binary data to the serial port. 
 *              This data is sent as a byte or series 
 *              of bytes; to send the characters representing
 *              the digits of a number use the print() function instead.
 * @param[in] c Specify the byte which will be sent to the console.
 * @return      The length of sending to the console.
 * 
 * @note 
 */ 
size_t HardwareSerial::write(uint8_t c)
{
    int ret = 0;
    AR_DBG();
    unsigned int reg = 0;
    
    sendchar1(c);
    /*init_uart1_cfg(_uart_no, _uart1_mul);
    
    ret = tls_uart_write(_uart_no, (char *)&c, 1);*/
    return 1;
}

/**
 * @brief       Get the number of bytes (characters) available
 *              for reading from the serial port. This is data
 *              that's already arrived and stored in the serial
 *              receive buffer (which holds 64 bytes). available()
 *              inherits from the Stream utility class.
 * @param[in] none
 *
 * @return      the number of bytes available to read 
 * 
 * @note 
 */ 
int HardwareSerial::available(void)
{
   /* if (*_pend >= *_pbegin)
        return (*_pend - *_pbegin);
    return TLS_UART_RX_BUF_SIZE - (*_pbegin - *_pend) - 1;*/
    return 0;
}
