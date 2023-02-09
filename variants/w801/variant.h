#ifndef _VARIANT_ARDUINO_W801_
#define _VARIANT_ARDUINO_W801_
#include "pins_arduino.h"
#include "./include/driver/wm_hal.h"
#define PINS_COUNT          (42U)           /*number of pins on hilink demo board*/
#define ADC_COUNT           (4U)            /*number of adc pins*/
#define PWM_COUNT           (5U)            /*number of pwm pins*/


/**
 * Pin Attributes to be OR-ed
 */
#define PIN_ATTR_NONE          (0UL<<0)
#define PIN_ATTR_COMBO         (1UL<<0)
#define PIN_ATTR_ANALOG        (1UL<<1)
#define PIN_ATTR_DIGITAL       (1UL<<2)
#define PIN_ATTR_PWM           (1UL<<3)
#define PIN_ATTR_TIMER         (1UL<<4)
#define PIN_ATTR_TIMER_ALT     (1UL<<5)
#define PIN_ATTR_EXTINT        (1UL<<6)


typedef enum _EAnalogChannel
{
    No_ADC_Channel  =   -1,
    //ADC_Channel0    =   0,
    ADC_Channel1    =   ADC_ANA_CR_CH_0,
    ADC_Channel2    =   ADC_ANA_CR_CH_1,
    ADC_Channel3    =   ADC_ANA_CR_CH_2,
    ADC_Channel4    =   ADC_ANA_CR_CH_3,
}EAnalogChannel;

// Definitions for PWM channels
typedef enum _EPWMChannel
{
    NOT_ON_PWM      =   -1,
    PWM_CH0        =   0,
    PWM_CH1        =   1,
    PWM_CH2        =   2,
    PWM_CH3        =   3,
    PWM_CH4        =   4,
} EPWMChannel ;

/* Types used for the tables below */
typedef struct _PinDescription
{
  GPIO_TypeDef *  pPort ;
  uint32_t  ulPin ;                                  /*pin number*/
  //uint8_t ulPinType ;  
  //uint32_t ulPeripheralId ;
  //uint32_t ulPinConfiguration ;                     
  uint32_t ulPinAttribute ;                         
  EAnalogChannel ulAnalogChannel ;          /* Analog pin in the Arduino context (label on the board) */
  EPWMChannel ulPWMChannel ;              /* PWM pin in the Arduino context (label on the board) */
  //ETCChannel ulTCChannel ;
} PinDescription ;


extern uint8_t g_pinStatus[];
/* Pins table to be instanciated into variant.cpp */
extern const PinDescription g_APinDescription[] ;

#endif /* _VARIANT_ARDUINO_W801_ */