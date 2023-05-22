#include <Arduino.h>
#include "HardwareI2C.h"

HardwareI2C::HardwareI2C() {}
HardwareI2C::~HardwareI2C() {}

void HardwareI2C::begin() {
	
  __HAL_RCC_I2C_CLK_ENABLE();           		// Тактирование i2c контроллера
  __HAL_AFIO_REMAP_I2C_SCL(GPIOA, GPIO_PIN_1);  // Переключение выводов
  __HAL_AFIO_REMAP_I2C_SDA(GPIOA, GPIO_PIN_4);  // на I2C контроллер
  hi2c.Instance = I2C;
  hi2c.Frequency = 100000;            			// Частота тактирования по умолчанию 
  HAL_I2C_Init(&hi2c);

}
void HardwareI2C::setClock(int clockFrequency) {

  hi2c.Frequency = clockFrequency;            	
  HAL_I2C_Init(&hi2c);
	
}
uchar HardwareI2C::beginTransmission(uchar addr) {

  HAL_I2C_Write_Byte(&hi2c, ((addr << 1) & 0xFE), 1); // Предваряя START сигнал
  if (HAL_I2C_Wait_Ack(&hi2c) != HAL_OK) 
	{HAL_I2C_Stop(&hi2c);
	return GETNAK;}
  else
	return GETACK;
}
uchar HardwareI2C::endTransmission() {

	HAL_I2C_Stop(&hi2c);
	return 0;
}
uchar HardwareI2C::write(uchar dta) {
	HAL_I2C_Write_Byte(&hi2c, dta, 0);	// Без сигнала START
    if (HAL_I2C_Wait_Ack(&hi2c) != HAL_OK)
		{HAL_I2C_Stop(&hi2c); 
		return GETNAK;}
	else
		return GETACK;
}
uchar HardwareI2C::write(uint16_t len, uchar* dta) {
    for (uint16_t i = 0; i < len; i++) {
		HAL_I2C_Write_Byte(&hi2c, dta[i], 0);	// Без сигнала START
		if (HAL_I2C_Wait_Ack(&hi2c) != HAL_OK) 
		{	HAL_I2C_Stop(&hi2c); 
			return GETNAK;
		}
    }
	return GETACK;
}
uchar HardwareI2C::requestFrom(uchar addr, uint16_t len) {
	
	recv_len = len;
	HAL_I2C_Write_Byte(&hi2c, ((addr << 1) | 0x01), 1); //Режим чтения
    if (HAL_I2C_Wait_Ack(&hi2c) != HAL_OK) 
		{HAL_I2C_Stop(&hi2c); 
		recv_len = 0;
		return GETNAK;}
	else
		return GETACK;

}
uchar HardwareI2C::read() {
	uchar dta;
    if (recv_len <= 0) {
        return 0;
    }
	recv_len--;	
	if (recv_len > 0) dta = HAL_I2C_Read_Byte(&hi2c, 1, 0); // Отправляем ACK без сигнала STOP
	else dta = HAL_I2C_Read_Byte(&hi2c, 0, 1);				// Последний принимаемый байт - Отправляем NAK и сигнал STOP
	return dta;	
}