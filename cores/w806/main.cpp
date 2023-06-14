#include <stdio.h>
#include "Arduino.h"
#include "wm_hal.h"
//#include "Stream.h"

void Error_Handler(void);
static volatile bool _loop = false;

/*TIM_HandleTypeDef htim0;
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim5;*/

int main(void)
{
#if (F_CPU == 240000000)
    uint32_t f_cpu_dir = (uint32_t)CPU_CLK_240M;
#elif (F_CPU == 80000000)
    uint32_t f_cpu_dir = (uint32_t)CPU_CLK_80M;
#elif (F_CPU == 4000000)
    uint32_t f_cpu_dir = (uint32_t)CPU_CLK_40M;
#elif (F_CPU == 2000000)
    uint32_t f_cpu_dir = (uint32_t)CPU_CLK_2M;
#else
    uint32_t f_cpu_dir = (uint32_t)CPU_CLK_240M;
#endif
	
    SystemClock_Config(f_cpu_dir);
    setup();
	//setup_pwm();
    //setup_adc();

    for (;;) {
		_loop = true;
        loop();
    }

    return 0;
}

//bool is_loop(){return _loop;}
//void set_loop(bool param) {_loop = param;}

void Error_Handler(void)
{
    while (1)
    {
		printf("System failure... Nuclear explosion in 1 minute. \r\n");
    }
}

void assert_failed(uint8_t *file, uint32_t line)
{
	printf("Wrong parameters value: file %s on line %d\r\n", file, line);
}
