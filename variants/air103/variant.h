#ifndef _VARIANT_ARDUINO_Air103_
#define _VARIANT_ARDUINO_Air103_

#define LED_BUILTIN_1 PB24
#define LED_BUILTIN_2 PB25
#define LED_BUILTIN_3 PB26
#define LED_BUILTIN LED_BUILTIN_1

// SPI
#define PIN_SPI_SS    (PB14) 
#define PIN_SPI_SCK   (PB15)
#define PIN_SPI_MISO  (PB16)
#define PIN_SPI_MOSI  (PB17)

#define PINS_COUNT  (44U) // pins in total, PA5, PA6, PB19, PB20, PB21, PB22 and PB23 are not used
#define ADC_COUNT   (4U)  // number of ADC channels
#define PWM_COUNT   (5U)  // number of PWM channels

#endif /* _VARIANT_ARDUINO_Air103_ */
