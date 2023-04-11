#include "Arduino.h"
#include "HardwareI2C.h"

HardwareI2C Wire;

void i2c_eeprom_write_byte( int deviceaddress, unsigned int eeaddress, char data ) {
    int rdata = data;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.write(rdata);
    Wire.endTransmission();
}

void i2c_eeprom_write_page( int deviceaddress, unsigned int eeaddresspage, char* data, char length ) {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddresspage >> 8)); // MSB
    Wire.write((int)(eeaddresspage & 0xFF)); // LSB
    uint8_t c;
    for ( c = 0; c < length; c++)
        Wire.write(data[c]);
    Wire.endTransmission();
}

char i2c_eeprom_read_byte( int deviceaddress, unsigned int eeaddress ) {
    char rdata = 0xFF;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
	delayMicroseconds(10); // AT24C32 не успевает ответить
    Wire.requestFrom(deviceaddress,1);
    if (Wire.available()) rdata = Wire.read();
    return rdata;
}

void i2c_eeprom_read_buffer( int deviceaddress, unsigned int eeaddress, char *buffer, int length ) {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(deviceaddress,length);
    int c = 0;
    for ( c = 0; c < length; c++ )
        if (Wire.available()) buffer[c] = Wire.read();
}




void setup()
{
    char somedata[] = "this is data from the eeprom"; // data to write
    Wire.begin(); //   initialise the connection
    i2c_eeprom_write_page(0x50, 0, (char *)somedata, sizeof(somedata)); // write to EEPROM

    delay(100); //add a small delay

}

void loop()
{
    int addr=0; //first address
    char b = i2c_eeprom_read_byte(0x50, 0); // access the first address from the memory

    while (b!=0)
    {
        printf("%c",(char)b); //print content to serial port
        addr++; //increase address
        b = i2c_eeprom_read_byte(0x50, addr); //access an address from the memory
    }
    printf(" \n\r");
    delay(2000);
}