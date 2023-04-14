#ifndef _HARDWAREI2C_H_
#define _HARDWAREI2C_H_

#include "Arduino.h"
#include "Wire_base.h"


/*#define  GETACK        1                      // get ack                        
#define  GETNAK        0                      // get nak   


#ifndef uchar
    #define uchar unsigned char
#endif*/


class HardwareI2C : public WireBase {
  private:

	I2C_HandleTypeDef hi2c;
    unsigned int recv_len;
  private:
  
  public:
    //~HardwareI2C();
    void begin(); // Инициализация контроллера i2c. Используются выводы PA1-SCL и PA4-SDA. Альтернативные выводы заняты под системные нужды (UART0)
	  virtual void setClock(int clockFrequency) override;	// Установка тактовой частоты в герцах. Возможные значения от 100кГц до 1000кГц
    virtual uchar beginTransmission(uchar addr) override;
    virtual uchar endTransmission() override;
    virtual uchar write(uchar dta) override;
    virtual uchar write(uint16_t len, uchar* dta) override;
    virtual uchar requestFrom(uchar addr, uint16_t len) override;
    virtual uchar read() override;
    uchar available() {return recv_len;}
	/* NOT IMPLEMENTED YET */
	//onReceive()
	//onRequest()
	//setWireTimeout()
	//clearWireTimeoutFlag()
	//getWireTimeoutFlag()
	
};

#endif
