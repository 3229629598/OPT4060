#include "bringup.h"

uint8_t i;

void bringup_init(void)
{
	opt4060_init();
	data_process_init();
	HAL_Delay(10);
	HAL_TIM_Base_Start_IT(&htim1);
}

void tim1_loop(void)
{
	opt4060_loop();
	if (i++ > 10)
	{
		i = 0;
		data_process_loop();
	}
}

void main_loop(void)
{
}
