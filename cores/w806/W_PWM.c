#include "./include/driver/wm_pwm.h"
#include "W_PWM.h"



pwm_irq_callback pwm_callback[PWM_COUNT]  = {0};

uint32_t setPWM_Freq_Period(PWM_HandleTypeDef *hpwm, uint32_t pwmFreq, uint8_t period)
{
	if (hpwm == NULL)
		return 0;

	uint32_t freq = 40000000ul / period;
	uint32_t prescaler = freq / pwmFreq + 1;
	HAL_PWM_Stop(hpwm);
	hpwm->Init.Prescaler = prescaler; // PWM Freq = 40,000,000 / prescaler / (period + 1)
									  
	if (HAL_OK == HAL_PWM_Freq_Set(hpwm, prescaler, period))
	{
		HAL_PWM_Start(hpwm);
		return (freq / prescaler);
	}
	return 0;
}
uint32_t setPWM_Freq(PWM_HandleTypeDef *hpwm, uint32_t pwmFreq)
{
	return(setPWM_Freq_Period(hpwm, pwmFreq, PWM_8BIT));
}

// for compatibility with prev release
uint32_t setPWMFreq(uint8_t pin, uint32_t pwmFreq)
{
	PWM_HandleTypeDef *hpwm = getPWMHandle(pin);
	return(setPWM_Freq_Period(hpwm, pwmFreq, PWM_8BIT));
}

void setPWM_OutInverse(PWM_HandleTypeDef *hpwm, bool pwm_inverse, bool start)
{
	if (hpwm == NULL)
		return ;
	HAL_PWM_Stop(hpwm);
	if (pwm_inverse)
	{
		hpwm->Init.OutInverse = PWM_OUT_INVERSE_ENABLE;
	}
	else
	{
		hpwm->Init.OutInverse = PWM_OUT_INVERSE_DISABLE;
	}

	HAL_PWM_Init(hpwm);
	if (start)
		HAL_PWM_Start(hpwm);
	
}
void setPWM_AutoReload(PWM_HandleTypeDef *hpwm, bool pwm_autoreload, bool start) {
	if (hpwm == NULL)
		return ;
	HAL_PWM_Stop(hpwm);
	if (pwm_autoreload)
	{
		hpwm->Init.AutoReloadPreload = PWM_AUTORELOAD_PRELOAD_ENABLE;
	}

	else
	{
		hpwm->Init.AutoReloadPreload = PWM_AUTORELOAD_PRELOAD_DISABLE;

	}

	HAL_PWM_Init(hpwm);
	if (start)
		HAL_PWM_Start(hpwm);
}

void setPWM_PulseCounter(PWM_HandleTypeDef *hpwm, uint8_t num_cnt) {
        if (hpwm == NULL)
			return;
		
		uint32_t Channel = hpwm->Channel;
		uint32_t PNUM_Mask = 0;
		uint32_t reg_val = 0;
		uint32_t pnum = num_cnt;

		if (Channel == PWM_CHANNEL_0)
		{
			PNUM_Mask = PWM_PNUM_CH0;
			reg_val = pnum;
		}
		else if (Channel == PWM_CHANNEL_1)
		{
			PNUM_Mask = PWM_PNUM_CH1;
			reg_val = (pnum << PWM_PNUM_CH1_Pos);
		}
		else if (Channel == PWM_CHANNEL_2)
		{
			PNUM_Mask = PWM_PNUM_CH2;
			reg_val = (pnum << PWM_PNUM_CH2_Pos);
		}
		else if (Channel == PWM_CHANNEL_3)
		{
			PNUM_Mask = PWM_PNUM_CH3;
			reg_val = (pnum << PWM_PNUM_CH3_Pos);
		}
		else if (Channel == PWM_CHANNEL_4)
		{
			MODIFY_REG(hpwm->Instance->CH4CR1, PWM_CH4CR1_PNUM, (pnum << PWM_CH4CR1_PNUM_Pos));
		}

		if (PNUM_Mask)
		{
			MODIFY_REG(hpwm->Instance->PNUM, PNUM_Mask, reg_val);
		}

}

void configPWM_Pulses(PWM_HandleTypeDef *hpwm, uint8_t pulse_cnt, pwm_irq_callback callback, bool start)
{
	if ((hpwm == NULL) || (pulse_cnt == 0))
		return;

	HAL_PWM_Stop(hpwm);
	if (callback != NULL)
	{
		if (pwm_callback[hpwm->Channel] == NULL )  {
		NVIC_ClearPendingIRQ(PWM_IRQn);
		NVIC_EnableIRQ(PWM_IRQn);
	}
		SET_BIT(PWM->IF, (1 << hpwm->Channel));  // clearing IRQ flag by writing 1
		SET_BIT(PWM->IE, (1 << hpwm->Channel));
	}
	else
	{
		SET_BIT(PWM->IF, (1 << hpwm->Channel));  // clearing IRQ flag by writing 1
		CLEAR_BIT(PWM->IE, (1 << hpwm->Channel));
	}

    
	pwm_callback[hpwm->Channel] = callback;
	if (pulse_cnt == 1) {
		setPWM_AutoReload(hpwm, false, start);
	}
	else {
		setPWM_PulseCounter(hpwm, pulse_cnt); 
		setPWM_AutoReload(hpwm, true, start);
		
	}
	
}

void disablePWM_Pulses(PWM_HandleTypeDef *hpwm, bool start)
{
	if (hpwm == NULL)
		return;

	HAL_PWM_Stop(hpwm);
	SET_BIT(PWM->IF, (1 << hpwm->Channel));  // clearing IRQ flag by writing 1
	CLEAR_BIT(PWM->IE, (1 << hpwm->Channel));

	pwm_callback[hpwm->Channel] = NULL;
	setPWM_PulseCounter(hpwm, 0); 
	setPWM_AutoReload(hpwm, true, start);
}



void PWM_Init(PWM_HandleTypeDef *hpwm, uint32_t channel)
{
	hpwm->Instance = PWM;
	hpwm->Init.AutoReloadPreload = PWM_AUTORELOAD_PRELOAD_ENABLE;
	hpwm->Init.CounterMode = PWM_COUNTERMODE_EDGEALIGNED_DOWN;
	hpwm->Init.Prescaler = 8; 
	hpwm->Init.Period = 255;  // PWM Freq = 40,000,000 / prescaler / (255 + 1) = 19 530 Hz
	hpwm->Init.Pulse = 0;	  
	hpwm->Init.OutMode = PWM_OUT_MODE_INDEPENDENT;
	hpwm->Channel = channel;
	setPWM_PulseCounter(hpwm, 0); 
	HAL_PWM_Init(hpwm);
	HAL_PWM_Start(hpwm);
}

void startPWM(PWM_HandleTypeDef* hpwm)  { 
	HAL_PWM_Start(hpwm);
	}

void stopPWM(PWM_HandleTypeDef* hpwm)  { 
	HAL_PWM_Stop(hpwm);
	}

void HAL_PWM_IRQHandler(void) {
 for (int8_t i = 0; i < PWM_COUNT; i++) {
	if ((READ_BIT(PWM->IE, (1 << i))) && (READ_BIT(PWM->IF, (1 << i))) ) {
		SET_BIT(PWM->IF, (1 << i));  // clear IRQ flag
		if (pwm_callback[i] != NULL)  {
			 (* (pwm_callback[i])) ();

		}
	}
 }
    
}