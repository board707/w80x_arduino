## PWM controller

The W80x controller family supports up to five independent PWM generation channels. PWM output frequency range is 3Hz~160kHz (default 19.5 KHz), with 8bit resolution (0-255). Each PWM channel can be attached to several output pins, but only one pin per channel can be used at the time. Availiable output pins can be varied, see the pinout for defined board.

### **How to use**
To add PWM functionality to your sketch you'll need to initialize a pwm-capable pin in `PWM_OUT` mode:

#### **`pinMode(PB12, PWM_OUT)`**

Than you will able to control a PWM output by usual arduino way:

#### **`analogWrite(PB12, duty)`**

where `duty` should be in range 0-255.

### **Change PWM frequency**

Frequency of PWM signal can be changed for each PWM output individually by `setPWMFreq()` method:

#### **`uint32_t setPWMFreq(uint8_t pin, uint32_t pwmFreq)`**

Note that method must be used after pin initialisation as pwm by `pinMode()`. The first parameter is pwm pin, the second - desired pwm frequency in hertz. Due to limitations in timer settings, not every frequency can be used for a PWM signal. The method tunes the output to the nearest available frequency. The function returns the selected frequency in success or zero in case the pin is uninitialized or cannot be used as pwm.

Calling the `pinMode()` method again returns the pwm frequency to its default value.


### Documentation 

For a detailed overview of the extended PWM controller features see section 23 of the datasheet for Winner Micro W800 serie microcontrollers. 