#ifndef _HARDWAREI2C_H_
#define _HARDWAREI2C_H_

#include "Arduino.h"


#define  GETACK        1                      // get ack                        
#define  GETNAK        0                      // get nak   


#ifndef uchar
    #define uchar unsigned char
#endif


class HardwareI2C {
  private:

	I2C_HandleTypeDef hi2c;
    unsigned int recv_len;
  private:
  
  public:

    void begin(); // Инициализация контроллера i2c. Используются выводы PA1-SCL и PA4-SDA. Альтернативные выводы заняты под системные нужды (UART0)
	void setClock(int clockFrequency);	// Установка тактовой частоты в герцах. Возможные значения от 100кГц до 1000кГц
    uchar beginTransmission(uchar addr);
    uchar endTransmission();
    uchar write(uchar dta);
    uchar write(uint16_t len, uchar* dta);
    uchar requestFrom(uchar addr, uint16_t len);
    uchar read();
    uchar available() {return recv_len;}
	/* NOT IMPLEMENTED YET */
	//onReceive()
	//onRequest()
	//setWireTimeout()
	//clearWireTimeoutFlag()
	//getWireTimeoutFlag()
	
};

#endif
