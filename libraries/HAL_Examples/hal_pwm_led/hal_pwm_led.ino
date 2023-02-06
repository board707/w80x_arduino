// For HLK-806-KIT HAL v "0.4.0"

#include <stdio.h>
#include <Arduino.h>

#define DUTY_MAX 99
#define DUTY_MIN 49

PWM_HandleTypeDef pwm[3];

static void PWM_Init(PWM_HandleTypeDef *hpwm, uint32_t channel);

void setup() {
  
  __HAL_RCC_PWM_CLK_ENABLE();

  // Remapping pin PB0,PB1,PB2 to PWM module

  __HAL_AFIO_REMAP_PWM0(GPIOB, GPIO_PIN_0);
  __HAL_AFIO_REMAP_PWM1(GPIOB, GPIO_PIN_1);
  __HAL_AFIO_REMAP_PWM2(GPIOB, GPIO_PIN_2);
  
  // Starting each instance of PWM channel
  
  for (int i = 2; i >= 0; i--) {
    PWM_Init(&pwm[i], PWM_CHANNEL_0 + i);
    HAL_PWM_Start(&pwm[i], i);
  }
}

void loop() {
  for (int i = 0; i < 3; i++) {
    for (int j = DUTY_MIN; j < DUTY_MAX; j++) {HAL_PWM_Duty_Set(&pwm[i], i, j); HAL_Delay(20);}
    for (int j = DUTY_MAX; j >= DUTY_MIN; j--) {HAL_PWM_Duty_Set(&pwm[i], i, j); HAL_Delay(20);}
  }
  HAL_Delay(20);
}

static void PWM_Init(PWM_HandleTypeDef *hpwm, uint32_t channel) {
  hpwm->Instance = PWM;
  hpwm->Init.AutoReloadPreload = PWM_AUTORELOAD_PRELOAD_ENABLE;
  hpwm->Init.CounterMode = PWM_COUNTERMODE_EDGEALIGNED_DOWN;
  hpwm->Init.Prescaler = 4;
  hpwm->Init.Period = 99;                         // Frequency = 40,000,000 / 4 / (99 + 1) = 100,000 = 100KHz
  hpwm->Init.Pulse = 19;                          // Duty Cycle = (19 + 1) / (99 + 1) = 20%
  hpwm->Init.OutMode = PWM_OUT_MODE_INDEPENDENT;  // Independent mode
  hpwm->Channel = channel;
  HAL_PWM_Init(hpwm);
}
