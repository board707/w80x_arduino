#include "Arduino.h"
#include "wm_hal.h"
//#include "pins_arduino.h"

// Массив таймеров
TIM_HandleTypeDef htim0;
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim5;



// Массив для фиксации занятых каналов PWM
typedef struct {
    uint8_t channel;
    uint8_t pin;
    PWM_HandleTypeDef hpwm;
	bool isPresent;
} pwmPinStateTD;
pwmPinStateTD pwmPinState[PWM_COUNT] = { 0 };
//PWM_HandleTypeDef pwm[PWM_COUNT] = { 0 };

// Массив для фиксации занятых каналов ADC
typedef struct
{
    uint32_t channel;
    uint8_t pin;
    ADC_HandleTypeDef hadc;
	bool isPresent;
} adcPinStateTD;
adcPinStateTD adcPinState[ADC_COUNT] = {0};
//ADC_HandleTypeDef adc[ADC_COUNT] = { 0 };

// Прототипы функций
void PWM_Init(PWM_HandleTypeDef* hpwm, uint32_t channel);
void ADC_Init(ADC_HandleTypeDef* hadc, uint32_t channel);
bool check_pin(void);

// Задержка в микросекундах
void delayMicroseconds(uint32_t us) {
    HAL_Delay(us);
}
// Задержка в миллисекундах
void delay(uint32_t ms) {
	delayMicroseconds(ms*1000);
}
// Время upTime в микросекундах
uint32_t micros() {
	return HAL_GetTick();
}
// Время upTime в миллисекундах
uint32_t millis() {
	return HAL_GetTick() / 1000;
}

// Установка режимов ввода/вывода
void pinMode(uint8_t pin, uint8_t mode)
{
    uint8_t pwm_channel = 0;
	uint32_t adc_channel = 0;
	uint8_t adc_index = 0;
	
    //if(!is_loop()) {
		if(pin_Map[pin].ulPinAttribute != NONE) {
			if (check_pin()) {
				if(pin_Map[pin].ulPinAttribute & PIN_DIO_Msk) {
					if ((mode == INPUT) || (mode == OUTPUT) || (mode == INPUT_PULLUP) || (mode == INPUT_PULLDOWN))
					{
						GPIO_InitTypeDef GPIO_InitStruct = {0};
						GPIO_InitStruct.Pin = pin_Map[pin].halPin;
						switch (mode)
						{
						case INPUT:
							GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
							GPIO_InitStruct.Pull = GPIO_NOPULL;
							break;
						case INPUT_PULLUP:
							GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
							GPIO_InitStruct.Pull = GPIO_PULLUP;
							break;
						case INPUT_PULLDOWN:
							GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
							GPIO_InitStruct.Pull = GPIO_PULLDOWN;
							break;
						case OUTPUT:
							GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT;
							GPIO_InitStruct.Pull = GPIO_NOPULL;
							break;
						default:
							break;
						}
						if (!is_gpio_clk_en)
						{
							__HAL_RCC_GPIO_CLK_ENABLE();
							is_gpio_clk_en = true;
						}
						HAL_GPIO_Init(pin_Map[pin].pPort, &GPIO_InitStruct);
					}
				}
				if(mode == ANALOG_INPUT) {
					if(pin_Map[pin].ulPinAttribute & PIN_ADC_Msk) {
						switch(pin_Map[pin].ulPinAttribute & PIN_ADC_Msk) {
							case ADC1:
								adc_channel = ADC_CHANNEL_0;
								adc_index = 0;
								__HAL_AFIO_REMAP_ADC(GPIOA, GPIO_PIN_1);
								break;
							case ADC2:
								adc_channel = ADC_CHANNEL_1;
								adc_index = 1;
								__HAL_AFIO_REMAP_ADC(GPIOA, GPIO_PIN_4);
								break;
							case ADC3:
								adc_channel = ADC_CHANNEL_2;
								adc_index = 2;
								__HAL_AFIO_REMAP_ADC(GPIOA, GPIO_PIN_3);
								break;
							case ADC4:
								adc_channel = ADC_CHANNEL_3;
								adc_index = 3;
								__HAL_AFIO_REMAP_ADC(GPIOA, GPIO_PIN_2);
								break;
							default:
								break;
						}
						if(!is_adc_clk_en) {
							__HAL_RCC_ADC_CLK_ENABLE();
							is_adc_clk_en = true;
						}
						adcPinState[adc_index].channel = adc_channel;
						adcPinState[adc_index].pin = pin;
						//adcPinState[adc_index].hadc = adc[adc_index];
						ADC_Init(&(adcPinState[adc_index].hadc), adc_channel);
						adcPinState[adc_index].isPresent = true;
					} else {
					printf("Вывод %d не работает в АЦП режиме! \n\r", pin);}
				}
				if(mode == PWM_OUT) {
					if(pin_Map[pin].ulPinAttribute & PIN_PWM_Msk) {
						switch(pin_Map[pin].ulPinAttribute & PIN_PWM_Msk) {
							case PWM0:
								pwm_channel = PWM_CHANNEL_0;
								__HAL_AFIO_REMAP_PWM0(pin_Map[pin].pPort, pin_Map[pin].halPin);
								break;
							case PWM1:
								pwm_channel = PWM_CHANNEL_1;
								__HAL_AFIO_REMAP_PWM1(pin_Map[pin].pPort, pin_Map[pin].halPin);
								break;
							case PWM2:
								pwm_channel = PWM_CHANNEL_2;
								__HAL_AFIO_REMAP_PWM2(pin_Map[pin].pPort, pin_Map[pin].halPin);
								break;
							case PWM3:
								pwm_channel = PWM_CHANNEL_3;
								__HAL_AFIO_REMAP_PWM3(pin_Map[pin].pPort, pin_Map[pin].halPin);
								break;
							case PWM4:
								pwm_channel = PWM_CHANNEL_4;
								__HAL_AFIO_REMAP_PWM4(pin_Map[pin].pPort, pin_Map[pin].halPin);
								break;
							
							default:
								break;
						}
						if(!is_pwm_clk_en) {
							__HAL_RCC_PWM_CLK_ENABLE();
							is_pwm_clk_en = true;
						}
						pwmPinState[pwm_channel].channel = pwm_channel;
						pwmPinState[pwm_channel].pin = pin;
						//pwmPinState[pwm_channel].hpwm = pwm[pwm_channel];
						if (pwmPinState[pwm_channel].isPresent) {
							HAL_PWM_Stop(&(pwmPinState[pwm_channel].hpwm));
							HAL_PWM_DeInit(&(pwmPinState[pwm_channel].hpwm));
                            }
						pwmPinState[pwm_channel].isPresent = true;
						PWM_Init(&(pwmPinState[pwm_channel].hpwm), pwm_channel);
					} else {
						printf("Вывод %d не работает в ШИМ режиме! \n\r", pin);
					}
				}
			} else {
				printf("Повторное использование вывода %d не допустимо! \n\r", pin);
			}
		} else {
			printf("Вывод %d не конфигурируется! \n\r", pin);
		}	
    /*} else {
		printf("Конфигурирование выводов в Loop запрещено! \n\r");
    }*/
}

// Запись/чтение цифрового выхода/входа
void digitalWrite(uint8_t pin, uint8_t val) {
	
    HAL_GPIO_WritePin(pin_Map[pin].pPort, pin_Map[pin].halPin, (GPIO_PinState)val);
}

uint8_t digitalRead(uint8_t pin) {
	
    return HAL_GPIO_ReadPin(pin_Map[pin].pPort, pin_Map[pin].halPin);
}

void digitalToggle(uint8_t pin) {
	
    HAL_GPIO_TogglePin(pin_Map[pin].pPort, pin_Map[pin].halPin);
}

// Установка заполнения ШИМ
void analogWrite(uint8_t pin, uint8_t val) {
	
    for(uint8_t i = 0; i < PWM_COUNT; i++) {
		if(pwmPinState[i].pin == pin) {
			HAL_PWM_Duty_Set(&(pwmPinState[i].hpwm), val);
			pwmPinState[i].hpwm.Init.Pulse = val; 
		}
    }
}

uint32_t setPWMFreq(uint8_t pin, uint32_t pwmFreq) {
    for(uint8_t i = 0; i < PWM_COUNT; i++) {
		if(pwmPinState[i].pin == pin) {
			uint32_t freq = 40000000ul/ PWM_8BIT;
			uint32_t prescaler = freq/pwmFreq + 1;
			HAL_PWM_Stop(&(pwmPinState[i].hpwm));
			pwmPinState[i].hpwm.Init.Prescaler = prescaler ; // Прескалер
            //pwmPinState[i].hpwm.Init.Period = 200; // Частота ШИМ = 40,000,000 / prescaler / (255 + 1) 
			if (HAL_OK == HAL_PWM_Freq_Set(&(pwmPinState[i].hpwm), prescaler, PWM_8BIT))
			  {
			  HAL_PWM_Start(&(pwmPinState[i].hpwm));
			  return (freq/prescaler);
			  }			
			  break;  
		}
    }
   return 0;
}

void setPWM_Inverse(uint8_t pin, bool pwm_inverse, bool start) {
    for(uint8_t i = 0; i < PWM_COUNT; i++) {
		if(pwmPinState[i].pin == pin) {
			
			HAL_PWM_Stop(&(pwmPinState[i].hpwm));
			if (pwm_inverse) {
				pwmPinState[i].hpwm.Init.OutInverse = PWM_OUT_INVERSE_ENABLE;
			}
			else {
				pwmPinState[i].hpwm.Init.OutInverse = PWM_OUT_INVERSE_DISABLE;
			}
             
			 HAL_PWM_Init(&(pwmPinState[i].hpwm));  
			 if (start) HAL_PWM_Start(&(pwmPinState[i].hpwm));  
			 break;
		}
    }
   
}

void setPWM_OneShotMode(uint8_t pin, bool os_mode, uint8_t num_cnt, bool start) {
    for(uint8_t i = 0; i < PWM_COUNT; i++) {
		if(pwmPinState[i].pin == pin) {
			
			HAL_PWM_Stop(&(pwmPinState[i].hpwm));
			if (os_mode) {
				pwmPinState[i].hpwm.Init.AutoReloadPreload = PWM_AUTORELOAD_PRELOAD_DISABLE;
			 }
			 
			else {
				pwmPinState[i].hpwm.Init.AutoReloadPreload = PWM_AUTORELOAD_PRELOAD_ENABLE;
			}

			PWM_HandleTypeDef* hpwm = &(pwmPinState[i].hpwm);
			uint32_t Channel = hpwm->Channel;
            uint32_t PNUM_Mask = 0;
			uint32_t reg_val = 0;
			uint32_t pnum = num_cnt;
		 
		 
		     if (Channel == PWM_CHANNEL_0) {PNUM_Mask = PWM_PNUM_CH0; reg_val = pnum;}
             else if (Channel == PWM_CHANNEL_1) {PNUM_Mask = PWM_PNUM_CH1; reg_val = (pnum << PWM_PNUM_CH1_Pos);}
			 else if (Channel == PWM_CHANNEL_2) {PNUM_Mask = PWM_PNUM_CH2; reg_val = (pnum << PWM_PNUM_CH2_Pos);}
			 else if (Channel == PWM_CHANNEL_3) {PNUM_Mask = PWM_PNUM_CH3; reg_val = (pnum << PWM_PNUM_CH3_Pos);}
 			 else if (Channel == PWM_CHANNEL_4)
 			   {
  			      MODIFY_REG(hpwm->Instance->CH4CR1, PWM_CH4CR1_PNUM, (pnum << PWM_CH4CR1_PNUM_Pos));

   				}
			
			 if (PNUM_Mask) {
				MODIFY_REG(hpwm->Instance->PNUM, PNUM_Mask, reg_val);
			 }
			




    
  
  
			 HAL_PWM_Init(&(pwmPinState[i].hpwm));  
			 if (start) HAL_PWM_Start(&(pwmPinState[i].hpwm));  
			 break;
		}
    }
   
}
// Инициализация ШИМ
/*void setup_pwm()
{
	for(int8_t i = PWM_COUNT-1; i >= 0 ; i--) {
		if (pwmPinState[i].isPresent == true) {
			PWM_Init(&(pwmPinState[i].hpwm), pwmPinState[i].channel);
		}
	}
}*/

void PWM_Init(PWM_HandleTypeDef* hpwm, uint32_t channel)
{
    hpwm->Instance = PWM;
    hpwm->Init.AutoReloadPreload = PWM_AUTORELOAD_PRELOAD_ENABLE;
    hpwm->Init.CounterMode = PWM_COUNTERMODE_EDGEALIGNED_DOWN;
    hpwm->Init.Prescaler = 8; // Прескалер
    hpwm->Init.Period = 255; // Частота ШИМ = 40,000,000 / 8 / (255 + 1) = 19 530 Hz
    hpwm->Init.Pulse = 19;   // Заполнение = (19 + 1) / (255 + 1) = 7%
    hpwm->Init.OutMode = PWM_OUT_MODE_INDEPENDENT;
    hpwm->Channel = channel;
    HAL_PWM_Init(hpwm);
    HAL_PWM_Start(hpwm);
}

// Инициализация АЦП
/*void setup_adc() {
	for(uint8_t i = 0; i < ADC_COUNT; i++) {
		if (adcPinState[i].isPresent == true) {
			ADC_Init(&(adcPinState[i].hadc), adcPinState[i].channel);
		}
	}
}*/
// Чтение данных АЦП
double analogRead(uint8_t pin) {
	double value = 0.0;
    for(uint8_t i = 0; i < ADC_COUNT; i++) {
		if(adcPinState[i].pin == pin) {
			value = HAL_ADC_GET_INPUT_VOLTAGE(&(adcPinState[i].hadc));
		}
    }
	return value;
}

void ADC_Init(ADC_HandleTypeDef* hadc, uint32_t channel) {
    hadc->Instance = ADC;
    hadc->Init.channel = channel;
    hadc->Init.freq = 1000;
    HAL_ADC_Init(hadc);
}

//Встроенный криптомодуль ядра
void trngInit(void) {
	RCC->CLK_EN |= 1<<17;            //Тактирование 1-Вкл, 0-Выкл
    GPSEC->KEY0 = 31415926ul;         //Загрузка какого либо числа
    GPSEC->TRNG_CR =   (4<<3) | (0<<2) | (1<<1) | (1<<0) | 0x00;	//Магия! Все манипуляции с регистрами взяты из wm-w80x-sdk
}
void prngInit(void) {
    RCC->CLK_EN |= 1<<17;            
    GPSEC->KEY0 = 0x31415926ul;  
    GPSEC->CFG =   (1<<30) | (1<<29) |  (1<<28) |  0x00;
    GPSEC->TRNG_CR =   (4<<3) | (0<<2) |  (0<<1) |  (1<<0) | 0x00;
}

// Добавлено для совместимости wiring shiftOut/shiftIn
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder) {
	uint8_t value = 0;
	uint8_t i;

	for (i = 0; i < 8; ++i) {
		digitalWrite(clockPin, HIGH);
		if (bitOrder == LSBFIRST)
			value |= digitalRead(dataPin) << i;
		else
			value |= digitalRead(dataPin) << (7 - i);
		digitalWrite(clockPin, LOW);
	}
	return value;
}
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val)
{
	uint8_t i;

	for (i = 0; i < 8; i++)  {
		if (bitOrder == LSBFIRST) {
			digitalWrite(dataPin, val & 1);
			val >>= 1;
		} else {	
			digitalWrite(dataPin, (val & 128) != 0);
			val <<= 1;
		}
			
		digitalWrite(clockPin, HIGH);
		digitalWrite(clockPin, LOW);		
	}
}

// ToDo: Контроль коллизий в использовании выводов платы
bool check_pin() {
	return true;
}
