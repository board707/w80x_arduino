/*
    SoftwareI2C.h
    2012 Copyright (c) Seeed Technology Inc.  All right reserved.

    Author:Loovee
    2013-11-1

    This is a Software I2C Library, can act as I2c master mode.


    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _SOFTWAREI2C_H_
#define _SOFTWAREI2C_H_

#include "Arduino.h"
#include "Wire_base.h"

/*
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
*/

class SoftwareI2C : public WireBase{
  private:

    int pinSda;
    int pinScl;
    int recv_len;
    int sda_in_out;

  private:

    inline void sdaSet(uchar ucDta);
    inline void sclSet(uchar ucDta);

    inline void sendStart(void);
    inline void sendStop(void);
    inline uchar getAck(void);
    inline void sendByte(uchar ucDta);
    inline uchar sendByteAck(uchar ucDta);                                 // send byte and get ack

  public:

    SoftwareI2C();
    ~SoftwareI2C();

    void begin(int Sda, int Scl);
    uchar beginTransmission(uchar addr) override;
    uchar endTransmission() override;

    uchar write(uchar dta) override;
    uchar write(uint16_t len, uchar* dta) override;
    uchar requestFrom(uchar addr, uint16_t len) override;
    uchar read() override;
    uchar available() {return recv_len;};
    void setClock(int clockFrequency) override {};
	/* NOT IMPLEMENTED YET */
	//setClock()
	//onReceive()
	//onRequest()
	//setWireTimeout()
	//clearWireTimeoutFlag()
	//getWireTimeoutFlag()
};

#endif
/*********************************************************************************************************
    END FILE
*********************************************************************************************************/
