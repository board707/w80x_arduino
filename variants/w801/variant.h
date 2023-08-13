#ifndef _VARIANT_ARDUINO_W801_
#define _VARIANT_ARDUINO_W801_

#define LED_BUILTIN_1 PB5
#define LED_BUILTIN_2 PB25
#define LED_BUILTIN_3 PB26
#define LED_BUILTIN_4 PB18
#define LED_BUILTIN_5 PB17
#define LED_BUILTIN_6 PB16
#define LED_BUILTIN_7 PB11

// SPI
#define PIN_SPI_SS    (PB14) 
#define PIN_SPI_SCK   (PB15)
#define PIN_SPI_MISO  (PB16)
#define PIN_SPI_MOSI  (PB17)

#define PINS_COUNT  (44U) // pins in total, PB19, PB20 and PB23 are not used
#define ADC_COUNT   (4U)  // number of ADC channels
#define PWM_COUNT   (5U)  // number of PWM channels

#endif /* _VARIANT_ARDUINO_W801_ */
