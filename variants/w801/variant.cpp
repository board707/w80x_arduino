#include "variant.h"
#include <stdint.h>

uint8_t g_pinStatus[PINS_COUNT] = {0};

/*
  uint8_t  pPort ;
  uint32_t ulPin ;                                  
  uint32_t ulPinType ;                      
  uint32_t ulPinAttribute ;                         
  EAnalogChannel ulAnalogChannel ;          
*/

const PinDescription g_APinDescription[] = {
    { PIOA, GPIO_PIN_0,          PIN_ATTR_DIGITAL|PIN_ATTR_ANALOG|PIN_ATTR_PWM,           No_ADC_Channel,      NOT_ON_PWM},
    { PIOA, GPIO_PIN_1,          PIN_ATTR_DIGITAL|PIN_ATTR_ANALOG|PIN_ATTR_PWM,           ADC_Channel1,    NOT_ON_PWM},
    { PIOA, GPIO_PIN_2,          PIN_ATTR_DIGITAL|PIN_ATTR_ANALOG|PIN_ATTR_PWM,           ADC_Channel4,      NOT_ON_PWM},
    { PIOA, GPIO_PIN_3,          PIN_ATTR_DIGITAL|PIN_ATTR_ANALOG|PIN_ATTR_PWM,           ADC_Channel3,      NOT_ON_PWM},
    { PIOA, GPIO_PIN_4,          PIN_ATTR_DIGITAL|PIN_ATTR_ANALOG|PIN_ATTR_PWM,           ADC_Channel2,      NOT_ON_PWM},
    { PIOA, GPIO_PIN_5,          PIN_ATTR_DIGITAL,                                        No_ADC_Channel,    NOT_ON_PWM},
    { PIOA, GPIO_PIN_6,          PIN_ATTR_DIGITAL,                                        No_ADC_Channel,    NOT_ON_PWM},
    { PIOA, GPIO_PIN_7,          PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel,    PWM_CH4},
    { PIOA, GPIO_PIN_8,          PIN_ATTR_DIGITAL,                                        No_ADC_Channel,    NOT_ON_PWM},
    { PIOA, GPIO_PIN_9,          PIN_ATTR_DIGITAL,                                        No_ADC_Channel,    NOT_ON_PWM},
    { PIOA, GPIO_PIN_10,         PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel,    NOT_ON_PWM},
    { PIOA, GPIO_PIN_11,         PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel,    NOT_ON_PWM},       
    { PIOA, GPIO_PIN_12,         PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel,    NOT_ON_PWM},
    { PIOA, GPIO_PIN_13,         PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel,    NOT_ON_PWM},    
    { PIOA, GPIO_PIN_14,         PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel,    NOT_ON_PWM},
    { PIOA, GPIO_PIN_15,         PIN_ATTR_DIGITAL,                                        No_ADC_Channel,    NOT_ON_PWM},             

    { PIOB, GPIO_PIN_0,          PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel,    PWM_CH0},
    { PIOB, GPIO_PIN_1,          PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel,    PWM_CH1},
    { PIOB, GPIO_PIN_2,          PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel,    PWM_CH2},
    { PIOB, GPIO_PIN_3,          PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel,    PWM_CH3},
    { PIOB, GPIO_PIN_4,          PIN_ATTR_DIGITAL,                                        No_ADC_Channel,    NOT_ON_PWM},
    { PIOB, GPIO_PIN_5,          PIN_ATTR_DIGITAL,                                        No_ADC_Channel,    NOT_ON_PWM},
    { PIOB, GPIO_PIN_6,          PIN_ATTR_DIGITAL,                                        No_ADC_Channel,    NOT_ON_PWM},
    { PIOB, GPIO_PIN_7,          PIN_ATTR_DIGITAL,                                        No_ADC_Channel,    NOT_ON_PWM},
    { PIOB, GPIO_PIN_8,          PIN_ATTR_DIGITAL,                                        No_ADC_Channel,    NOT_ON_PWM},
    { PIOB, GPIO_PIN_9,          PIN_ATTR_DIGITAL,                                        No_ADC_Channel,    NOT_ON_PWM},
    { PIOB, GPIO_PIN_10,         PIN_ATTR_DIGITAL,                                        No_ADC_Channel,    NOT_ON_PWM},
    { PIOB, GPIO_PIN_11,         PIN_ATTR_DIGITAL,                                        No_ADC_Channel,    NOT_ON_PWM},       
    { PIOB, GPIO_PIN_12,         PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel,    NOT_ON_PWM},
    { PIOB, GPIO_PIN_13,         PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel,    NOT_ON_PWM},    
    { PIOB, GPIO_PIN_14,         PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel,    NOT_ON_PWM},
    { PIOB, GPIO_PIN_15,         PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel,    NOT_ON_PWM},             
    { PIOB, GPIO_PIN_16,         PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel,    NOT_ON_PWM},
    { PIOB, GPIO_PIN_17,         PIN_ATTR_DIGITAL,                                        No_ADC_Channel,    NOT_ON_PWM},       
    { PIOB, GPIO_PIN_18,         PIN_ATTR_DIGITAL,                                        No_ADC_Channel,    NOT_ON_PWM},
    { PIOB, GPIO_PIN_19,         PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel,    NOT_ON_PWM},    
    { PIOB, GPIO_PIN_20,         PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel,    NOT_ON_PWM},
    { PIOB, GPIO_PIN_21,         PIN_ATTR_DIGITAL,                                        No_ADC_Channel,    NOT_ON_PWM}, 
    { PIOB, GPIO_PIN_22,         PIN_ATTR_DIGITAL,                                        No_ADC_Channel,    NOT_ON_PWM},
    { PIOB, GPIO_PIN_23,         PIN_ATTR_NONE,                                           No_ADC_Channel,    NOT_ON_PWM},       
    { PIOB, GPIO_PIN_24,         PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel,    NOT_ON_PWM},
    { PIOB, GPIO_PIN_25,         PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel,    NOT_ON_PWM},    
    { PIOB, GPIO_PIN_26,         PIN_ATTR_DIGITAL|PIN_ATTR_PWM,                           No_ADC_Channel,    NOT_ON_PWM},
};