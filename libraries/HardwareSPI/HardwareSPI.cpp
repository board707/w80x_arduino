#include "HardwareSPI.h"


HardSPI::HardSPI(uint8_t mosi, uint8_t miso, uint8_t sck) {
    _mosi = mosi;
    _miso = miso;
    _sck = sck;
}
void HardSPI::beginTransaction(SPISettings settings) {
	_clock = settings.clock;
	_bitOrder = settings.bitOrder;
	_dataMode = settings.dataMode;
	begin();
}
void HardSPI::SPI_Settings(uint32_t clock, uint16_t bitOrder, uint8_t dataMode) {
	_clock = clock;
	_bitOrder = bitOrder;
	_dataMode = dataMode;
	
}
void HardSPI::begin() {

    switch(_dataMode)
    {
    case 0:
        clock_polarity = SPI_POLARITY_LOW;
        clock_phase = SPI_PHASE_1EDGE;
        break;
    case 1:
        clock_polarity = SPI_POLARITY_LOW;
        clock_phase = SPI_PHASE_2EDGE;
        break;
    case 2:
        clock_polarity = SPI_POLARITY_HIGH;
        clock_phase = SPI_PHASE_1EDGE;
        break;
    case 3:
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
	if (HAL_SPI_Init(&hspi) != HAL_OK) printf("Что то пошло не так... \n\r");
}

void HardSPI::end() {
   
	HAL_GPIO_DeInit(pin_Map[_sck].pPort, pin_Map[_sck].halPin);
	HAL_GPIO_DeInit(pin_Map[_miso].pPort, pin_Map[_miso].halPin);
	HAL_GPIO_DeInit(pin_Map[_mosi].pPort, pin_Map[_mosi].halPin);
	HAL_SPI_DeInit(&hspi);
    __HAL_RCC_SPI_CLK_DISABLE();
	
}
uint8_t HardSPI::transfer(uint8_t val) {
    uint8_t recv;
    HAL_SPI_TransmitReceive(&hspi, &val, &recv, 1, timeOut);
    return recv;
}
uint16_t HardSPI::transfer16(uint16_t data)
{
  union
    {
         uint8_t _8recv[2];
         uint16_t recv;
      /* data */
    }in, out;

    in.recv = data;
    HAL_SPI_TransmitReceive(&hspi, in._8recv, out._8recv, 2, timeOut);

    return out.recv;
}
void HardSPI::transfer(void *buf, size_t count) {
    if (count == 0) return;
    HAL_SPI_Transmit(&hspi, (uint8_t *)buf, count, timeOut);
}

/* NOT IMPLEMENTED YET. DEPRECATED */

void HardSPI::setBitOrder(uint8_t order) {
}

void HardSPI::setDataMode(uint8_t mode) {
}

void HardSPI::setClockDivider(uint8_t div) {
}
