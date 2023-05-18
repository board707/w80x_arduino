## **HardwareSerial Library for W80x boards**

**Written by:** _Dmitry E. Dmitriev_ .

### **What is the HardwareTimer library.**

The HardwareSerial library provides an easy to use interface to interact with the UART channels of WM MCUs. 

Winner Micro devices and clones such as W806, W801, Air103... etc has as much as six high-speed hardware UARTs. The one, UART0, is connected to the board's USB-UART bridge (CH340/341) chip, and the others five are available to the user.

### **UART pins table**

| **UART** | **Default pins** (TX, RX) | **Alternative pins**   | 
|:---------|:----------------:|:--------------:|
| UART0 | **PB19**, **PB20**  | - | 
| UART1 | **PB6**, **PB7** | - |
| UART2 | **PB2**, **PB3** | PA2, PA3 | 
| UART3 | **PB0**, **PB1** | PA5, PA6 | 
| UART4 | **PB4**, **PB5** | PA8, PA9 |
| UART5 | **PA12**, **PA13** | PB18,PB17 | 

(Alternative pins selection is not supported yet.)

### **How to use it**
To add its functionality to your sketch you'll need to initialize HardwareSerial object with number of used MCU UART (1..5). The UART0 `Serial` instance is defined by system and shouldn't be defined by user.

```Arduino
HardwareSerial Serial2(2); 

void setup(){
Serial2.begin(9600, SERIAL_8N1);
}

void loop(){

}

```

For uarts 2-5 you can change the default TX RX pins to the alternative option by calling the HardwareSerial constructor with number of UART and keyword `UART_ALT_TX_RX` as second parameter:

##### **`HardwareSerial Serial5(5, UART_ALT_TX_RX);`** 


### **Configure**

Before using the uart, it must be configured. Using begin() method you can set port baudrate, number of stopbits and mode of parity control:

#### **`Serial.begin(baudrate)`**
#### **`Serial.begin(baudrate, options)`**

The first parameter `baudrate` can be varied from 600 to 2000000 baud. For UART control options you can use defines below:

 - **SERIAL_8N1**	- 8 bits, 1 stopbit, parity none
 - **SERIAL_8N2**	- 8 bits, 2 stopbits, parity none
 - **SERIAL_8E1**	- 8 bits, 1 stopbit, parity even
 - **SERIAL_8E2**	- 8 bits, 2 stopbits, parity even
 - **SERIAL_8O1**	- 8 bits, 1 stopbit, parity odd
 - **SERIAL_8O2**	- 8 bits, 2 stopbits, parity odd

If `options` parameter is omitted, default `SERIAL_8N1` will be used.

This function doesn't have a return value.

### **Library functions**

Most HardwareSerial methods such as `print()`, `println()`, `write()`, `peek()`, `available()` and `read()` are similar to the ones of the Arduino standard Serial class. For more info please consult the Arduino manual. The only one that have been added to the current library is

#### **`Serial.printf(const char *fmt,...)`**

The function sends formatted output to the serial port. It uses standard C++ `printf` style format to control the output. Please consult C++ `printf` manual for detail.

If successful, the function returns the total number of characters printed. On failure, a negative number is returned.