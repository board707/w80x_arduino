## **HardwareTimer Library for W80x boards**

**Originally written by:** _Rodger Clark_ (for stm32).

This copy is slightly modified, for use with Winner Micro boards.

### **What is the HardwareTimer library.**

The HardwareTimer library provides an easy to use interface to interact with the MCU Timers. This library will work on many Winner Micro devices and clones such as W806, W801, Air103.

### **How to use it**
To add its functionality to your sketch you'll need to reference the library header file and initialize HardwareTimer object with number of used MCU timer (0..5)

```Arduino
#include <HardwareTimer.h>
HardwareTimer MyTim(2); 

void setup(){

}

void loop(){

}

```

### **Configure timer**

Before using the timer, it must be configured. Using configure() method you can set timer period, timer counter units and pointer to timer irq handler function.

#### **`MyTim.configure(period, units, reload_flag, irq_function)`**
#### **`MyTim.configure(period, units, reload_flag)`**

First and second parameters `period` and `units` define duration of one timer cycle in milli- or microseconds. The `period` is 32bit integer and `units` can be one of two: `TIM_UNIT_MS` or `TIM_UNIT_US`. 
The `reload_flag` determines whether the timer will automatically restart after overflow or the loop will be executed only once. Valid parameter values are:
`TIM_AUTORELOAD_PRELOAD_ENABLE` or `TIM_AUTORELOAD_PRELOAD_DISABLE`. 
The last is a pointer to user function that will be called when the period expires. The irq function pointer can be omitted and attached later with `attachInterrupt()` method.

This function returns zero if no errors occured in configuration or non-zero errorcode otherwise.


### **Library functions**


#### **`MyTim.start()`**

Start the timer with current configuration. Timer counter resets to zero. If timer is already running, the function will do nothing. 

This function doesn't have a return value.

#### **`MyTim.stop()`**

Stop the running timer. The counter is reset to zero immediately after stopping and is not available after that.

This function doesn't have a return value.

#### **`MyTim.attachInterrupt( callback )`**

This function will attach a pointer to user defined function `callback` to the timer interrupt. The `callback` function will be called when the timer period expired. If the timer had a different callback already, it will be replaced.
Note, that using `attachInterrupt()` function on running timer will stop it. The timer than should be started again by `start()` method. 

This function doesn't have a return value.

#### **`MyTim.detachInterrupt( )`**

Remove the pointer to timer interrupt function(set it to NULL). 
Note, that using `detachInterrupt()` function on running timer will stop it. The timer than should be started again by `start()` method. 

This function doesn't have a return value.

#### **`MyTim.setPeriod( period )`**

The function sets the duration of timer cycle to the value of the `period` parameter.

This function doesn't have a return value.

### **Parameter getters**

#### **`MyTim.getPeriod( )`** 

The function returns a `uint32_t` value of the timer period in milli- or microseconds, depending on timer units.

#### **`MyTim.getCount( )`** 

On running timer the function returns a `uint32_t` value of the timer counter. When the timer stopped, returns 0.

#### **`MyTim.getTimerUnits( )`** 

This function will retrieve the units of the timer period value. The two options are `TIM_UNIT_MS` or `TIM_UNIT_US`. 

#### **`MyTim.getTimerReloadFlag( )`** 

This function will retrieve the value of the timer reload flag. The two options are `TIM_AUTORELOAD_PRELOAD_ENABLE` or `TIM_AUTORELOAD_PRELOAD_DISABLE`. 

#### **`MyTim.c_dev( )`** 

This function will retrieve the pointer to HAL timer device handler.

