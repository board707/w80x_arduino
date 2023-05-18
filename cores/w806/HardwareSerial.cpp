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

UART_HandleTypeDef huart0;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart4;
UART_HandleTypeDef huart5;

static HardwareSerial* serial_ptr[UART_COUNT] = {NULL};
UART_HandleTypeDef* uart_devices[UART_COUNT] = {&huart0, &huart1, &huart2, &huart3, &huart4, &huart5};

HardwareSerial Serial(0);
//extern HardwareSerial Serial1;

extern "C" {

int wm_printf(const char *fmt,...);
int wm_vprintf(const char *fmt, va_list arg_ptr);
int wm_vsnprintf(char* buffer, size_t count, const char* format, va_list va);
}



int _read_byte(unsigned char *buf, int _begin, int _end)
{
    int c = -1;
    if ((_begin < _UART_RX_BUF_SIZE)  && (_begin < _end))
    {
        c = (uint8_t)(buf[_begin]);
    }
    //printf("Buf start= %d, end = %d, char code %d\n", _begin, _end, c);
    return c;

}

/**
 * @brief       This constructor is used to init hardware serial.
 * @param[in] serial_no Specify serial_no
 *
 * @return      None 
 * 
 * @note 
 */ 
HardwareSerial::HardwareSerial(uint8_t serial_no):HardwareSerial(serial_no, false)
{
    //HardwareSerial(serial_no, false);
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

HardwareSerial::HardwareSerial(uint8_t serial_no, bool mul_flag):uart_num(serial_no), _uart_mul(mul_flag)
{
#if USE_UART0_PRINT
    //wm_printf("Serial created for UART%d\n", this->uart_num) ;
    //wm_printf("Internal UART number %d\n", this->uart_num) ;
#endif
  
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
 * @param[in] uart_mode Specify the mode.
 *
 * @return      nothing 
 * 
 * @note 
 */ 


void HardwareSerial::begin(unsigned long baud, int uart_mode)
{

 if (uart_num < UART_COUNT) {

#if USE_UART0_PRINT
   // wm_printf("Method begin() called for UART%d\n", uart_num) ;
#endif
    
      serial_ptr[uart_num] = this;
   
#if USE_UART0_PRINT
  //  wm_printf("Start UART config with baud = %d...", baud) ;
#endif    
      
      uart_init(baud, uart_mode);
      HAL_UART_Receive_IT(this->huart_handle, _hal_buf, IT_LEN); 
     }
   

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
    begin(baud, SERIAL_8N1);
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
    

    int c = _read_byte(_pbuf, _pbegin, _pend);
    if (c >= 0)
    {
        _pbegin++;
    }
    //else c =0;
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
    
    int c = _read_byte(_pbuf, _pbegin, _pend);
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
    
    HAL_UART_Transmit(this->huart_handle, &c, 1, 1000);
    return 1;
}

size_t HardwareSerial::write(const uint8_t *buffer, size_t size)
{
    uint8_t* data_ptr = (uint8_t*) buffer;
    //int result = HAL_UART_Transmit_IT(this->huart_handle, data_ptr, size/*, size*100 */);
    HAL_UART_Transmit(this->huart_handle, data_ptr, size, size*100 );
    return size;
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
    if (_pend >= _pbegin)  return (_pend - _pbegin);
    return 0;
}


int HardwareSerial::printf(const char *fmt,...) {
   
   va_list args;
   int len;
   va_start( args, fmt );
#if USE_UART0_PRINT
   if (this->uart_num == 0 ) {
     len = wm_vprintf(fmt, args);
     }
    else {
#endif
     char * buf2;
     char buf[SERIAL_PRINTF_BUFFER_SIZE];

     len = wm_vsnprintf(buf,SERIAL_PRINTF_BUFFER_SIZE,fmt, args) + 1;
     
     if (len > SERIAL_PRINTF_BUFFER_SIZE) {
        
        buf2 = (char *) malloc( len * sizeof(char) );
        if ( NULL != buf2 )
         {
         len = wm_vsnprintf(buf2,len,fmt, args) + 1;
      
         this->write((uint8_t*)buf2, len);
         free( buf2 );
         va_end( args );
         return len;
         }

         else len = SERIAL_PRINTF_BUFFER_SIZE;
       }
       this->write((uint8_t*)buf, len);
#if USE_UART0_PRINT      
     }
#endif     


 va_end( args );
 return len;
}



void HardwareSerial::process_rx(uint8_t* data, int size)
{
     if (this-> _pend == this-> _pbegin) {this-> _pend =0; this-> _pbegin =0;}
     if ((_UART_RX_BUF_SIZE - this-> _pend) >= size)
        {
            memcpy((this-> _pbuf + this-> _pend), data, size);
            this-> _pend += size;
        }

}

void HardwareSerial::uart_init(unsigned long baud, int uart_mode)
{
    this->huart_handle = uart_devices[this->uart_num];
    
    switch (this->uart_num) {
        case 0: this->huart_handle->Instance = UART0; break;
        case 1:this-> huart_handle->Instance = UART1; break;
        case 2:this-> huart_handle->Instance = UART2; break;
        case 3:this-> huart_handle->Instance = UART3; break;
        case 4:this-> huart_handle->Instance = UART4; break;
        case 5:this-> huart_handle->Instance = UART5; break;
    }
   
   this-> huart_handle->Init.BaudRate = baud;
   this-> huart_handle->Init.WordLength = UART_WORDLENGTH_8B;
   this-> huart_handle->Init.StopBits = uart_mode & UART_LC_STOP_Msk;
   this-> huart_handle->Init.Parity = uart_mode & ( ~UART_LC_STOP_Msk) ;
   this-> huart_handle->Init.Mode = UART_MODE_TX | UART_MODE_RX;
   this-> huart_handle->Init.HwFlowCtl = UART_HWCONTROL_NONE;

    if (this->huart_handle->gState != HAL_UART_STATE_RESET)
    {
        HAL_UART_DeInit(this->huart_handle);
    }

    

#if USE_UART0_PRINT
   int result = HAL_UART_Init(this->huart_handle);
   if ( result != HAL_OK)
    {
       wm_printf("Error initialize UART%d, error code = %d\n", this->uart_num, result);
    }
    //else wm_printf("UART%d started\n", this->uart_num);
#else
    HAL_UART_Init(this->huart_handle);
#endif  
}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
    __HAL_RCC_GPIO_CLK_ENABLE();

    if (huart->Instance == UART0)
	{
		__HAL_RCC_UART0_CLK_ENABLE();
		
	//	PB19: UART0_TX
	//	PB20: UART0_RX
		__HAL_AFIO_REMAP_UART0_TX(GPIOB, GPIO_PIN_19);
		__HAL_AFIO_REMAP_UART0_RX(GPIOB, GPIO_PIN_20);
		HAL_NVIC_SetPriority(UART0_IRQn, 0);
		HAL_NVIC_EnableIRQ(UART0_IRQn);
	}

	else if (huart->Instance == UART1)
	{
		__HAL_RCC_UART1_CLK_ENABLE();
		
	//	PB6: UART1_TX
	//	PB7: UART1_RX
		__HAL_AFIO_REMAP_UART1_TX(GPIOB, GPIO_PIN_6);
		__HAL_AFIO_REMAP_UART1_RX(GPIOB, GPIO_PIN_7);
		HAL_NVIC_SetPriority(UART1_IRQn, 0);
		HAL_NVIC_EnableIRQ(UART1_IRQn);
	}

    else {
     if (huart->Instance == UART2)
	{
		__HAL_RCC_UART2_CLK_ENABLE();
		
	//	PB2: UART2_TX
	//	PB3: UART2_RX
		__HAL_AFIO_REMAP_UART2_TX(GPIOB, GPIO_PIN_2);
		__HAL_AFIO_REMAP_UART2_RX(GPIOB, GPIO_PIN_3);
	}
     
     else if (huart->Instance == UART3)
	{
		__HAL_RCC_UART3_CLK_ENABLE();
		
	//	PB0: UART3_TX
	//	PB1: UART3_RX
		__HAL_AFIO_REMAP_UART3_TX(GPIOB, GPIO_PIN_0);
		__HAL_AFIO_REMAP_UART3_RX(GPIOB, GPIO_PIN_1);
	}

     else if (huart->Instance == UART4)
	{
		__HAL_RCC_UART4_CLK_ENABLE();
		
	//	PB4: UART4_TX
	//	PB5: UART4_RX
		__HAL_AFIO_REMAP_UART4_TX(GPIOB, GPIO_PIN_4);
		__HAL_AFIO_REMAP_UART4_RX(GPIOB, GPIO_PIN_5);
	}

    else if (huart->Instance == UART5)
	{
		__HAL_RCC_UART5_CLK_ENABLE();
		
	//	PA12: UART5_TX
	//	PA13: UART5_RX
		__HAL_AFIO_REMAP_UART5_TX(GPIOA, GPIO_PIN_12);
		__HAL_AFIO_REMAP_UART5_RX(GPIOA, GPIO_PIN_13);
	}

        HAL_NVIC_SetPriority(UART2_5_IRQn, 0);
		HAL_NVIC_EnableIRQ(UART2_5_IRQn);
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    int uart_num = -1;
   
    if (huart->Instance == UART0) uart_num = 0;
    if (huart->Instance == UART1) uart_num = 1;
    if (huart->Instance == UART2) uart_num = 2;
    if (huart->Instance == UART3) uart_num = 3;
    if (huart->Instance == UART4) uart_num = 4;
    if (huart->Instance == UART5) uart_num = 5;
	
    if ((uart_num >= 0) && (uart_num < UART_COUNT) && (serial_ptr[uart_num] != NULL))
    {
       serial_ptr[uart_num]->process_rx(huart->pRxBuffPtr, huart->RxXferCount);
       
    }
}