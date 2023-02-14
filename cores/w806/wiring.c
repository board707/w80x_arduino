/*
  wiring.c - Partial implementation of the Wiring API for the ATmega8.
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

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

#include "./include/driver/wm_hal.h"

/*
unsigned long millis()
{
    return 1;
}
*/
uint32_t HAL_Get_Micros();

/* Delay for the given number of microseconds.  Assumes a 1, 8, 12, 16, 20 or 24 MHz clock. */
void delayMicroseconds(uint32_t us)
{
    uint32_t t0 = HAL_Get_Micros();
    while(HAL_Get_Micros() - t0 < us);
}

void init()
{
    HAL_Init();
    __HAL_RCC_GPIO_CLK_ENABLE();
    __HAL_RCC_ADC_CLK_ENABLE();
    __HAL_RCC_PWM_CLK_ENABLE();
}

