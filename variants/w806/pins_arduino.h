// pin description for HLK-W806-KIT board
#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include "./include/driver/wm_hal.h"
#include <GPIO_defs.h>

// pin enum - index of an array of possible alternatives
enum pins 
{
  PA0,PA1,PA2,PA3,PA4,PA5,PA6,PA7,PA8,PA9,PA10,PA11,PA12,PA13,PA14,PA15,
  PB0,PB1,PB2,PB3,PB4,PB5,PB6,PB7,PB8,PB9,PB10,PB11,PB12,PB13,PB14,PB15,
  PB16,PB17,PB18,PB19,PB20,PB21,PB22,PB23,PB24,PB25,PB26,PB27
};

// Arduino style analog inputs
#define A0  PA1
#define A1  PA4
#define A2  PA3
#define A4  PA2

// pin mux macros for W806
#define MUX_PA0 (DIO | PWM2 | _SPI_SS)      // Boot
#define MUX_PA1 (DIO | ADC1 | PWM3)         // JTAG_CK
#define MUX_PA2 (DIO | ADC4 | PWM0)
#define MUX_PA3 (DIO | ADC3 | PWM1)
#define MUX_PA4 (DIO | ADC2 | PWM4)         // JTAG_SWO
#define MUX_PA5 (DIO)
#define MUX_PA6 (DIO)
#define MUX_PA7 (DIO | PWM4 | _SPI_MOSI)
#define MUX_PA8 (DIO)
#define MUX_PA9 (DIO)
#define MUX_PA10 (DIO | PWM0)
#define MUX_PA11 (DIO | PWM1)
#define MUX_PA12 (DIO | PWM2)
#define MUX_PA13 (DIO | PWM3)
#define MUX_PA14 (DIO | PWM4)
#define MUX_PA15 (DIO)

#define MUX_PB0 (DIO | PWM0 | _SPI_MISO)    // LED_BUILTIN_1
#define MUX_PB1 (DIO | PWM1 | _SPI_SCK)     // LED_BUILTIN_2
#define MUX_PB2 (DIO | PWM2 | _SPI_SCK)     // LED_BUILTIN_3
#define MUX_PB3 (DIO | PWM3 | _SPI_MISO)
#define MUX_PB4 (DIO | _SPI_SS)
#define MUX_PB5 (DIO | _SPI_MOSI)
#define MUX_PB6 (DIO)
#define MUX_PB7 (DIO)
#define MUX_PB8 (DIO)
#define MUX_PB9 (DIO)
#define MUX_PB10 (DIO)
#define MUX_PB11 (DIO)
#define MUX_PB12 (DIO | PWM0)
#define MUX_PB13 (DIO | PWM1)
#define MUX_PB14 (DIO | PWM2 | _SPI_SS)
#define MUX_PB15 (DIO | PWM3 | _SPI_SCK)
#define MUX_PB16 (DIO | _SPI_MISO)
#define MUX_PB17 (DIO | _SPI_MOSI)
#define MUX_PB18 (DIO)
#define MUX_PB19 (NONE) // TX0: used to download firmware and output to console
#define MUX_PB20 (NONE) // RX0: debug data via printf()
#define MUX_PB21 (DIO)
#define MUX_PB22 (DIO)
#define MUX_PB23 (NONE)                     // no pin for PB23 on W806 package
#define MUX_PB24 (DIO | PWM2 | _SPI_SCK)
#define MUX_PB25 (DIO | PWM3 | _SPI_MISO)
#define MUX_PB26 (DIO | PWM4 | _SPI_MOSI)
#define MUX_PB27 (DIO)

extern const PIN_MAP pin_Map[];

// Additional board settings
#include "variant.h"

#endif
