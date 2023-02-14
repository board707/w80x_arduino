/*
  wiring_analog.c - analog input and output
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

  Modified 28 September 2010 by Mark Sproul

  $Id: wiring.c 248 2007-02-03 15:36:30Z mellis $
*/

#include "pins_arduino.h"
#include "./include/driver/wm_adc.h"
#include "Arduino.h"

#define ADC_NORMAL (0U) /*polling mode*/
#define ADC_IT (1U)     /*interrupt mode*/

#define ADC_MODE ADC_NORMAL /*using polling mode */
#define ADC_DEFAULT_OFFSET 176859
#define ADC_MAX_VAL 270000
int adcResolution = ADC8BIT;

typedef struct
{
    uint8_t ch;
    uint8_t pin;
    //uint8_t ch_state;
    ADC_HandleTypeDef hadc;
} adcPinStateTypeDef;

typedef struct
{
    uint8_t ch;
    uint8_t pin;

    PWM_HandleTypeDef hpwm;
} pwmPinStateTypeDef;

adcPinStateTypeDef adcPinState[ADC_COUNT] = {0};

pwmPinStateTypeDef pwmPinState[PWM_COUNT] = {0};

void Error_Handler(void);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc);

// void analogReference(uint32_t val)
// {
//     uint8_t ch = 0;
//     if (val == 0)
//     {
//         val = ADC_DEFAULT_OFFSET;
//     }

//     for (ch = 0; ch < sizeof(ADC_COUNT); ch++)
//     {
//         adcPinState[ch].hadc.offset = val;
//     }
// }
void analogReadResolution(uint8_t res)
{
    adcResolution = res;
}

int analogRead(uint8_t pin) /*all ADC channel range is 0-2.4V*/
{
    uint32_t val = 0;
    uint8_t channel_index = 0;

    if (g_APinDescription[pin].ulPinAttribute & PIN_ATTR_ANALOG)
    {
        channel_index = (uint16_t)g_APinDescription[pin].ulAnalogChannel >> 8; //ADC_ANA_CR_CH_0

        if (g_pinStatus[pin] != ANALOG_INPUT)
        {
            g_pinStatus[pin] = ANALOG_INPUT;

            adcPinState[channel_index].pin = pin;
            adcPinState[channel_index].hadc.Instance = ADC;
            adcPinState[channel_index].hadc.Init.channel = g_APinDescription[pin].ulAnalogChannel; //channel[ch];
            adcPinState[channel_index].hadc.Init.freq = 1000;
            adcPinState[channel_index].hadc.offset = ADC_DEFAULT_OFFSET;
            HAL_ADC_Init(&(adcPinState[channel_index].hadc));
        }

        assert_param(IS_ADC_ALL_INSTANCE(hadc->Instance));

#if (ADC_MODE == ADC_IT)
        HAL_ADC_Start_IT(&adcPinState[ch].hadc);
#else
        HAL_ADC_Start(&adcPinState[channel_index].hadc);
        HAL_ADC_PollForConversion(&adcPinState[channel_index].hadc);
        HAL_ADC_Stop(&adcPinState[channel_index].hadc);

        val = ADC->DR;
        val = val - ADC_DEFAULT_OFFSET;

        if (adcResolution == ADC8BIT)
        {
            val = (val) ? (val * 256 / 76000) : 0;
        }

#endif
    }

    return val;
}

void disable_adc(uint8_t pin) /*disable pin's ADC channel function*/
{
#if (ADC_MODE == ADC_IT)
     HAL_NVIC_DisableIRQ(ADC_IRQn);
#else

   // uint8_t channel_index = 0;
    ADC_HandleTypeDef hadc;
    if (g_APinDescription[pin].ulPinAttribute & PIN_ATTR_ANALOG)
    {
       // channel_index = (uint16_t)g_APinDescription[pin].ulAnalogChannel >> 8; //ADC_ANA_CR_CH_0
        hadc.Instance = ADC;
        hadc.Init.channel = g_APinDescription[pin].ulAnalogChannel; //channel[ch];
        hadc.Init.freq = 1000;
        hadc.offset = ADC_DEFAULT_OFFSET;
    }

    HAL_ADC_Stop(&hadc);
#endif
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
}

void Error_Handler(void)
{
    while (1)
    {
    }
}

#if (ADC_MODE == ADC_IT)
__attribute__((isr)) void ADC_IRQHandler(void)
{
    uint8_t i = 0;

    for (i = 0; i < 4; i++)
    {
        if (adcPinState[i].hadc != NULL)
        {
            HAL_ADC_IRQHandler(&adcPinState[i].hadc);
        }
    }
}
#endif

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC)
    {
        // __HAL_RCC_GPIO_CLK_ENABLE();

        //ADC_CHANNEL_0 : PA1
        //ADC_CHANNEL_1 : PA4
        //ADC_CHANNEL_2 : PA3
        //ADC_CHANNEL_3 : PA2
        //ADC_CHANNEL_0_1 : PA1 and PA4
        //ADC_CHANNEL_2_3 : PA3 and PA2
        if (hadc->Init.channel == ADC_CHANNEL_0)
        {
            __HAL_AFIO_REMAP_ADC(GPIOA, GPIO_PIN_1);
        }
        else if (hadc->Init.channel == ADC_CHANNEL_1)
        {
            __HAL_AFIO_REMAP_ADC(GPIOA, GPIO_PIN_4);
        }
        else if (hadc->Init.channel == ADC_CHANNEL_2)
        {
            __HAL_AFIO_REMAP_ADC(GPIOA, GPIO_PIN_3);
        }
        else if (hadc->Init.channel == ADC_CHANNEL_3)
        {
            __HAL_AFIO_REMAP_ADC(GPIOA, GPIO_PIN_2);
        }
        else if (hadc->Init.channel == ADC_CHANNEL_0_1)
        {
            __HAL_AFIO_REMAP_ADC(GPIOA, GPIO_PIN_1);
            __HAL_AFIO_REMAP_ADC(GPIOA, GPIO_PIN_4);
        }
        else if (hadc->Init.channel == ADC_CHANNEL_2_3)
        {
            __HAL_AFIO_REMAP_ADC(GPIOA, GPIO_PIN_3);
            __HAL_AFIO_REMAP_ADC(GPIOA, GPIO_PIN_2);
        }
#if (ADC_MODE == ADC_IT)
        // 如果用到中断方式需要使能中断
        HAL_NVIC_SetPriority(ADC_IRQn, 0);
        HAL_NVIC_EnableIRQ(ADC_IRQn);
#endif
    }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
{
    __HAL_RCC_ADC_CLK_DISABLE();

    for (int i = 0; i < ADC_COUNT; i++)
    {
        if (hadc->Init.channel == adcPinState[i].hadc.Init.channel)
        {
            if (g_pinStatus[adcPinState[i].pin] == ANALOG_INPUT)
            {
                g_pinStatus[adcPinState[i].pin] = PIN_IDLE;
                HAL_GPIO_DeInit(GPIOA, adcPinState[i].pin);
                break;
            }
        }
    }
#if (ADC_MODE == ADC_IT)
    HAL_NVIC_DisableIRQ(ADC_IRQn);
#endif
}

void analogWrite(uint32_t pin, uint32_t val)
{
    uint8_t channel_index = 0;
    if (g_APinDescription[pin].ulPinAttribute & PIN_ATTR_PWM) /*this pin has pwm attr*/
    {
        channel_index = (uint16_t)g_APinDescription[pin].ulPWMChannel;
        g_pinStatus[pin] = PWM_OUT;
        pwmPinState[channel_index].pin = pin;
        pwmPinState[channel_index].hpwm.Instance = PWM;
        pwmPinState[channel_index].hpwm.Init.AutoReloadPreload = PWM_AUTORELOAD_PRELOAD_ENABLE;
        pwmPinState[channel_index].hpwm.Init.CounterMode = PWM_COUNTERMODE_EDGEALIGNED_DOWN;
        pwmPinState[channel_index].hpwm.Init.Prescaler = 312;                    //40M / 312 = 12.8K						//鍒嗛 1M
        pwmPinState[channel_index].hpwm.Init.Period = 255;                       // 12.8K / 256 = 500HZ
        pwmPinState[channel_index].hpwm.Init.Pulse = 100;                        // (100/256)% DUTY
        pwmPinState[channel_index].hpwm.Init.OutMode = PWM_OUT_MODE_INDEPENDENT;
        pwmPinState[channel_index].hpwm.Channel = channel_index;
        HAL_PWM_Init(&(pwmPinState[channel_index].hpwm));
        HAL_PWM_Start(&(pwmPinState[channel_index].hpwm));
        HAL_PWM_Duty_Set(&(pwmPinState[channel_index].hpwm), val);
    }
    else /*this pin is not pwm pin*/
    {
        ;
        ;
        ;
    }
}

void disable_pwm(uint8_t pin) /*disable pin's ADC channel function*/
{
    uint8_t channel_index = 0;
    PWM_HandleTypeDef hpwm;
    if (g_APinDescription[pin].ulPinAttribute & PIN_ATTR_PWM) /*this pin has pwm attr*/
    {
       channel_index = (uint16_t)g_APinDescription[pin].ulPWMChannel;

       hpwm.Instance = PWM;
       hpwm.Channel = channel_index;
    }
    HAL_PWM_Stop(&hpwm);
}


void HAL_PWM_MspInit(PWM_HandleTypeDef *hpwm)
{
    for (int i = 0; i < PWM_COUNT; i++)
    {
        if (hpwm->Channel == pwmPinState[i].hpwm.Channel)
        {
            if (g_pinStatus[pwmPinState[i].pin] == PWM_OUT)
            {
                if (hpwm->Channel == PWM_CH0)
                {
                    __HAL_AFIO_REMAP_PWM0(g_APinDescription[pwmPinState[i].pin].pPort, g_APinDescription[pwmPinState[i].pin].ulPin);
                }
                else if (hpwm->Channel == PWM_CH1)
                {
                    __HAL_AFIO_REMAP_PWM1(g_APinDescription[pwmPinState[i].pin].pPort, g_APinDescription[pwmPinState[i].pin].ulPin);
                }
                else if (hpwm->Channel == PWM_CH2)
                {
                    __HAL_AFIO_REMAP_PWM2(g_APinDescription[pwmPinState[i].pin].pPort, g_APinDescription[pwmPinState[i].pin].ulPin);
                }
                else if (hpwm->Channel == PWM_CH3)
                {
                    __HAL_AFIO_REMAP_PWM3(g_APinDescription[pwmPinState[i].pin].pPort, g_APinDescription[pwmPinState[i].pin].ulPin);
                }
                else if (hpwm->Channel == PWM_CH4)
                {
                    __HAL_AFIO_REMAP_PWM4(g_APinDescription[pwmPinState[i].pin].pPort, g_APinDescription[pwmPinState[i].pin].ulPin);
                }
                break;
            }
        }
    }
}

void HAL_PWM_MspDeInit(PWM_HandleTypeDef *hpwm)
{
    for (int i = 0; i < PWM_COUNT; i++)
    {
        if (hpwm->Channel == pwmPinState[i].hpwm.Channel) /*find channel num 0-4*/
        {
            if (g_pinStatus[pwmPinState[i].pin] == PWM_OUT)
            {
                g_pinStatus[pwmPinState[i].pin] = PIN_IDLE;

                __HAL_RCC_PWM_CLK_DISABLE();
                HAL_GPIO_DeInit(g_APinDescription[pwmPinState[i].pin].pPort, g_APinDescription[pwmPinState[i].pin].ulPin);
            }
            break;
        }
    }
}
