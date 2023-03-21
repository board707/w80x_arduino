// Описание выводов для платы w801

#include "./include/driver/wm_hal.h"

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#define PINS_COUNT          (44U)           // Количество выводов на плате. Выводы PB19,PB20,PB23 не используются
#define ADC_COUNT           (4U)            // Количество каналов АЦП
#define PWM_COUNT           (5U)            // Количество каналов ШИМ


// Перечисление выводов - индекс массива возможных альтернатив 

enum pins 
{PA0,PA1,PA2,PA3,PA4,PA5,PA6,PA7,PA8,PA9,PA10,PA11,PA12,PA13,PA14,PA15,
 PB0,PB1,PB2,PB3,PB4,PB5,PB6,PB7,PB8,PB9,PB10,PB11,PB12,PB13,PB14,PB15,PB16,PB17,PB18,PB19,PB20,PB21,PB22,PB23,PB24,PB25,PB26,PB27};

// Аналоговые входы в стиле Ардуино

#define A0	PA1
#define A1	PA4
#define A2	PA3
#define A4	PA2

// Атрибуты выводов

#define NONE	0b0000000000000000		// Вывод не используется
#define DIO     0b0000000000000001		// Цифровой ввод/вывод
#define PWM0    0b0000000000000010		// Каналы ШИМ
#define PWM1    0b0000000000000100
#define PWM2    0b0000000000001000
#define PWM3    0b0000000000010000
#define PWM4    0b0000000000100000
#define ADC1    0b0000000001000000		// Каналы АЦП
#define ADC2    0b0000000010000000
#define ADC3    0b0000000100000000
#define ADC4    0b0000001000000000

// Константная таблица/массив возможных альтернатив
// Порт,Вывод порта,Пин платы,Альтернативы

typedef struct {
  GPIO_TypeDef *  pPort ;		// Указатель на порт
  uint32_t  halPin ;			// Вывод определен в HAL
  uint32_t  ulPin ;         	// Вывод на плате
  uint32_t 	ulPinAttribute ;    // Аттрибуты вывода (альтернативы)                    
} PIN_MAP ;

const PIN_MAP pin_Map[] = {
    { GPIOA, GPIO_PIN_0,	PA0,	NONE},          // Отсутствует на плате 
    { GPIOA, GPIO_PIN_1,	PA1,	DIO|ADC1|PWM3},	// JTAG_CK
    { GPIOA, GPIO_PIN_2,	PA2,	DIO|ADC4|PWM0},
    { GPIOA, GPIO_PIN_3,	PA3,	DIO|ADC3|PWM1},
    { GPIOA, GPIO_PIN_4,	PA4,	DIO|ADC2|PWM4},	// JTAG_SWO
    { GPIOA, GPIO_PIN_5,	PA5,	DIO},
    { GPIOA, GPIO_PIN_6,	PA6,	DIO},
    { GPIOA, GPIO_PIN_7,	PA7,	DIO|PWM4},
    { GPIOA, GPIO_PIN_8,	PA8,	DIO},
    { GPIOA, GPIO_PIN_9,	PA9,	DIO},
    { GPIOA, GPIO_PIN_10,	PA10,	DIO|PWM0},
    { GPIOA, GPIO_PIN_11,	PA11,	DIO|PWM1},       
    { GPIOA, GPIO_PIN_12,	PA12,	DIO|PWM2},
    { GPIOA, GPIO_PIN_13,	PA13,	DIO|PWM3},    
    { GPIOA, GPIO_PIN_14,	PA14,	DIO|PWM4},
    { GPIOA, GPIO_PIN_15,	PA15,	DIO},             

    { GPIOB, GPIO_PIN_0,	PB0,	DIO|PWM0},
    { GPIOB, GPIO_PIN_1,	PB1,	DIO|PWM1},
    { GPIOB, GPIO_PIN_2,	PB2,	DIO|PWM2},
    { GPIOB, GPIO_PIN_3,	PB3,	DIO|PWM3},
    { GPIOB, GPIO_PIN_4,	PB4,	DIO},
    { GPIOB, GPIO_PIN_5,	PB5,	DIO},
    { GPIOB, GPIO_PIN_6,	PB6,	DIO},
    { GPIOB, GPIO_PIN_7,	PB7,	DIO},
    { GPIOB, GPIO_PIN_8,	PB8,	DIO},
    { GPIOB, GPIO_PIN_9,	PB9,	DIO},
    { GPIOB, GPIO_PIN_10,	PB10,	DIO},
    { GPIOB, GPIO_PIN_11,	PB11,	DIO},       
    { GPIOB, GPIO_PIN_12,	PB12,	DIO|PWM0},
    { GPIOB, GPIO_PIN_13,	PB13,	DIO|PWM1},    
    { GPIOB, GPIO_PIN_14,	PB14,	DIO|PWM2},
    { GPIOB, GPIO_PIN_15,	PB15,	DIO|PWM3},             
    { GPIOB, GPIO_PIN_16,	PB16,	DIO},
    { GPIOB, GPIO_PIN_17,	PB17,	DIO},       
    { GPIOB, GPIO_PIN_18,	PB18,	DIO},
    { GPIOB, GPIO_PIN_19,	PB19,	NONE},	    // TX0 |Используется для загрузки прошивки и вывода на консоль
    { GPIOB, GPIO_PIN_20,	PB20,	NONE},		  // RX0 |отладочных данных через printf()
    { GPIOB, GPIO_PIN_21,	PB21,	DIO}, 
    { GPIOB, GPIO_PIN_22,	PB22,	DIO},
    { GPIOB, GPIO_PIN_23,	PB23,	DIO},      
    { GPIOB, GPIO_PIN_24,	PB24,	DIO|PWM2},
    { GPIOB, GPIO_PIN_25,	PB25,	DIO|PWM3},    
    { GPIOB, GPIO_PIN_26,	PB26,	DIO|PWM4},
    { GPIOB, GPIO_PIN_27,	PB27,	DIO}
};

// Additional board settings
#include "variant.h"

#endif
