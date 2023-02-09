/*
  pins_arduino.h - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include "variant.h"

#define PIOA ((GPIO_TypeDef *)GPIOA_BASE)
#define PIOB ((GPIO_TypeDef *)GPIOB_BASE)

#define PA0 0
#define PA1 1
#define PA2 2
#define PA3 3
#define PA4 4
#define PA5 5
#define PA6 6
#define PA7 7
#define PA8 8
#define PA9 9
#define PA10 10
#define PA11 11
#define PA12 12
#define PA13 13
#define PA14 14
#define PA15 15

#define PB0 16
#define PB1 17
#define PB2 18
#define PB3 19
#define PB4 20
#define PB5 21
#define PB6 22
#define PB7 23
#define PB8 24
#define PB9 25
#define PB10 26
#define PB11 27
#define PB12 28
#define PB13 29
#define PB14 30
#define PB15 31
#define PB16 32
#define PB17 33
#define PB18 34
#define PB19 35
#define PB20 36
#define PB21 37
#define PB22 38
#define PB23 39
#define PB24 40
#define PB25 41
#define PB26 42

#define A0 PA1
#define A1 PA4
#define A2 PA3
#define A3 PA2
//#define PA##n (n)
//#define PB##n (16+n)

/*
#ifndef _BV
#define _BV(X) (1<<(X))
#endif

__code uint8_t digital_pin_to_pwm_PGM[] = {
    NOT_ON_PWM, //PIN00
    NOT_ON_PWM,
    NOT_ON_PWM,
    NOT_ON_PWM,
    NOT_ON_PWM,
    NOT_ON_PWM,
    NOT_ON_PWM,
    NOT_ON_PWM,
    NOT_ON_PWM,	//not exist
    NOT_ON_PWM, //not exist
	
    NOT_ON_PWM, //PIN10
    NOT_ON_PWM,
    NOT_ON_PWM,
    NOT_ON_PWM,
    NOT_ON_PWM,
    PIN_PWM1,
    NOT_ON_PWM,
    NOT_ON_PWM,
    NOT_ON_PWM,	//not exist
    NOT_ON_PWM, //not exist

    NOT_ON_PWM, //PIN20
    NOT_ON_PWM,
    NOT_ON_PWM,
    NOT_ON_PWM,
    NOT_ON_PWM,
    NOT_ON_PWM,
    NOT_ON_PWM,
    NOT_ON_PWM,
    NOT_ON_PWM,	//not exist
    NOT_ON_PWM, //not exist
	
    PIN_PWM1_, //PIN30
    PIN_PWM2_,
    NOT_ON_PWM,
    NOT_ON_PWM,
    PIN_PWM2,
    NOT_ON_PWM,
    NOT_ON_PWM,
    NOT_ON_PWM,
    NOT_ON_PWM,	//not exist
    NOT_ON_PWM, //not exist
};

__code uint8_t PROGMEM digital_pin_to_port_PGM[] = {
    GPIOA, //PA0
    GPIOA,
    GPIOA,
    GPIOA,
    GPIOA,
    GPIOA,
    GPIOA,
    GPIOA,
    GPIOA,
    GPIOA,
    GPIOA,
    GPIOA,
    GPIOA,
    GPIOA,
    GPIOA,
    GPIOA, //PA15

    GPIOB, //PB0
    GPIOB,
    GPIOB,
    GPIOB,
    GPIOB,
    GPIOB,
    GPIOB,
    GPIOB,
    GPIOB,
    GPIOB,
    GPIOB,
    GPIOB,
    GPIOB,
    GPIOB,
    GPIOB,
    GPIOB, //PB15
    GPIOB, //PB16
    GPIOB,
    GPIOB,
    GPIOB,
    GPIOB,
    GPIOB,
    GPIOB,
    GPIOB,
    GPIOB,
    GPIOB,
    GPIOB, //PB26
    GPIOB, //PB27
};

__code uint8_t digital_pin_to_bit_mask_PGM[] = {
    _BV(0), //PIN00
    _BV(1),
    _BV(2),
    _BV(3),
    _BV(4),
    _BV(5),
    _BV(6),
    _BV(7),
    0,	//not exist
    0, //not exist
	
    _BV(0), //PIN10
    _BV(1),
    _BV(2),
    _BV(3),
    _BV(4),
    _BV(5),
    _BV(6),
    _BV(7),
    0,	//not exist
    0, //not exist
	
    _BV(0), //PIN20
    _BV(1),
    _BV(2),
    _BV(3),
    _BV(4),
    _BV(5),
    _BV(6),
    _BV(7),
    0,	//not exist
    0, //not exist
	
    _BV(0), //PIN30
    _BV(1),
    _BV(2),
    _BV(3),
    _BV(4),
    _BV(5),
    _BV(6),
    _BV(7),
    0,	//not exist
    0, //not exist
};

__code uint8_t digital_pin_to_channel_PGM[] = {
    NOT_ANALOG, //PIN00
    NOT_ANALOG,
    NOT_ANALOG,
    NOT_ANALOG,
    NOT_ANALOG,
    NOT_ANALOG,
    NOT_ANALOG,
    NOT_ANALOG,
    NOT_ANALOG,	//not exist
    NOT_ANALOG, //not exist
	
    NOT_ANALOG, //PIN10
    0,
    NOT_ANALOG,
    NOT_ANALOG,
    1,
    2,
    NOT_ANALOG,
    NOT_ANALOG,
    NOT_ANALOG,	//not exist
    NOT_ANALOG, //not exist
	
    NOT_ANALOG, //PIN20
    NOT_ANALOG,
    NOT_ANALOG,
    NOT_ANALOG,
    NOT_ANALOG,
    NOT_ANALOG,
    NOT_ANALOG,
    NOT_ANALOG,
    NOT_ANALOG,	//not exist
    NOT_ANALOG, //not exist
	
    NOT_ANALOG, //PIN30
    NOT_ANALOG,
    3,
    NOT_ANALOG,
    NOT_ANALOG,
    NOT_ANALOG,
    NOT_ANALOG,
    NOT_ANALOG,
    NOT_ANALOG,	//not exist
    NOT_ANALOG, //not exist
};
*/

#endif
