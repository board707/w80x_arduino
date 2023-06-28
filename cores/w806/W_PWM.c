#include "./include/driver/wm_pwm.h"
#include "W_PWM.h"



pwm_irq_callback pwm_callback[PWM_COUNT]  = {0};

uint32_t setPWMFreq(PWM_HandleTypeDef *hpwm, uint32_t pwmFreq)
{
	if (hpwm == NULL)
		return 0;

	uint32_t freq = 40000000ul / PWM_8BIT;
	uint32_t prescaler = freq / pwmFreq + 1;
	HAL_PWM_Stop(hpwm);
	hpwm->Init.Prescaler = prescaler; // Прескалер
									  // Частота ШИМ = 40,000,000 / prescaler / (255 + 1)
	if (HAL_OK == HAL_PWM_Freq_Set(hpwm, prescaler, PWM_8BIT))
	{
		HAL_PWM_Start(hpwm);
		return (freq / prescaler);
	}
	return 0;
}

void setPWM_Inverse(PWM_HandleTypeDef *hpwm, bool pwm_inverse, bool start)
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

void enablePWM_Train(PWM_HandleTypeDef *hpwm, uint8_t num_cnt, pwm_irq_callback callback, bool start)
{
	if ((hpwm == NULL) || (num_cnt == 0))
		return;

	HAL_PWM_Stop(hpwm);
	if (callback != NULL)
	{
		CLEAR_BIT(PWM->IF, (1 << hpwm->Channel));
		SET_BIT(PWM->IE, (1 << hpwm->Channel));
	}
	else
	{
		CLEAR_BIT(PWM->IF, (1 << hpwm->Channel));
		CLEAR_BIT(PWM->IE, (1 << hpwm->Channel));
	}
	pwm_callback[hpwm->Channel] = callback;
	setPWM_OneShotMode(hpwm, false, num_cnt, start);
}

void disablePWM_Train(PWM_HandleTypeDef *hpwm, bool start)
{
	if (hpwm == NULL)
		return;

	HAL_PWM_Stop(hpwm);
	CLEAR_BIT(PWM->IF, (1 << hpwm->Channel));
	CLEAR_BIT(PWM->IE, (1 << hpwm->Channel));

	pwm_callback[hpwm->Channel] = NULL;
	setPWM_OneShotMode(hpwm, false, 0, start);
}

void setPWM_OneShotMode(PWM_HandleTypeDef *hpwm, bool os_mode, uint8_t num_cnt, bool start)
{
	if (hpwm == NULL)
		return;
	HAL_PWM_Stop(hpwm);
	if (os_mode)
	{
		hpwm->Init.AutoReloadPreload = PWM_AUTORELOAD_PRELOAD_DISABLE;
	}

	else
	{
		hpwm->Init.AutoReloadPreload = PWM_AUTORELOAD_PRELOAD_ENABLE;

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
	HAL_PWM_Init(hpwm);
	if (start)
		HAL_PWM_Start(hpwm);
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

void PWM_Init(PWM_HandleTypeDef *hpwm, uint32_t channel)
{
	hpwm->Instance = PWM;
	hpwm->Init.AutoReloadPreload = PWM_AUTORELOAD_PRELOAD_ENABLE;
	hpwm->Init.CounterMode = PWM_COUNTERMODE_EDGEALIGNED_DOWN;
	hpwm->Init.Prescaler = 8; // Прескалер
	hpwm->Init.Period = 255;  // Частота ШИМ = 40,000,000 / 8 / (255 + 1) = 19 530 Hz
	hpwm->Init.Pulse = 19;	  // Заполнение = (19 + 1) / (255 + 1) = 7%
	hpwm->Init.OutMode = PWM_OUT_MODE_INDEPENDENT;
	hpwm->Channel = channel;
	HAL_PWM_Init(hpwm);
	HAL_PWM_Start(hpwm);
}

void startPWM(PWM_HandleTypeDef* hpwm)  { HAL_PWM_Start(hpwm);}
void stopPWM(PWM_HandleTypeDef* hpwm)  { HAL_PWM_Stop(hpwm);}

void HAL_PWM_IRQHandler(void) {
 for (int8_t i = 0; i < PWM_COUNT; i++) {
	if ((READ_BIT(PWM->IE, (1 << i))) && (READ_BIT(PWM->IF, (1 << i))) ) {
		CLEAR_BIT(PWM->IF, (1 << i));
		if (pwm_callback[i] != NULL)  {
			 (* (pwm_callback[i])) ();

		}
	}
 }
    
}