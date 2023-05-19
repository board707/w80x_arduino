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
#include "Common.h"
/*
 * C++ type
 */
#ifdef __cplusplus
#include "Stream.h"
#include "HardwareSerial.h"
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

#ifdef __cplusplus
extern "C"  {
#endif

void trngInit(void);    //Инициализация истинных случайных чисел 32bit
void prngInit(void);    //Инициализация псевдо случайных чисел 32bit

#ifdef __cplusplus
}
#endif
#define rngGet()      	(GPSEC->RNG_RESULT)      // Результат


// Флаги тактирования периферии
static volatile bool is_gpio_clk_en ;
static volatile bool is_pwm_clk_en ;
static volatile bool is_adc_clk_en;


#endif

