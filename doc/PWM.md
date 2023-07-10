## PWM controller

The W80x controller family supports up to five independent PWM generation channels. PWM output frequency range is 3Hz~160kHz (default 19.5 KHz), with 8bit resolution (0-255). Each PWM channel can be attached to several output pins, but only one pin per channel can be used at the time. Availiable output pins can be varied, see the pinout for defined board.

### **Basic usage**
To add PWM functionality to your sketch you'll need to initialize a pwm-capable pin in `PWM_OUT` mode:

##### **`pinMode(PB12, PWM_OUT)`**

Than you will able to control a PWM output by usual arduino way:

##### **`analogWrite(PB12, duty)`**

where `duty` should be in range 0-255.

### **Advanced PWM features**

>Note: all methods below must be used after pin initialisation as pwm by `pinMode()`. 

To work with more dippest PWM settings you first need to get pwm device handler:

##### **`PWM_HandleTypeDef *hpwm = getPWMHandle(PB12);`**

than you can use it to access the settings of the PWM channel.

#### **Change PWM frequency (and Period)**

Frequency of PWM signal can be changed for each PWM output individually by `setPWM_Freq()` (previously `setPWMFreq()`) method:

##### **`uint32_t setPWM_Freq(PWM_HandleTypeDef *hpwm, uint32_t pwmFreq)`**
##### **`uint32_t setPWM_Freq_Period(PWM_HandleTypeDef *hpwm, uint32_t pwmFreq, uint8_t period)`**

The first parameter is pwm handle, the second - desired pwm frequency in hertz. Due to limitations in timer settings, not every frequency can be used for a PWM signal. The method tunes the output to the nearest available frequency. The function returns the selected frequency in success or zero in case the pin is uninitialized or cannot be used as pwm.

The function `setPWM_Freq()` will setup the timer using default PWM resolution 8bit (`period` = 255) that determine the maximum frequency about 156 KHz (40 MHz / 256). With `setPWM_Freq_Period()` function you can increase the frequency by setting the period in range (1-255). Please note that after changing the PWM period, you should not use values more than `period` in `analogWrite()` function.

#### **Inverse output mode**

##### **`void setPWM_OutInverse(PWM_HandleTypeDef *hpwm, bool pwm_inverse, bool start)`**

Set output polarity for a PWM channel. Output will be inverted if parameter `pwm_inverse` is true. Default polarity mode is non-invert. The last parameter determine whether the PWM starts after changing the settings or it will be paused until a next start command.

#### **OneShot and MultiPulse modes**

By default the PWM timer is reenabled after each period (`AutoReload mode`), running until stops. There is also a `OneShot` mode, in which the timer runs for one period and stops, as well as a counter mode, in which the timer runs for a specified number of periods. In both latter cases, an interrupt can be generated at the end of the set number of periods.

##### **`void configPWM_Pulses(PWM_HandleTypeDef *hpwm, uint8_t pulse_cnt, pwm_irq_callback callback, bool start);`**

Start a PWM timer for a limited number of periods, determinated by `pulse_cnt` parameter (0-255) and attach the `callback` function to the interrupt, fired after finishing the cycle. The last parameter determine whether the PWM starts immediately or it will be paused until a next start command.

##### **`void disablePWM_Pulses(PWM_HandleTypeDef *hpwm, bool start);`**

Revert PWM channel to default `AutoReload mode`, detach the interrupt. The last parameter as in the method above.

##### **`void setPWM_PulseCounter(PWM_HandleTypeDef *hpwm, uint8_t num_cnt);`**

Set pulse counter for `OneShot` and counter modes. Note that setting non-zero pulse counter in `AutoReload mode` will cause the timer stops after defined number of periods. 

##### **`void startPWM(PWM_HandleTypeDef* hpwm);`**
##### **`void stopPWM(PWM_HandleTypeDef* hpwm);`**

Start and stop the PWM timer without changing the current setup.

### **Revert to default settings**

Calling the `pinMode()` method again returns the pwm parameters to its default values:
* Frequency - 19.5 KHz
* Period - 255 (8 bit)
* Duty - 0
* Polarity - non-invert
* AutoReload mode
* Interrupt detached
  


### Additional Documentation 

For a detailed overview of the extended PWM controller features see section 23 of the datasheet for Winner Micro W800 serie microcontrollers. 