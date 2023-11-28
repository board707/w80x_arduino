#ifndef _VARIANT_ARDUINO_W802_
#define _VARIANT_ARDUINO_W802_

// TFT LCD CONNECTOR
#define PIN_TFT_RST	(PA7)
#define PIN_TFT_RS	(PA12)
#define PIN_TFT_CS	(PA14)
#define PIN_TFT_SCL	(PA9)
#define PIN_TFT_SDI	(PA10)
#define PIN_TFT_LED	(PB8)
#define PIN_TFT_TE	(PB29)

// SPI
#define PIN_SPI_SS    (PB14) 
#define PIN_SPI_SCK   (PB15)
#define PIN_SPI_MISO  (PB16)
#define PIN_SPI_MOSI  (PB17)

#define PINS_COUNT  (44U) // pins in total, PB19, PB20 and PB23 are not used
#define ADC_COUNT   (4U)  // number of ADC channels
#define PWM_COUNT   (5U)  // number of PWM channels

#endif /* _VARIANT_ARDUINO_W802_ */
