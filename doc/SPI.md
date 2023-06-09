## SPI (Serial Peripheral Interface)

The W80x controller family has one high-speed SPI interface with frequency up to 20 MHz.
SPI bus is supported by the `SPI` library in master mode.

Default SPI pins for W806, W801 and Air103 boards are:

```
#define PIN_SPI_SCK   PB15
#define PIN_SPI_MISO  PB16
#define PIN_SPI_MOSI  PB17
```

Alternative pinouts can be set by using the following method:

##### **`SPI.setSPIpins(uint8_t mosi, uint8_t miso, uint8_t sck);`** 

Valid SPI pins should be as follows:

### Hardware SPI pins table

| **SPI signal** | **Default pins** | **Alternative pins**   | 
|:---------|:----------------:|:--------------:|
| SCK | **PB15**  | PB1, PB2, PB24 | 
| MISO | **PB16** | PB0, PB3, PB25 |
| MOSI | **PB17** | PA7, PB5, PB26 | 
| SS | **PB14** | PA0, PB4, PB23 | 

The function returns true after success pin remap and false otherwise.
Note that you need call **SPI.begin()** before any SPI operation after changing the pins.

### Hardware and software CS select

Note that the ``CS`` pin can be hardware or software controlled by the sketch.
The library uses software CS control by default. To change this you need call **SPI.setHardCS(uint8_t cs)** with one of predefined CS pins above. To switch hardware CS off, use the call **SPI.useSoftCS()**.

### Using interrupts

The current library does not supports the interrupt calls `usingInterrupt`, `notUsingInterrupt`, `attachInterrupt`, and `detachInterrpt`.

### Documentation 

For a detailed overview of the basic SPI interface methods see the Arduino [SPI documentation](https://www.arduino.cc/en/reference/SPI).


