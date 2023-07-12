#ifndef _HARDSPI_H
#define _HARDSPI_H

#include "Arduino.h"

// Внимание! Эта версия не совместима с SPI ванильного Ардуино... Но это пока.
// Чип селектом управляете сами, в ручном режиме, через digitalWrite

#define LSBFIRST 0
#define MSBFIRST 1

#define SPI_MODE0 0x00
#define SPI_MODE1 0x01
#define SPI_MODE2 0x02
#define SPI_MODE3 0x03

#define VALID_SPI_MODE(MODE) (((MODE) == SPI_MODE0 ) || \
                               ((MODE) == SPI_MODE1 ) || \
                               ((MODE) == SPI_MODE2 ) || \
                               ((MODE) == SPI_MODE3 ) )

// Soft_SPI
#define SPI_CLOCK_DIV4 0x00
#define SPI_CLOCK_DIV16 0x01
#define SPI_CLOCK_DIV64 0x02
#define SPI_CLOCK_DIV128 0x03
#define SPI_CLOCK_DIV2 0x04
#define SPI_CLOCK_DIV8 0x05
#define SPI_CLOCK_DIV32 0x06

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
    uint8_t   _miso = PIN_SPI_MISO;
    uint8_t   _mosi = PIN_SPI_MOSI;
    uint8_t   _sck  = PIN_SPI_SCK;
	int32_t   _clock = 4000000;
    uint16_t  _dataOrder = SPI_LITTLEENDIAN;    // not used in the code
    uint16_t  _bitOrder = MSBFIRST;
    uint8_t   _dataMode = SPI_MODE0;
	bool _is_init = false;
    private:
    //Base_SPI *pSPI;
	
	public:
	// methods
		
        //Base_SPI(int) : pSPI(NULL)  {};
        //Base_SPI(uint8_t mosi, uint8_t miso, uint8_t sck); 
        //Base_SPI() : Base_SPI(PB26, PB25, PB24) {};
        Base_SPI()  {};
        
		void SPI_Settings(uint32_t clock, uint16_t bitOrder, uint8_t dataMode) ;
               //{pSPI-> SPI_Settings(clock, bitOrder, dataMode); };
 		void SPI_Settings(SPISettings settings) ;
        void beginTransaction(SPISettings settings) 
               {SPI_Settings(settings);
                this-> beginTransaction();};
		void beginTransaction() { this-> begin();};
        void endTransaction() { this-> end();};	

        virtual void begin() =0 ;
        virtual void end() =0;	
        virtual uint8_t transfer(uint8_t data)  =0 ;	
		virtual uint16_t transfer16(uint16_t data)  =0 ;
		//virtual void transfer(void *buf, size_t count)  =0 ;
		
		virtual void setBitOrder(uint8_t);
        virtual void setDataMode(uint8_t);
        
        uint8_t reverseByte(uint8_t b);
        virtual bool isSPIpins(uint8_t mosi, uint8_t miso, uint8_t sck);
        // Deprecated, NOT IMPLEMENTED YET!
        //virtual void setClockDivider(uint8_t) {};
		
};

class HardSPI : public Base_SPI {
    
    private:
    uint8_t _ss = PIN_SPI_SS;
    bool _use_hard_cs = false;
    uint8_t  _prescaler = SPI_BAUDRATEPRESCALER_10;
    uint8_t clock_polarity ;
    uint8_t clock_phase ;
	uint32_t timeOut = 1000;
	SPI_HandleTypeDef hspi;
    
	
	public:
	// methods
		HardSPI() : HardSPI(PIN_SPI_MOSI, PIN_SPI_MISO, PIN_SPI_SCK) /* Pins by default */ {};
        HardSPI(uint8_t mosi, uint8_t miso, uint8_t sck) ;// alternatives see datasheet
		//void SPI_Settings(uint32_t clock, uint16_t bitOrder, uint8_t dataMode);
 		//void beginTransaction(SPISettings settings);
		//void beginTransaction() {begin();};		
		//void endTransaction() {this->end();};		
        void begin();
        void end();
        uint8_t transfer(uint8_t);
		uint16_t transfer16(uint16_t data);
		void transfer(void *buf, size_t count);
        void transfer(void *tbuf, void *rbuf,size_t count);
        bool isSPIpins(uint8_t mosi, uint8_t miso, uint8_t sck);
        bool setSPIpins(uint8_t mosi, uint8_t miso, uint8_t sck);
        bool setHardCS(uint8_t cs);
        void useSoftCS();
		void setTimeout(uint32_t timeout)  { timeOut = timeout;}
		uint32_t getTimeout() {return timeOut;};
        SPI_HandleTypeDef* getHalhandle() {return &hspi;}
        //void setBitOrder(uint8_t);
        //void setDataMode(uint8_t);
        //void setClockDivider(uint8_t);
		
};

class SoftSPI : public Base_SPI {
    private:
        void wait(uint_fast8_t del);

    private:
        uint8_t _cke =0;
        uint8_t _ckp =0;
        uint8_t _delay = 2;
        uint8_t _clock_div = SPI_CLOCK_DIV2;
    public:
        SoftSPI(uint8_t mosi, uint8_t miso, uint8_t sck);
        void begin();
        void end();
        void setClockDivider(uint8_t);
        uint8_t transfer(uint8_t);
		uint16_t transfer16(uint16_t data);
};
extern HardSPI SPI;
//extern Base_SPI* SPI;
#endif
