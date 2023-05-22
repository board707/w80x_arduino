#include "Arduino.h"
#include "Wire.h"

#define TinyRTC_EEPROM  0x50

void i2c_eeprom_write_page( int deviceaddress, unsigned int eeaddresspage, char* data, char length ) {
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddresspage >> 8)); // MSB
  Wire.write((int)(eeaddresspage & 0xFF)); // LSB
  uint8_t c;
  for ( c = 0; c < length; c++)
    Wire.write(data[c]);
  delayMicroseconds(10); // AT24C32 не успевает ответить
  Wire.endTransmission();
}

char i2c_eeprom_read_byte( int deviceaddress, unsigned int eeaddress ) {
  char rdata = 0xFF;
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  delayMicroseconds(10); // AT24C32 не успевает ответить
  Wire.requestFrom(deviceaddress, 1);
  if (Wire.available()) rdata = Wire.read();
  return rdata;
}

void setup()
{
  char somedata[] = "This is data from the eeprom"; // data to write

  Wire.begin();               // Hardware I2C  (PA4 = SDA, PA1 = SCL by default)
  //Wire.begin(PA12,PA14);    // Software I2C  (SDA, SCL) - any pins

  i2c_eeprom_write_page(TinyRTC_EEPROM, 0, (char *)somedata, sizeof(somedata)); // write to EEPROM
  Serial.begin(115200);
}

void loop()
{
  int addr = 0; //first address
  char b = i2c_eeprom_read_byte(TinyRTC_EEPROM, 0); // access the first address from the memory

  while (b != 0)
  {
    Serial.printf("%c", (char)b); //print content to serial port
    addr++; //increase address
    b = i2c_eeprom_read_byte(TinyRTC_EEPROM, addr); //access an address from the memory

  }
  Serial.printf(" \n\r");
  delay(2000);
}