#ifndef __GPIO_DEFS_H__
#define __GPIO_DEFS_H__

// Атрибуты выводов

// Pin masks
#define PIN_DIO_Pos (0)
#define PIN_DIO_Msk (1 << PIN_DIO_Pos)

#define PIN_PWM_Pos (1)
#define PIN_PWM_Msk ( 0b111 << PIN_PWM_Pos )

#define PIN_ADC_Pos (4)
#define PIN_ADC_Msk ( 0b111 << PIN_ADC_Pos )


#define NONE	(0)		// Вывод не используется
#define DIO     (1 << PIN_DIO_Pos)		// Цифровой ввод/вывод

#define PWM0    ((0+1) << PIN_PWM_Pos)		// Каналы ШИМ
#define PWM1    ((1+1) << PIN_PWM_Pos)
#define PWM2    ((2+1) << PIN_PWM_Pos)
#define PWM3    ((3+1) << PIN_PWM_Pos)
#define PWM4    ((4+1) << PIN_PWM_Pos)

#define ADC1    ((1) << PIN_ADC_Pos)		// Каналы АЦП
#define ADC2    ((2) << PIN_ADC_Pos)
#define ADC3    ((3) << PIN_ADC_Pos)
#define ADC4    ((4) << PIN_ADC_Pos)



// Константная таблица/массив возможных альтернатив
// Порт,Вывод порта,Пин платы,Альтернативы

typedef struct {
  GPIO_TypeDef *  pPort ;		// Указатель на порт
  uint32_t  halPin ;			// Вывод определен в HAL
  uint32_t  ulPin ;         	// Вывод на плате
  uint32_t 	ulPinAttribute ;    // Аттрибуты вывода (альтернативы)                    
} PIN_MAP ;



#endif