// *** Gpio interrupt example ***

volatile uint8_t key_flag = 0;


// user irq function
void _irq_routine() {
  key_flag = 1;
}


void setup() {
  pinMode(LED_BUILTIN_1, OUTPUT);
  pinMode(PB10, INPUT_PULLUP);

/* attach interrupt to pin, with mode, using callback
* available modes:
*  RISING     or  GPIO_MODE_IT_RISING             
*  FALLING    or  GPIO_MODE_IT_FALLING           
*  CHANGE     or  GPIO_MODE_IT_RISING_FALLING     
*  HIGH_LEVEL or  GPIO_MODE_IT_HIGH_LEVEL         
*  LOW_LEVEL  or  GPIO_MODE_IT_LOW_LEVEL  */
        
  attachInterrupt(PB10, FALLING, &_irq_routine);

// Also available
// detachInterrupt(pin);  
}

void loop() {
  if (key_flag == 1) {
    delay(200); // debounce
    digitalToggle(LED_BUILTIN_1);
    key_flag = 0;
  }
}
