#ifndef _VARIANT_ARDUINO_W800_
#define _VARIANT_ARDUINO_W800_

#define LED_BUILTIN_1 PB0
#define LED_BUILTIN_2 PB1
#define LED_BUILTIN_3 PB2
#define LED_BUILTIN LED_BUILTIN_1

// SPI
#define PIN_SPI_SS    (PB4) 
#define PIN_SPI_SCK   (PB1)
#define PIN_SPI_MISO  (PB0)
#define PIN_SPI_MOSI  (PB5)

#define PINS_COUNT  (44U) // pins in total, PB19, PB20 and PB23 are not used
#define ADC_COUNT   (4U)  // number of ADC channels
#define PWM_COUNT   (5U)  // number of PWM channels

#endif /* _VARIANT_ARDUINO_W800_ */
