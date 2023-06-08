#include "SPI.h"

//HardSPI SPI;
//Base_SPI* SPI = &hSPI;
HardSPI SPI;

HardSPI::HardSPI(uint8_t mosi, uint8_t miso, uint8_t sck)  {
    _mosi = mosi;
    _miso = miso;
    _sck = sck;
}


void HardSPI::begin() {

    if ( _is_init ) {
      this -> end();
    }

    switch(_dataMode)
    {
    case SPI_MODE0:
        clock_polarity = SPI_POLARITY_LOW;
        clock_phase = SPI_PHASE_1EDGE;
        break;
    case SPI_MODE1:
        clock_polarity = SPI_POLARITY_LOW;
        clock_phase = SPI_PHASE_2EDGE;
        break;
    case SPI_MODE2:
        clock_polarity = SPI_POLARITY_HIGH;
        clock_phase = SPI_PHASE_1EDGE;
        break;
    case SPI_MODE3:
        clock_polarity = SPI_POLARITY_HIGH;
        clock_phase = SPI_PHASE_2EDGE;
        break;
    default:
		break;
	}
	// Максимальная частота шины SPI 20 Мгц
    switch(_clock)
    {
    case 20000000:
		_prescaler = SPI_BAUDRATEPRESCALER_2;
        break;
    case 10000000:
		_prescaler = SPI_BAUDRATEPRESCALER_4;
        break;
    case 5000000:
		_prescaler = SPI_BAUDRATEPRESCALER_8;
        break;
    case 4000000:
		_prescaler = SPI_BAUDRATEPRESCALER_10;
        break;
    case 2000000:
		_prescaler = SPI_BAUDRATEPRESCALER_20;
        break;
    case 1000000:
		_prescaler = SPI_BAUDRATEPRESCALER_40;
        break;
    default:
        _prescaler = SPI_BAUDRATEPRESCALER_40;     // set SPI 1 NHz in case of incorrect value
		_clock = 1000000;
        break;
	}
	
    __HAL_RCC_SPI_CLK_ENABLE();
    __HAL_AFIO_REMAP_SPI_CLK(pin_Map[_sck].pPort, pin_Map[_sck].halPin);
    __HAL_AFIO_REMAP_SPI_MISO(pin_Map[_miso].pPort, pin_Map[_miso].halPin);
    __HAL_AFIO_REMAP_SPI_MOSI(pin_Map[_mosi].pPort, pin_Map[_mosi].halPin);
	
	hspi.Instance = SPI_HAL;
	hspi.Init.Mode = SPI_MODE_MASTER;
	hspi.Init.CLKPolarity = clock_polarity;
	hspi.Init.CLKPhase = clock_phase;
	// Ручное управление чип_селектом
	hspi.Init.NSS = SPI_NSS_SOFT;
	hspi.Init.BaudRatePrescaler = _prescaler;
	hspi.Init.FirstByte = SPI_LITTLEENDIAN; // ONLY! Говорят что BIGENDIAN нормально не работает. Не проверял.
	if (HAL_SPI_Init(&hspi) == HAL_OK) _is_init = true;
}

void HardSPI::end() {
   
	HAL_GPIO_DeInit(pin_Map[_sck].pPort, pin_Map[_sck].halPin);
	HAL_GPIO_DeInit(pin_Map[_miso].pPort, pin_Map[_miso].halPin);
	HAL_GPIO_DeInit(pin_Map[_mosi].pPort, pin_Map[_mosi].halPin);
	HAL_SPI_DeInit(&hspi);
    __HAL_RCC_SPI_CLK_DISABLE();
    _is_init = false;

}
uint8_t HardSPI::transfer(uint8_t val) {
    if (! _is_init) 
      return 0;

    uint8_t recv;
    if (_bitOrder == LSBFIRST) val = reverseByte(val);
    HAL_SPI_TransmitReceive(&hspi, &val, &recv, 1, timeOut);
    if (_bitOrder == LSBFIRST) recv = reverseByte(recv);
    return recv;
}
uint16_t HardSPI::transfer16(uint16_t data) 
{
     if (! _is_init) 
      return 0;
      
  union
    {
         uint8_t _8recv[2];
         uint16_t recv;
      /* data */
    }in, out;

    in.recv = data;
    if (_bitOrder == LSBFIRST) {
        in._8recv[0] = reverseByte(in._8recv[0]);
        in._8recv[1] = reverseByte(in._8recv[1]);
    }

    HAL_SPI_TransmitReceive(&hspi, in._8recv, out._8recv, 2, timeOut);
    
    if (_bitOrder == LSBFIRST) {
        out._8recv[0] = reverseByte(out._8recv[0]);
        out._8recv[1] = reverseByte(out._8recv[1]);
    }
    return out.recv;
}

void HardSPI::transfer(void *buf, size_t count) {
     if (! _is_init) 
      return ;
      
    if (count == 0)
      return ;
    
    uint32_t t_out = timeOut;
    if ((count * 100 * 1000000)/_clock > timeOut ) t_out = (count * 100 * 1000000)/_clock;

    if (_bitOrder == LSBFIRST) {
        uint8_t buf2[count];
        for (size_t i =0; i < count; i++) {buf2[i] = reverseByte( *((uint8_t *)buf + i));}
        HAL_SPI_Transmit(&hspi, (uint8_t *)buf2, count, t_out);
    }
    else HAL_SPI_Transmit(&hspi, (uint8_t *)buf, count, t_out);
}

void HardSPI::transfer(void *tbuf, void *rbuf,size_t count) {
     if (! _is_init) 
      return ;
      
    if (count == 0)
      return ;

    if (rbuf == NULL) { // transmit only!
      this-> transfer(tbuf, count);
      return;
    }  

    uint32_t t_out = timeOut;
    if ((count * 100 * 1000000)/_clock > timeOut ) t_out = (count * 100 * 1000000)/_clock;

    if (tbuf == NULL) { // receive only!
      if (_bitOrder == LSBFIRST) {
        uint8_t buf2[count];
        HAL_SPI_Receive(&hspi, (uint8_t *)buf2, count, t_out);
        for (size_t i =0; i < count; i++) { *((uint8_t *)rbuf + i) = reverseByte( buf2[i]);}
      }
       else HAL_SPI_Receive(&hspi, (uint8_t *)rbuf, count, t_out);

      return;
    }  
    
    // transmit - receive
    if (_bitOrder == LSBFIRST) {
        uint8_t buf2[count];
        uint8_t buf3[count];
        for (size_t i =0; i < count; i++) {buf2[i] = reverseByte( *((uint8_t *)tbuf + i));}
        HAL_SPI_TransmitReceive(&hspi, (uint8_t *)buf2, (uint8_t *)buf3, count, t_out);
        for (size_t i =0; i < count; i++) { *((uint8_t *)rbuf + i) = reverseByte( buf3[i]);}
    }
    HAL_SPI_TransmitReceive(&hspi, (uint8_t *)tbuf, (uint8_t *)rbuf, count, t_out);

}

bool HardSPI::isSPIpins(uint8_t mosi, uint8_t miso, uint8_t sck) {

    return ( ((pin_Map[mosi].ulPinAttribute & PIN_SPI_Msk) == _SPI_MOSI) && 
            ((pin_Map[miso].ulPinAttribute & PIN_SPI_Msk) == _SPI_MISO) && 
            ((pin_Map[sck].ulPinAttribute & PIN_SPI_Msk) == _SPI_SCK) ) ;
   
}

bool HardSPI::setSPIpins(uint8_t mosi, uint8_t miso, uint8_t sck) {

    if (isSPIpins(mosi, miso, sck)) {
        _mosi = mosi;
        _miso = miso;
        _sck = sck;
        this-> begin();
        return true;
    } 

    return false;
   
}

/* NOT IMPLEMENTED YET. DEPRECATED */
//void HardSPI::setClockDivider(uint8_t div) {
//}

/*
*   Base SPI class
*
*/



bool Base_SPI::isSPIpins(uint8_t mosi, uint8_t miso, uint8_t sck) {

    return false;
   
}

void Base_SPI::SPI_Settings(uint32_t clock, uint16_t bitOrder, uint8_t dataMode) {
	_clock = clock;
	_bitOrder = bitOrder;
	_dataMode = dataMode;	
}

void Base_SPI::SPI_Settings(SPISettings settings) {
	
    this-> SPI_Settings(settings.clock, settings.bitOrder, settings.dataMode);
    
}

void Base_SPI::setBitOrder(uint8_t order) {
     _bitOrder = order & 1;
}

void Base_SPI::setDataMode(uint8_t mode) {
    
    if ( ! VALID_SPI_MODE(mode)) return;   // invalid mode
    if (_dataMode == mode) return;      // mode unchanged
    
    _dataMode = mode;
   
}

inline uint8_t Base_SPI::reverseByte(uint8_t b) {
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

/*
*   SoftSPI SPI class
*
*/

SoftSPI::SoftSPI(uint8_t mosi, uint8_t miso, uint8_t sck) {
    _mosi = mosi;
    _miso = miso;
    _sck = sck;
    
}

void SoftSPI::begin() {
	set_loop(false);

    switch (_dataMode) {
        case SPI_MODE0:
            _ckp = 0;
            _cke = 0;
            break;
        case SPI_MODE1:
            _ckp = 0;
            _cke = 1;
            break;
        case SPI_MODE2:
            _ckp = 1;
            _cke = 0;
            break;
        case SPI_MODE3:
            _ckp = 1;
            _cke = 1;
            break;
    }
    
    pinMode(_mosi, OUTPUT);
    pinMode(_miso, INPUT);
    pinMode(_sck, OUTPUT);

    digitalWrite(_sck, _ckp ? HIGH : LOW);

    switch (_clock_div) {
        case SPI_CLOCK_DIV2:
            _delay = 2;
            break;
        case SPI_CLOCK_DIV4:
            _delay = 4;
            break;
        case SPI_CLOCK_DIV8:
            _delay = 8;
            break;
        case SPI_CLOCK_DIV16:
            _delay = 16;
            break;
        case SPI_CLOCK_DIV32:
            _delay = 32;
            break;
        case SPI_CLOCK_DIV64:
            _delay = 64;
            break;
        case SPI_CLOCK_DIV128:
            _delay = 128;
            break;
        default:
            _delay = 2;
            break;
    }
}

void SoftSPI::end() {
	set_loop(false);
    pinMode(_mosi, INPUT);
    pinMode(_miso, INPUT);
    pinMode(_sck, INPUT);
}

void SoftSPI::setClockDivider(uint8_t div) {
    if (div >= SPI_CLOCK_DIV2 && div <= SPI_CLOCK_DIV128) {
        _clock_div = div;
    }
}

void SoftSPI::wait(uint_fast8_t del) {
    for (uint_fast8_t i = 0; i < del; i++) {
        asm volatile("nop");
    }
}

uint8_t SoftSPI::transfer(uint8_t val) {
    uint8_t out = 0;
   /* if (_order == MSBFIRST) {
        uint8_t v2 = 
            ((val & 0x01) << 7) |
            ((val & 0x02) << 5) |
            ((val & 0x04) << 3) |
            ((val & 0x08) << 1) |
            ((val & 0x10) >> 1) |
            ((val & 0x20) >> 3) |
            ((val & 0x40) >> 5) |
            ((val & 0x80) >> 7);
        val = v2;
    } */

    uint8_t del = _delay >> 1;

    uint8_t bval = 0;
    /*
     * CPOL := 0, CPHA := 0 => INIT = 0, PRE = Z|0, MID = 1, POST =  0
     * CPOL := 1, CPHA := 0 => INIT = 1, PRE = Z|1, MID = 0, POST =  1
     * CPOL := 0, CPHA := 1 => INIT = 0, PRE =  1 , MID = 0, POST = Z|0
     * CPOL := 1, CPHA := 1 => INIT = 1, PRE =  0 , MID = 1, POST = Z|1
     */

    int sck = (_ckp) ? HIGH : LOW;

    for (uint8_t bit = 0u; bit < 8u; bit++)
    {
        if (_cke) {
            sck ^= 1;
            digitalWrite(_sck, sck);            
            wait(del);
        }

        /* ... Write bit */
        if (_bitOrder == MSBFIRST) {
            
            digitalWrite(_mosi, ((val & (1<<(7-bit))) ? HIGH : LOW));
        }
        else {
            digitalWrite(_mosi, ((val & (1<<bit)) ? HIGH : LOW));
        }

        wait(del);

        sck ^= 1u; 
        digitalWrite(_sck, sck);

        /* ... Read bit */
        {
            bval = digitalRead(_miso);

            if (_bitOrder == MSBFIRST)  {
                out <<= 1;
                out |= bval;
            } else {
                out >>= 1;
                out |= bval << 7;
            }
        }

        wait(del);

        if (!_cke) {
            sck ^= 1u;
            digitalWrite(_sck, sck);
        }
    }

    return out;
}

uint16_t SoftSPI::transfer16(uint16_t data)
{
	union {
		uint16_t val;
		struct {
			uint8_t lsb;
			uint8_t msb;
		};
	} in, out;
  
	in.val = data;

	if ( _dataOrder == SPI_BIGENDIAN ) {
		out.msb = transfer(in.msb);
		out.lsb = transfer(in.lsb);
	} else {
		out.lsb = transfer(in.lsb);
		out.msb = transfer(in.msb);
	}

	return out.val;
}
