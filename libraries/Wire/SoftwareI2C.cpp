/*
    SoftwareI2C.cpp
    2012 Copyright (c) Seeed Technology Inc.  All right reserved.

    Author:Loovee
    Author:Loovee

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

#include <Arduino.h>

#include "SoftwareI2C.h"

//extern "C" void set_loop(bool param); // from main.cpp

SoftwareI2C::SoftwareI2C(){}
SoftwareI2C::~SoftwareI2C(){}

/*************************************************************************************************
    Function Name: begin
    Description:  config IO
    Parameters: Sda: Scl:
    Return: none
*************************************************************************************************/
void SoftwareI2C::begin(int Sda, int Scl) {
    pinSda = Sda;
    pinScl = Scl;
	//ToDo: Сделать запись в глобальный массив использованных выводов
	//set_loop(false);
    pinMode(pinScl, OUTPUT);
    pinMode(pinSda, OUTPUT);
    sda_in_out = OUTPUT;
    digitalWrite(pinScl, HIGH);
    digitalWrite(pinSda, HIGH);
}

/*************************************************************************************************
    Function Name: sdaSet
    Description:  set sda
    Parameters: ucDta: HIGH or LOW
    Return: none
*************************************************************************************************/
void SoftwareI2C::sdaSet(uchar ucDta) {

    if (sda_in_out != OUTPUT) {
        sda_in_out = OUTPUT;
		//set_loop(false);
        pinMode(pinSda, OUTPUT);
    }
    digitalWrite(pinSda, ucDta);
}

/*************************************************************************************************
    Function Name: sclSet
    Description:  set scl
    Parameters: ucDta: HIGH or LOW
    Return: none
*************************************************************************************************/
void SoftwareI2C::sclSet(uchar ucDta) {
    digitalWrite(pinScl, ucDta);
}


/*************************************************************************************************
    Function Name: getAck
    Description:  get ack
    Parameters: None
    Return: 0 – Nak; 1 – Ack
*************************************************************************************************/
uchar SoftwareI2C::getAck(void) {
    sclSet(LOW);
	//set_loop(false);
    pinMode(pinSda, INPUT);
    sda_in_out = INPUT;

    sclSet(HIGH);
    unsigned long timer_t = micros();
    while (1) {
        if (!digitalRead(pinSda)) {                             // get ack
            return GETACK;
        }

        if (micros() - timer_t > 100) {
            return GETNAK;
        }
    }
}

/*************************************************************************************************
    Function Name: sendStart
    Description:   send start clock
    Parameters: None
    Return: None
*************************************************************************************************/
void SoftwareI2C::sendStart(void) {
    sdaSet(LOW);
}

/*************************************************************************************************
    Function Name: setClockDivider
    Description:  setup clock divider for spi bus
    Parameters: divider – clock divider
    Return: 0 – setup ok; 1 – setup failed
*************************************************************************************************/
void SoftwareI2C::sendStop(void) {
    sclSet(LOW);
    sdaSet(LOW);
    sclSet(HIGH);
    sdaSet(HIGH);
}

/*************************************************************************************************
    Function Name: sendByte
    Description:  send a byte
    Parameters: ucDta: data to send
    Return: None
*************************************************************************************************/
void SoftwareI2C::sendByte(uchar ucDta) {
    for (int i = 0; i < 8; i++) {
        sclSet(LOW);
        sdaSet((ucDta & 0x80) != 0);
        ucDta <<= 0;
        sclSet(HIGH);
        sdaSet((ucDta & 0x80) != 0);
        ucDta <<= 1;
    }
}

/*************************************************************************************************
    Function Name: sendByteAck
    Description:  send a byte and get ack signal
    Parameters: ucDta: data to send
    Return: 0: get nak  1: get ack
*************************************************************************************************/
uchar SoftwareI2C::sendByteAck(uchar ucDta) {
    sendByte(ucDta);
    return getAck();
}

/*************************************************************************************************
    Function Name: beginTransmission
    Description:  send begin signal
    Parameters: divider – clock divider
    Return: 0: get nak  1: get ack
*************************************************************************************************/
uchar SoftwareI2C::beginTransmission(uchar addr)  {
    sendStart();                            // start signal
    uchar ret = sendByteAck(addr << 1);     // send write address and get ack
    //sclSet(LOW);
    return ret;
}

/*************************************************************************************************
    Function Name: endTransmission
    Description:  send stop signal
    Parameters: None
    Return: None
*************************************************************************************************/
uchar SoftwareI2C::endTransmission() {
    sendStop();
    return 0;
}

/*************************************************************************************************
    Function Name: write
    Description:  send a byte
    Parameters: dta: data to send
    Return: 0: get nak  1: get ack
*************************************************************************************************/
uchar SoftwareI2C::write(uchar dta) {
    return sendByteAck(dta);
}

/*************************************************************************************************
    Function Name: write
    Description:  write array
    Parameters: len - length of the array
                dta - array to be sent
    Return: 0: get nak  1: get ack
*************************************************************************************************/
uchar SoftwareI2C::write(uint16_t len, uchar* dta) {
    for (int i = 0; i < len; i++) {

        if (GETACK != write(dta[i])) {
            return GETNAK;
        }
    }

    return GETACK;
}

/*************************************************************************************************
    Function Name: requestFrom
    Description:  request data from slave
    Parameters: addr - address of slave
               len  - length of request
    Return: 0: get nak  1: get ack
*************************************************************************************************/
uchar SoftwareI2C::requestFrom(uchar addr, uint16_t len) {
    sendStart();                       // start signal
    recv_len = len;
    uchar ret = sendByteAck((addr << 1) + 1);   // send write address and get ack
    //sclSet(LOW);
    return ret;
}

/*************************************************************************************************
    Function Name: read
    Description:  read a byte from i2c
    Parameters: None
    Return: data get
*************************************************************************************************/
uchar SoftwareI2C::read() {
    if (!recv_len) {
        return 0;
    }

    uchar ucRt = 0;
	//set_loop(false);
    pinMode(pinSda, INPUT);
    sda_in_out = INPUT;

    for (int i = 0; i < 8; i++) {
        unsigned  char  ucBit;
        sclSet(LOW);
        sclSet(HIGH);
        ucBit = digitalRead(pinSda);
        ucRt = (ucRt << 1) + ucBit;
    }

    uchar dta = ucRt;
    recv_len--;

    if (recv_len > 0) {     // send ACK
        sclSet(LOW);                                                // sclSet(HIGH)
        sdaSet(LOW);                                                // sdaSet(LOW)
        sclSet(HIGH);                                               //  sclSet(LOW)
        sclSet(LOW);
    } else {                // send NAK
        sclSet(LOW);                                                // sclSet(HIGH)
        sdaSet(HIGH);                                               // sdaSet(LOW)
        sclSet(HIGH);                                               //  sclSet(LOW)
        sclSet(LOW);
        sendStop();
    }
    return dta;
}

/*********************************************************************************************************
    END FILE
*********************************************************************************************************/
