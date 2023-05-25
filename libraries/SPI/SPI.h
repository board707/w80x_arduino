#ifndef _HARDSPI_H
#define _HARDSPI_H

#include "Arduino.h"

// Внимание! Эта версия не совместима с SPI ванильного Ардуино... Но это пока.
// Чип селектом управляете сами, в ручном режиме, через digitalWrite

#ifndef LSBFIRST
#  define LSBFIRST 0
#endif

#define SPI_MODE0 0x00
#define SPI_MODE1 0x01
#define SPI_MODE2 0x02
#define SPI_MODE3 0x03


class SPISettings
{
public:
    SPISettings(uint32_t clock, uint16_t bitOrder, uint8_t dataMode)
    {
        init_AlwaysInline(clock, bitOrder, dataMode);
    }
    SPISettings(uint32_t clock)
    {
        init_AlwaysInline(clock, LSBFIRST, SPI_MODE0);
    }
    SPISettings()
    {
        init_AlwaysInline(4000000, LSBFIRST, SPI_MODE0);
    }
private:

    void init_AlwaysInline(uint32_t clock, uint16_t bitOrder, uint8_t dataMode) __attribute__((__always_inline__))
    {
        this->clock = clock;
        this->bitOrder = bitOrder;
        this->dataMode = dataMode;
    }
    uint32_t clock;
    uint16_t bitOrder;
    uint8_t dataMode;

    friend class HardSPI;
    friend class Base_SPI;
};
// Максимальная частота шины SPI clock - 20 Мhz
class Base_SPI {
    
    protected:
	// var
    uint8_t   _miso = PB25;
    uint8_t   _mosi = PB26;
    uint8_t   _sck  = PB24;
	int32_t   _clock = 4000000;
    uint16_t  _bitOrder = SPI_LITTLEENDIAN;
    uint8_t   _dataMode = SPI_MODE0;
	bool _is_init = false;
    private:
    Base_SPI *pSPI;
	
	public:
	// methods
		
        Base_SPI(int) : pSPI(NULL)  {};
        Base_SPI(uint8_t mosi, uint8_t miso, uint8_t sck); 
        Base_SPI() : Base_SPI(PB26, PB25, PB24) {};
        bool isSPIpins(uint8_t mosi, uint8_t miso, uint8_t sck); 
		void SPI_Settings(uint32_t clock, uint16_t bitOrder, uint8_t dataMode) ;
               //{pSPI-> SPI_Settings(clock, bitOrder, dataMode); };
 		void SPI_Settings(SPISettings settings) ;
        void beginTransaction(SPISettings settings) 
               {SPI_Settings(settings);
                beginTransaction();};
		void beginTransaction() {pSPI -> begin();};
        
        virtual void endTransaction() {pSPI-> end();};	
        virtual void begin() {pSPI -> begin();};
        virtual void end() {pSPI -> end();};	
        virtual uint8_t transfer(uint8_t data) { return pSPI -> transfer(data);};	
		virtual uint16_t transfer16(uint16_t data) {return pSPI -> transfer16(data);};
		virtual void transfer(void *buf, size_t count) {pSPI -> transfer(buf, count);};
		
		virtual void setBitOrder(uint8_t);
        virtual void setDataMode(uint8_t) {};

        // Deprecated, NOT IMPLEMENTED YET!
        virtual void setClockDivider(uint8_t) {};
		
};

class HardSPI : public Base_SPI {
    
    private:
    
    uint8_t  _prescaler = SPI_BAUDRATEPRESCALER_10;
    uint8_t clock_polarity ;
    uint8_t clock_phase ;
	uint16_t timeOut = 1000;
	SPI_HandleTypeDef hspi;
    
	
	public:
	// methods
		HardSPI() : HardSPI(PB26, PB25, PB24) /* Pins by default */ {};
        HardSPI(uint8_t mosi, uint8_t miso, uint8_t sck) ;// alternatives see datasheet
		//void SPI_Settings(uint32_t clock, uint16_t bitOrder, uint8_t dataMode);
 		//void beginTransaction(SPISettings settings);
		//void beginTransaction() {begin();};		
		void endTransaction() {this->end();};		
        void begin();
        void end();
        uint8_t transfer(uint8_t);
		uint16_t transfer16(uint16_t data);
		void transfer(void *buf, size_t count);
		
		
        //void setBitOrder(uint8_t);
        void setDataMode(uint8_t);
        void setClockDivider(uint8_t);
		
};
extern Base_SPI SPI;
//extern Base_SPI* SPI;
#endif
