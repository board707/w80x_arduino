#include <Arduino.h>
#include <HardwareTimer.h>

// initialize a new HardwareTimer, using mcu timer 2.  
// You can use up to six timers with numbers 0..5.
HardwareTimer MyTim(2);     
static volatile uint8_t key_flag = 0;

// Timer irq handler function
void tim_irq_routine() {
    
    
    key_flag++;
    if (key_flag % 3 == 0)
    {
        digitalWrite(LED_BUILTIN_1, LOW);
        digitalWrite(LED_BUILTIN_2, HIGH);
        digitalWrite(LED_BUILTIN_3, HIGH);      
      
    }
    else if (key_flag % 3 == 1)
    {
        digitalWrite(LED_BUILTIN_1, HIGH);
        digitalWrite(LED_BUILTIN_2, LOW);
        digitalWrite(LED_BUILTIN_3, HIGH);    
    }
    else
    {
        digitalWrite(LED_BUILTIN_1, HIGH);
        digitalWrite(LED_BUILTIN_2, HIGH);
        digitalWrite(LED_BUILTIN_3, LOW);    
    }
}


void setup() {
// Led pins init
pinMode(LED_BUILTIN_1, OUTPUT);
pinMode(LED_BUILTIN_2, OUTPUT);
pinMode(LED_BUILTIN_3, OUTPUT);

// Configure the timer with period 1000 milliseconds, auto reload mode
MyTim.configure(1000, TIM_UNIT_MS, TIM_AUTORELOAD_PRELOAD_ENABLE); 
// Enable timer interrupt and attach the handler
MyTim.attachInterrupt( (timer_irq_callback) &tim_irq_routine);
// start the timer
MyTim.start(); 
}


void loop() {
  // put your main code here, to run repeatedly:


}