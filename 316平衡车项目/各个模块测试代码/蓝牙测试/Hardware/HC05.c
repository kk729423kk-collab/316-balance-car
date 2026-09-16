#include "stm32f10x.h"                  // Device header
#include "Serial.h"

void HC05_init(void)
{
	Serial_Init(9600);
}
void HC05_send(char *ch)
{
	Serial_Printf(ch);
}

void HC05_get(char *buf)
{
		uint32_t count=0,index=0;
	while(count<1000)
	{
		if(Serial_GetRxFlag()==1)
		{
			buf[index]=Serial_GetRxData();
			index++;
			count=0;
		}
		count++;
	}
}
uint8_t  BlueTooth_Get_State(void)
{
	return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3);
}
