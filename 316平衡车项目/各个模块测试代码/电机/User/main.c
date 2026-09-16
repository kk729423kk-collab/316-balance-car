#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "key.h"
#include "PWM.h"
#include "IC.h"
#include "motor.h"
uint16_t duty1=40,duty2=80;
uint16_t num;
int32_t motor1_value=3600;//电机B
int32_t motor2_value=1000;
//
int main(void)
{
	/*模块初始化*/
	OLED_Init();		//
	Key_Init();
	motor_init(0,7200-1);
	OLED_ShowString(1,1,"motor1:");
	OLED_ShowString(3,1,"motor2:");
	while (1)
	{
		motor1_set(-6000);
		motor2_set(motor2_value);
		OLED_ShowSignedNum(2,1,motor1_value,6);
		OLED_ShowSignedNum(4,1,motor2_value,6);
	}
}
