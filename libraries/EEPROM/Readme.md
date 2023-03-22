## **EEPROM Library for W80x boards

**Originally written by:** _Paul Stoffregen_.

This copy is slightly modified, for use with Winner Micro boards.

### **What is the EEPROM library.**

The EEPROM library provides an easy to use interface to interact with the internal non-volatile flash storage. This library will work on many Winner Micro devices and clones such as W806, W801, Air103.

### **How to use it**
To add its functionality to your sketch you'll need to reference the library header file and initialize EEPROMClass object.

```Arduino
#include <EEPROM.h>
EEPROMClass W_EEPROM;

void setup(){

}

void loop(){

}

```
Default size of the internal flash sorage is 2048 bytes, but you can choose different value by defining an EEPROMClass object with parameter:

#### **`EEPROMClass W_EEPROM( size )`**

The methods provided in the EEPROMClass are similar with ones, available in standard EEPROM library for AVR boards.


### **Library functions**

#### **`W_EEPROM.read( address )`**

This function allows you to read a single byte of data from the eeprom.
Its only parameter is an `int` which should be set to the address you wish to read.

The function returns an `unsigned char` containing the value read.

#### **`W_EEPROM.write( address, value )`** 

The `write()` method allows you to write a single byte of data to the EEPROM.
Two parameters are needed. The first is an `int` containing the address that is to be written, and the second is a the data to be written (`unsigned char`).

This function returns true after succesfull write and false otherwise.

#### **`W_EEPROM.update( address, value )`** 

This function is similar to `W_EEPROM.write()` however this method will only write data if the cell contents pointed to by `address` is different to `value`. This method can help prevent unnecessary wear on the EEPROM cells.

Return value of the function is similar to `W_EEPROM.write()`.

#### **`W_EEPROM.get( address, object )`** 

This function will retrieve any object from the EEPROM.
Two parameters are needed to call this function. The first is an `int` containing the address that is to be written, and the second is the object you would like to read.

This function returns a reference to the `object` passed in. It does not need to be used and is only returned for conveience.

#### **`W_EEPROM.put( address, object )`** 

This function will write any object to the EEPROM.
Two parameters are needed to call this function. The first is an `int` containing the address that is to be written, and the second is the object you would like to write.

This function **does not use the _update_ method** to write its data.

This function returns a reference to the `object` passed in. It does not need to be used and is only returned for conveience.

#### **`W_EEPROM.length()`**

This function returns an `unsigned int` containing the number of cells in the EEPROM.

### NOT IMPLEMENTED YET!!!

#### **Subscript operator: `W_EEPROM[address]`** 

This operator allows using the identifier `W_EEPROM` like an array.  
EEPROM cells can be read _and_ **_written_** directly using this method.

This operator returns a reference to the EEPROM cell.

```c++
unsigned char val;

//Read first EEPROM cell.
val = W_EEPROM[ 0 ];

//Write first EEPROM cell.
W_EEPROM[ 0 ] = val;

//Compare contents
if( val == W_EEPROM[ 0 ] ){
  //Do something...
}
```
