// For HLK-806-KIT

#include <stdio.h>
#include <Arduino.h>

#define DUTY_MAX 250
#define DUTY_MIN 50

PWM_HandleTypeDef hal_pwm[3];

int i, j, m[3] = { 0 }, d[3] = { DUTY_MIN, (DUTY_MIN + DUTY_MAX) / 2, DUTY_MAX - 1 };

static void PWM_Init(PWM_HandleTypeDef *hpwm, uint32_t channel);


void setup() {

  __HAL_RCC_PWM_CLK_ENABLE();

  // Remapping pin PB0,PB1,PB2 to PWM module

  __HAL_AFIO_REMAP_PWM0(GPIOB, GPIO_PIN_0);
  __HAL_AFIO_REMAP_PWM1(GPIOB, GPIO_PIN_1);
  __HAL_AFIO_REMAP_PWM2(GPIOB, GPIO_PIN_2);

  // Starting each instance of PWM channel
  for (i = 2; i >= 0; i--) {
    PWM_Init(&hal_pwm[i], PWM_CHANNEL_0 + i);
    HAL_PWM_Start(&hal_pwm[i]);
  }
}

void loop() {
  for (i = 0; i < 3; i++) {
    if (m[i] == 0)  // Increasing
    {
      HAL_PWM_Duty_Set(&hal_pwm[i], d[i]++);
      if (d[i] == DUTY_MAX) {
        m[i] = 1;
      }
    } else  // Decreasing
    {
      HAL_PWM_Duty_Set(&hal_pwm[i], d[i]--);
      if (d[i] == DUTY_MIN) {
        m[i] = 0;
      }
    }
  }
  delay(10);
}

static void PWM_Init(PWM_HandleTypeDef *hpwm, uint32_t channel) {
  hpwm->Instance = PWM;
  hpwm->Init.AutoReloadPreload = PWM_AUTORELOAD_PRELOAD_ENABLE;
  hpwm->Init.CounterMode = PWM_COUNTERMODE_EDGEALIGNED_DOWN;
  hpwm->Init.Prescaler = 4;
  hpwm->Init.Period = 255;                         
  hpwm->Init.Pulse = 19;                          
  hpwm->Init.OutMode = PWM_OUT_MODE_INDEPENDENT;  
  hpwm->Channel = channel;
  HAL_PWM_Init(hpwm);
}
