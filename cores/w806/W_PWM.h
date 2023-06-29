
#ifndef W_PWM_h
#define W_PWM_h


#include "Common.h"
#include "variant.h"

/** pwm interrupt callback type **/
typedef void (*pwm_irq_callback)();

void PWM_Init(PWM_HandleTypeDef* hpwm, uint32_t channel);
uint32_t setPWM_Freq(PWM_HandleTypeDef* hpwm, uint32_t pwmFreq);
uint32_t setPWM_Freq_Period(PWM_HandleTypeDef* hpwm, uint32_t pwmFreq, uint8_t period);
void setPWM_OutInverse(PWM_HandleTypeDef* hpwm, bool pwm_inverse, bool start);
void setPWM_AutoReload(PWM_HandleTypeDef *hpwm, bool pwm_autoreload, bool start);
void setPWM_PulseCounter(PWM_HandleTypeDef *hpwm, uint8_t num_cnt);
void configPWM_Pulses(PWM_HandleTypeDef *hpwm, uint8_t pulse_cnt, pwm_irq_callback callback, bool start);
void disablePWM_Pulses(PWM_HandleTypeDef *hpwm, bool start);
void startPWM(PWM_HandleTypeDef* hpwm);
void stopPWM(PWM_HandleTypeDef* hpwm);

void HAL_PWM_IRQHandler(void);

#endif