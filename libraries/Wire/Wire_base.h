
#ifndef _WIREBASE_H_
#define _WIREBASE_H_

#include "Arduino.h"

#define  GETACK        1                      // get ack                        
#define  GETNAK        0                      // get nak   

#ifndef  HIGH
    #define  HIGH          1
#endif
#ifndef  LOW
    #define  LOW           0
#endif

#ifndef uchar
    #define uchar unsigned char
#endif

class WireBase {
    
    
    public:
    
     //void begin(int Sda, int Scl);
    WireBase();
    virtual ~WireBase();
    virtual uchar beginTransmission(uchar addr) {return 0;};
    virtual uchar endTransmission() {return 0;};

    virtual uchar write(uchar dta)  {return 0;};
    virtual uchar write(uint16_t len, uchar* dta) {return 0;};
    virtual uchar requestFrom(uchar addr, uint16_t len) {return 0;};
    virtual uchar read()  {return 0;};
    //uchar available() {return recv_len;}
    virtual void setClock(int clockFrequency) {};// Установка тактовой частоты в герцах. Возможные значения от 100кГц до 1000кГц

};
#endif
