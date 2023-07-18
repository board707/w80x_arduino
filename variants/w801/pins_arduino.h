// Описание выводов для платы w801
#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include "./include/driver/wm_hal.h"
#include <GPIO_defs.h>


// Перечисление выводов - индекс массива возможных альтернатив 

enum pins 
{PA0,PA1,PA2,PA3,PA4,PA5,PA6,PA7,PA8,PA9,PA10,PA11,PA12,PA13,PA14,PA15,
 PB0,PB1,PB2,PB3,PB4,PB5,PB6,PB7,PB8,PB9,PB10,PB11,PB12,PB13,PB14,PB15,PB16,PB17,PB18,PB19,PB20,PB21,PB22,PB23,PB24,PB25,PB26,PB27};

// Arduino macros for analog inputs
#define A1 PA1
#define A2 PA2
#define A3 PA3
#define A4 PA4

// add A0 as alias to A1 for compatibility
#define A0 A1


const PIN_MAP pin_Map[] = {
    { GPIOA, GPIO_PIN_0,	PA0,	NONE},          // Отсутствует на плате 
    { GPIOA, GPIO_PIN_1,	PA1,	DIO|ADC1|PWM3},	// JTAG_CK
    { GPIOA, GPIO_PIN_2,	PA2,	DIO|ADC4|PWM0},
    { GPIOA, GPIO_PIN_3,	PA3,	DIO|ADC3|PWM1},
    { GPIOA, GPIO_PIN_4,	PA4,	DIO|ADC2|PWM4},	// JTAG_SWO
    { GPIOA, GPIO_PIN_5,	PA5,	DIO},
    { GPIOA, GPIO_PIN_6,	PA6,	DIO},
    { GPIOA, GPIO_PIN_7,	PA7,	DIO|PWM4|_SPI_MOSI},
    { GPIOA, GPIO_PIN_8,	PA8,	DIO},
    { GPIOA, GPIO_PIN_9,	PA9,	DIO},
    { GPIOA, GPIO_PIN_10,	PA10,	DIO|PWM0},
    { GPIOA, GPIO_PIN_11,	PA11,	DIO|PWM1},       
    { GPIOA, GPIO_PIN_12,	PA12,	DIO|PWM2},
    { GPIOA, GPIO_PIN_13,	PA13,	DIO|PWM3},    
    { GPIOA, GPIO_PIN_14,	PA14,	DIO|PWM4},
    { GPIOA, GPIO_PIN_15,	PA15,	DIO},             

    { GPIOB, GPIO_PIN_0,	PB0,	DIO|PWM0|_SPI_MISO},
    { GPIOB, GPIO_PIN_1,	PB1,	DIO|PWM1|_SPI_SCK},
    { GPIOB, GPIO_PIN_2,	PB2,	DIO|PWM2|_SPI_SCK},
    { GPIOB, GPIO_PIN_3,	PB3,	DIO|PWM3|_SPI_MISO},
    { GPIOB, GPIO_PIN_4,	PB4,	DIO|_SPI_SS},
    { GPIOB, GPIO_PIN_5,	PB5,	DIO|_SPI_MOSI},
    { GPIOB, GPIO_PIN_6,	PB6,	DIO},
    { GPIOB, GPIO_PIN_7,	PB7,	DIO},
    { GPIOB, GPIO_PIN_8,	PB8,	DIO},
    { GPIOB, GPIO_PIN_9,	PB9,	DIO},
    { GPIOB, GPIO_PIN_10,	PB10,	DIO},
    { GPIOB, GPIO_PIN_11,	PB11,	DIO},       
    { GPIOB, GPIO_PIN_12,	PB12,	DIO|PWM0},
    { GPIOB, GPIO_PIN_13,	PB13,	DIO|PWM1},    
    { GPIOB, GPIO_PIN_14,	PB14,	DIO|PWM2|_SPI_SS},
    { GPIOB, GPIO_PIN_15,	PB15,	DIO|PWM3|_SPI_SCK},             
    { GPIOB, GPIO_PIN_16,	PB16,	DIO|_SPI_MISO},
    { GPIOB, GPIO_PIN_17,	PB17,	DIO|_SPI_MOSI},       
    { GPIOB, GPIO_PIN_18,	PB18,	DIO},
    { GPIOB, GPIO_PIN_19,	PB19,	NONE},	    // TX0 |Используется для загрузки прошивки и вывода на консоль
    { GPIOB, GPIO_PIN_20,	PB20,	NONE},		  // RX0 |отладочных данных через printf()
    { GPIOB, GPIO_PIN_21,	PB21,	DIO}, 
    { GPIOB, GPIO_PIN_22,	PB22,	DIO},
    { GPIOB, GPIO_PIN_23,	PB23,	DIO|_SPI_SS},    
    { GPIOB, GPIO_PIN_24,	PB24,	DIO|PWM2|_SPI_SCK},
    { GPIOB, GPIO_PIN_25,	PB25,	DIO|PWM3|_SPI_MISO}, 
    { GPIOB, GPIO_PIN_26,	PB26,	DIO|PWM4|_SPI_MOSI},
    { GPIOB, GPIO_PIN_27,	PB27,	DIO}
};

// Additional board settings
#include "variant.h"

#endif
