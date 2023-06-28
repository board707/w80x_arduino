#ifndef _VARIANT_ARDUINO_W806_
#define _VARIANT_ARDUINO_W806_

#define LED_BUILTIN_1 PB0
#define LED_BUILTIN_2 PB1
#define LED_BUILTIN_3 PB2

// SPI
#define PIN_SPI_SS    (PB14) 
#define PIN_SPI_SCK   (PB15)
#define PIN_SPI_MISO  (PB16)
#define PIN_SPI_MOSI  (PB17)


#define PINS_COUNT          (44U)           // Количество выводов на плате. Выводы PB19,PB20,PB23 не используются
#define ADC_COUNT           (4U)            // Количество каналов АЦП
#define PWM_COUNT           (5U)            // Количество каналов ШИМ

#endif /* _VARIANT_ARDUINO_Air103_ */