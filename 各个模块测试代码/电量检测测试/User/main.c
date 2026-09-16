#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"
int main(void)
{
	OLED_Init();	
	AD_Init();
  OLED_ShowString(1,3,"Battery Test");
	OLED_ShowString(2,1,"Current:000%");
	OLED_ShowString(3,1,"Vol:0.00V");
	while (1)
	{
		
		OLED_ShowNum(2,9,Battery_GetValue(),3);
		OLED_ShowNum(3,5,(int)((AD_GetValue()/4095.0*3.3)*100)/100%10,1);
		OLED_ShowNum(3,7,(int)((AD_GetValue()/4095.0*3.3)*100)/10%10,1);
		OLED_ShowNum(3,8,(int)((AD_GetValue()/4095.0*3.3)*100)%10,1);
		//Delay_ms(100);
	}
}

