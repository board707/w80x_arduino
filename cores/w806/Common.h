#ifndef Common_h
#define Common_h
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include "./include/driver/wm_pwm.h"
#include "Binary.h"

#define SPIClass HardSPI

#ifdef __cplusplus 
extern "C" {
#endif
 
typedef enum {
  LSBFIRST = 0,
  MSBFIRST = 1,
} BitOrder;


/* 
#ifndef LSBFIRST
#define LSBFIRST 0
#endif
#ifndef MSBFIRST
#define MSBFIRST 1
#endif
*/
typedef uint8_t byte;
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

// AVR compatibility macros...naughty and accesses the HW directly
#define digitalPinToPort(pin)       ((pin < PB0) ? GPIOA : GPIOB ) 
#define digitalPinToBitMask(pin)    ((pin < PB0) ? (1UL << (pin)) : (1UL << (pin - PB0)))
#define digitalPinToTimer(pin)      (0)
#define digitalPinToInterrupt(pin)  (pin)
#define NOT_AN_INTERRUPT            (-1)
#define portOutputRegister(port)    ((volatile uint32_t*) &(port -> DATA))
#define portInputRegister(port)     ((volatile uint32_t*) &(port -> DATA))
#define portModeRegister(port)      ((volatile uint32_t*) &(port -> DIR))

// TODO: Add a _disable_irq() & _enable_irq() here
#define interrupts()                { __enable_irq(); }
#define noInterrupts()              { __disable_irq(); }

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

// PWM Resolution
#define PWM_8BIT  (0xFF)  // defined by HAL registers size

// Прототипы для DIO
void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t val);
void digitalToggle(uint8_t pin);
uint8_t digitalRead(uint8_t pin);

// Прототипы для режимов ШИМ/АЦП
//void setup_pwm(void);
PWM_HandleTypeDef* getPWMHandle(uint8_t pin);
void analogWrite(uint8_t pin, uint8_t val);
//void setup_adc(void);
double analogRead(uint8_t pin);

// Прототипы для измерения времени в runtime
uint32_t millis(void);
uint32_t micros(void);

// Прототипы для функций задержек
void delay(uint32_t ms);
void delayMicroseconds(uint32_t us);

// Прототип для контроля вхождения в суперцикл Loop()
//bool is_loop(void) ;
//void set_loop(bool param);

// Прототипы для скетча Ардуино
void setup(void);
void loop(void);

//Прототипы для wiring shiftOut/shiftIn
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder); 
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);


// added for compatibility
void yield();

#ifdef __cplusplus 
}
#endif
#ifdef __cplusplus
// WMath prototypes //Случайные числа stdlib
int32_t random(int32_t);
int32_t random(int32_t, int32_t);
void randomSeed(uint32_t);
int32_t map(int32_t, int32_t, int32_t, int32_t, int32_t);
#endif
#endif