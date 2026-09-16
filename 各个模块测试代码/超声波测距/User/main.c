#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "SR04.h"
int main(void)
{
	OLED_Init();		
	SR04_Init();
	OLED_ShowString(1,1,"Distance:");
	OLED_ShowString(2,7,"mm");
	uint16_t value;
	while (1)
	{
		value=Distance_Get();
		OLED_ShowNum(2,3,value,4);
		
	}
}

