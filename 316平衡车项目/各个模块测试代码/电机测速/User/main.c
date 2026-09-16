#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Encoder.h"
int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	Encoder_Init();
	OLED_ShowString(1,1,"code1:");
	OLED_ShowString(2,1,"code2:");
	while (1)
	{
		OLED_ShowSignedNum(1,7,Encoder1_Get(),5);
		OLED_ShowSignedNum(2,7,Encoder2_Get(),5);
//		OLED_ShowNum(3,1,TIM_GetCounter(TIM3),4);
//		OLED_ShowNum(4,1,TIM_GetCounter(TIM4),4);

	}
}
