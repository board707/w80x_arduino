#ifndef Arduino_h
#define Arduino_h

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include "pins_arduino.h"
#include "./include/driver/wm_hal.h"
#include "W_interrupt.h"
/*
 * C++ type
 */
#ifdef __cplusplus
#include "Stream.h"
#endif

#define TIMER_COUNT 6
// Timer instances (defined in Arduino.c)
extern TIM_HandleTypeDef htim0;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;

#define UART_COUNT 6
// UART instances (defined in Arduino.c)
extern UART_HandleTypeDef huart0;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;


#ifdef __cplusplus 
extern "C" {
#endif
 
#ifndef LSBFIRST
#define LSBFIRST 0
#endif
#ifndef MSBFIRST
#define MSBFIRST 1
#endif
typedef int8_t byte;
typedef bool boolean;

// Блок математики
// Переопределение числа Pi 
#ifdef PI
#undef PI
#endif
#ifdef HALF_PI
#undef HALF_PI
#endif
#ifdef TWO_PI
#undef TWO_PI
#endif

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

// Переопределение abs()
#ifdef abs
#undef abs
#endif

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

//Битовые операции
#define byte(w) ((uint8_t)(w))
#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

#define maskSet(value, mask) ((value) |= (mask))
#define maskClear(value, mask) ((value) &= ~(mask))

//Случайные числа
#define randomSeed(seed)   srand(seed)
#define random(max)         random2(0, max)
#define random2(min, max)   ((min) + (int32_t) ((max) - (min)) * rand() / 32768)

//Истинные случайные числа
//В SDK для w806 этого нет. Подсмотрено у отцов основателей WinnerMicro

#define GPSEC_BASE         0x40000600	// Адрес криптомодуля

typedef struct {
   __IOM uint32_t SRC;               //source register @ 0x00
   __IOM uint32_t DEST;            //destination / decrypted text register @ 0x04
   __IOM uint32_t CFG;               //configuration register @ 0x08
   __IOM uint32_t CTRL;            //controll register @ 0x0c
   __IOM uint32_t KEY0;            //key0 register @ 0x10
   __IOM uint32_t KEY1;            //key1 register @ 0x14
   __IOM uint32_t KEY2;            //key2 register @ 0x18
   __IOM uint32_t KEY3;            //key3 register @ 0x1c
   __IOM uint32_t KEY4;            //key4 register = crc key / result register @ 0x20
   __IOM uint32_t KEY5;            //key5 register @ 0x24
   __IOM uint32_t IV0;               //iv0 register @ 0x28
   __IOM uint32_t IV1;               //iv1 register @ 0x2c
   __IOM uint32_t STS;               //status register @ 0x30
   __IOM uint32_t SHA1_DIGEST0;      //sha1/md5 digest0 register @ 0x34
   __IOM uint32_t SHA1_DIGEST1;      //sha1/md5 digest1 register @ 0x38
   __IOM uint32_t SHA1_DIGEST2;      //sha1/md5 digest2 register @ 0x3c
   __IOM uint32_t SHA1_DIGEST3;      //sha1/md5 digest3 register # 0x40
   __IOM uint32_t SHA1_DIGEST4;      //sha1/md5 digest4 register @ 0x44
   __IOM uint32_t RNG_RESULT;         //random number result register # 0x48
   __IOM uint32_t KEY6;            //key6 register @ 0x4c
   __IOM uint32_t KEY7;            //key7 register @ 0x50
   __IOM uint32_t TRNG_CR;            //true random number control register @ 0x54
} GPSEC_TypeDef;

#define GPSEC            ((GPSEC_TypeDef *) GPSEC_BASE)

void trngInit(void);    //Инициализация истинных случайных чисел 32bit
void prngInit(void);    //Инициализация псевдо случайных чисел 32bit
#define rngGet()      	(GPSEC->RNG_RESULT)      // Результат


// Флаги тактирования периферии
static volatile bool is_gpio_clk_en ;
static volatile bool is_pwm_clk_en ;
static volatile bool is_adc_clk_en;

// Определения для DIO
#define HIGH 			0x1
#define LOW  			0x0

#define INPUT 			0b00000001
#define OUTPUT 			0b00000010
#define INPUT_PULLUP 	0b00000100
#define INPUT_PULLDOWN 	0b00001000

// Определения для ШИМ/АЦП
#define ANALOG_INPUT 	0b00010000
#define PWM_OUT 		0b00100000

// Прототипы для DIO
void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t val);
void digitalToggle(uint8_t pin);
uint8_t digitalRead(uint8_t pin);

// Прототипы для режимов ШИМ/АЦП
void setup_pwm(void);
void analogWrite(uint8_t pin, uint8_t val);
void setup_adc(void);
double analogRead(uint8_t pin);

// Прототипы для измерения времени в runtime
uint32_t millis(void);
uint32_t micros(void);

// Прототипы для функций задержек
void delay(uint32_t ms);
void delayMicroseconds(uint32_t us);

// Прототип для контроля вхождения в суперцикл Loop()
bool is_loop(void) ;
void set_loop(bool param);

// Прототипы для скетча Ардуино
void setup(void);
void loop(void);

//Прототипы для wiring shiftOut/shiftIn
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder); 
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);


#ifdef __cplusplus 
}
#endif
#endif


