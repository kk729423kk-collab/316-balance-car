#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "HC05.h"
int main(void)
{
	OLED_Init();		//OLED初始化
	Serial_Init(128000);
	//Srial_Init(9600);
	OLED_ShowString(1,1,"bluetoothTest:");
	OLED_ShowString(2,1,"Rec:");
	OLED_ShowString(3,1,"Link State:");
	while (1)
	{
		
		if(Serial_GetRxFlag()==1)
		{
			Serial_Printf("Succeed!\r\n");
			OLED_ShowChar(2,5,Serial_GetRxData());
		}
		OLED_ShowNum(3,12,BlueTooth_Get_State(),1);
		//OLED_ShowNum(3,12,Serial_GetRxFlag(),1);
		
	}
}
