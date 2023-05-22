#include "Arduino.h"
#include "Wire.h"

uint8_t dev_i2c[128] = {0};

void setup()
{
  Wire.begin();               // Hardware I2C  (PA4 = SDA, PA1 = SCL by default)
  //Wire.begin(PA12,PA14);    // Software I2C  (SDA, SCL) - any pins
  Serial.begin(115200);       // Arduino Serial communication. Recommended speed for w80x
  Serial.println("I2C Scanner");
}


void loop()
{
  uint8_t error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    delayMicroseconds(10); //Timeout must be!
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      dev_i2c[nDevices] = address;
      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknown error at address [0x");
      Serial.print(address, HEX);
      Serial.println("]");
    }
  }
  if (nDevices == 0) Serial.println("No I2C devices found.");
  else {
    Serial.println("Scan complete...");
    for (address = 0; address < nDevices; address++) {
      Serial.print("Found i2c device at address [0x");
      Serial.print(dev_i2c[address], HEX);
      Serial.println("]");
    }
    Serial.println("done...");
  }
  delay(5000);	// wait 5 seconds for next scan
}