#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Beep.h"
#include "Key.h"
#include "LED.h"
int8_t num;
uint8_t key_value;
int main(void)
{
	Beep_Init();
	OLED_Init();
	Key_Init();
	LED_Init();
	OLED_ShowString(1,4,"Test!");
	OLED_ShowString(2,1,"LED1:");
	OLED_ShowString(3,1,"LED2:");
	OLED_ShowString(4,1,"Beep:");
	while (1)
	{
		key_value=Key_GetNum();
		if(key_value==1)
		{
			num++;
			Beep_Turn();
			LED1_Turn();
		}
		else if(key_value==2)
		{
			num--;
			Beep_Turn();
			LED2_Turn();
		}
		OLED_ShowSignedNum(1,7,num,2);
		OLED_ShowNum(2,6,!GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_0),1);
		OLED_ShowNum(3,6,!GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_1),1);
		OLED_ShowNum(4,6,GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_15),1);
	}
}

