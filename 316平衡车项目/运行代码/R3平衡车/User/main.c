#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MPU6050.h"
#include "inv_mpu.h"
#include "MPU6050_Int.h "
#include "motor.h"
#include "Encoder.h"
#include "HC05.h"
#include "AD.h"
#include "LED.h"
#include "Beep.h"
#include "Key.h"
#include "SR04.h"
extern uint8_t HC05_dat;
extern uint8_t rec_flag;
extern float pitch,roll,yaw;//俯仰角，横滚角，航向角
extern int Vel_Bias_sum;
extern uint32_t PWM;
extern short gx,gy,gz;
extern int RC_vel;
extern int RC_turn;
extern uint8_t rec_flag;
extern int8_t turn_flag;
extern float TurnKd;
extern uint8_t Load_flag;
extern int left_code,right_code;

extern uint8_t Fore,Back,Left,Right;

uint8_t dis_value,temp_dis_value;
uint8_t mode;
uint8_t dis_dly;
int main(void)
{
	OLED_Init();
	MPU6050_DMP_Init();
	MPU6050_Int_Init();
	motor_init(0,7200-1);
	Encoder_Init();
	HC05_init();
	AD_Init();
	SR04_Init();
//	OLED_ShowChinese(1,3,0);
//	OLED_ShowChinese(1,4,1);
//	OLED_ShowChinese(1,5,2);
//	OLED_ShowChinese(1,6,3);//平衡小车汉字显示
	OLED_ShowString(2,1,"System State:");
	OLED_ShowString(1,1,"Dis:");
	while (1)
	{
		if(dis_dly>6)
		{
			dis_dly=0;	
			temp_dis_value=Distance_Get();//获取当前距离
			if(temp_dis_value!=0)dis_value=temp_dis_value;
			OLED_ShowNum(1,6,dis_value,4);
		}
		//拿起检测
		if(Load_flag)
		{
			OLED_ShowString(3,4,"Start Work   " );
		}
		else 
		{
			OLED_ShowString(3,4,"Stop Work    ");
		}
		
		if(mode == 0)
		{
			OLED_ShowString(4,1,"Void Mode       ");
		}
		else if(mode == 1)
		{
			OLED_ShowString(4,1,"Blue control    ");
		}
	}
}
