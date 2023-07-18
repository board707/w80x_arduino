#include "Arduino.h"

/*
 * Read/Write external i2c EEPROM chip
 * i.e. 24с02
 */
#define ADRS_BYTES 2  // Address length in bytes, i.e. 1-24с02,  2-24с1024

I2C_HandleTypeDef hi2c;

void I2C_Init(void);
void i2c_eeprom_write_byte( int deviceaddress, unsigned int eeaddress, char data ); 
uint8_t i2c_eeprom_read_byte( int deviceaddress, unsigned int eeaddress );

#define DEVICE_ADDR 0x50 

void setup() {
  Serial.begin(115200); 
  char w_buf[] = "AT24CXX I2C WRITE THEN READ TEST";  // Data for write
  uint8_t len = strlen(w_buf);

  I2C_Init();
  
  // Writing
  if (ADRS_BYTES == 2) {
    for (int i=0; i < len; i++) i2c_eeprom_write_byte(DEVICE_ADDR,i,w_buf[i]);
  } else {
    for (int i = 0; i < len; i++) HAL_I2C_Write(&hi2c, (DEVICE_ADDR << 1), i, (uint8_t *)(w_buf + i), 1);
  }
  uint8_t b;
  
  // Reading
  if (ADRS_BYTES == 2) {
    for (int i=0; i < len; i++) {
        b = i2c_eeprom_read_byte(DEVICE_ADDR, i); 
      Serial.printf("%c",(char)b); 
    } 
    Serial.printf("\r\n"); 
  } else {
    for (int i=0; i < len; i++) {
      HAL_I2C_Read(&hi2c, (DEVICE_ADDR << 1), i, &b, 1);
      Serial.printf("%c",(char)b); 
    }
    Serial.printf("\r\n"); 
  }

}

void loop() {

}

uint8_t i2c_eeprom_read_byte( int deviceaddress, unsigned int eeaddress )
{
    HAL_I2C_Write_Byte(&hi2c, ((deviceaddress << 1) & 0xFE), 1);
    if (HAL_I2C_Wait_Ack(&hi2c) != HAL_OK) goto OUT;
    HAL_I2C_Write_Byte(&hi2c, (int)(eeaddress >> 8), 0);
    if (HAL_I2C_Wait_Ack(&hi2c) != HAL_OK) goto OUT;
    HAL_I2C_Write_Byte(&hi2c, (int)(eeaddress & 0xFF), 0);
    if (HAL_I2C_Wait_Ack(&hi2c) != HAL_OK) goto OUT;
    HAL_I2C_Write_Byte(&hi2c, ((deviceaddress << 1) | 0x01), 1);
    if (HAL_I2C_Wait_Ack(&hi2c) != HAL_OK) goto OUT;
  return HAL_I2C_Read_Byte(&hi2c, 0, 1);
OUT:
    HAL_I2C_Stop(&hi2c);
    return 0;
}

void i2c_eeprom_write_byte( int deviceaddress, unsigned int eeaddress, char data )
{
    HAL_I2C_Write_Byte(&hi2c, ((deviceaddress << 1) & 0xFE), 1);
    if (HAL_I2C_Wait_Ack(&hi2c) != HAL_OK) goto OUT;
    HAL_I2C_Write_Byte(&hi2c, (int)(eeaddress >> 8), 0);
    if (HAL_I2C_Wait_Ack(&hi2c) != HAL_OK) goto OUT;
    HAL_I2C_Write_Byte(&hi2c, (int)(eeaddress & 0xFF), 0);
    if (HAL_I2C_Wait_Ack(&hi2c) != HAL_OK) goto OUT;
    HAL_I2C_Write_Byte(&hi2c, data, 0);
    if (HAL_I2C_Wait_Ack(&hi2c) != HAL_OK) goto OUT;
OUT:
    HAL_I2C_Stop(&hi2c);
  delay(3);
}

void I2C_Init(void)
{
  __HAL_RCC_I2C_CLK_ENABLE();                   // Turn on i2c clock
  __HAL_AFIO_REMAP_I2C_SCL(GPIOA, GPIO_PIN_1);  // Pin remap
  __HAL_AFIO_REMAP_I2C_SDA(GPIOA, GPIO_PIN_4);  // to i2c
  hi2c.Instance = I2C;
  hi2c.Frequency = 100000;            // i2c clock frequency (up to 1 MHz)
  HAL_I2C_Init(&hi2c);
}