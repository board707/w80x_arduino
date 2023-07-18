
#include <Arduino.h>
#include <EEPROM.h>

EEPROMClass W_EEPROM;           // initialize EEPROM with default size = 2048 bytes
//EEPROMClass W_EEPROM(4096);   // or you can choose different size


// sample data sctructure
struct data_storage
{
  uint32_t offset;
  uint8_t data1;
  uint16_t data2;
  float data3;
};

// sample data values
data_storage ds = {
             .offset = 200,
             .data1 = 155,
             .data2 = 0x8000,
             .data3 = 39.14f
};
data_storage ds1;
uint8_t a1 = 122;
uint8_t a2 = 255;

void setup() {

  Serial.begin(115200);
  Serial.printf("\nInit EEPROM size = %d bytes\n", W_EEPROM.length());

  // storing data
  Serial.printf("Write to EEPROM:\n");
  if (! W_EEPROM.write(0, a1))          // store single byte
     { Serial.printf("EEPROM error!\n");}

  W_EEPROM.put(10, ds);                 // store whole structure
  
  Serial.printf("DS offset = %d, data1 = %d, data2 = %d, data3 = %f\n", ds.offset, ds.data1, ds.data2, ds.data3);
  delay(500);

  // reading data
  Serial.printf("\nRead from EEPROM:\n");
  a2 = W_EEPROM.read(0);              // read single byte
  Serial.printf("Byte a2 = %d\n", a2);
  W_EEPROM.get(10, ds1);              // read structure
  Serial.printf("DS1 offset = %d, data1 = %d, data2 = %d, data3 = %f\n", ds1.offset, ds1.data1, ds1.data2, ds1.data3);

}

void loop() {
  // put your main code here, to run repeatedly:

}