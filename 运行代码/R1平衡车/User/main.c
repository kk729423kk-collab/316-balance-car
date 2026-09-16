#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MPU6050.h"
#include "inv_mpu.h"
#include "MPU6050_Int.h "
#include "motor.h"
#include "Encoder.h"
#include "BlueTooth.h"
#include "AD.h"
#include "LED.h"
#include "Beep.h"
#include "Key.h"
#include "SR04.h"
#include "Serial.h"
#define RC_mode 0
#define Follow_mode 1
#define Aviod_mode 2
uint8_t Work_mode=RC_mode;

extern uint8_t BlueTooth_dat;
extern float pitch,roll,yaw;//俯仰角，横滚角，航向角
extern int Target_vel;
extern int Target_turn;
extern int Vel_Bias_sum;
extern int PWM,Now_encode_bias;
extern float Turn_Value;
extern short gx,gy,gz;
extern int RC_vel;
extern int RC_turn;
extern uint8_t rec_flag;
extern int8_t turn_flag;
extern float TurnKd;
extern float TurnKd,VerKp,VerKd,VelKp,VelKi;
extern uint64_t bug_time,time,num;

uint8_t now_blue_state;
uint8_t last_blue_state; 
uint8_t key_flag,key_value;
uint16_t dis_value,dis_temp,dis_temp_last;
uint8_t dis_dly;
uint8_t Down_flag,Load_flag;
uint16_t beep_count;
uint8_t para_index;
void TiaoCan_GUI(void)
{
			//OLED_Clear();
			OLED_ShowSignedNum(1,1,VerKp,5);
			OLED_ShowSignedNum(1,9,VerKd*100,3);
			OLED_ShowSignedNum(2,1,VelKp,5);
			OLED_ShowSignedNum(2,10,VelKi*100,3);
			OLED_ShowSignedNum(4,1,TurnKd*10,2);
			if(key_flag)
				{
					key_flag=1;
					key_value=Key_GetNum();
				}
				if(key_value ==1)
				{
					TurnKd+=0.1;
				}
				else if(key_value == 2)
				{
					TurnKd-=0.1;
				}
}
void BLueTooth_GUI(void)
{												 
		OLED_ShowString(2,1,"BLueTooth:");
			now_blue_state=BlueTooth_Get_State();
			if(now_blue_state==1)
		{
			if(last_blue_state==0)
			{
				OLED_ShowString(2,10,"   ");
			}
			//OLED_ShowString(2,10,"ON");
			OLED_ShowChinese(2,6,3);
			OLED_ShowChinese(2,7,4);
			OLED_ShowChinese(2,8,5);
		}
		else 
		{
			OLED_ShowChinese(2,6,6);
			OLED_ShowChinese(2,7,7);
			OLED_ShowChinese(2,8,8);
			//OLED_ShowString(2,10,"OFF");
		}
		
		
		last_blue_state=now_blue_state;
}
void Power_GUI(void)
{
			OLED_ShowString(1,1,"Power:000%");
			OLED_ShowNum(1,7,Battery_GetValue(),3);
			OLED_ShowNum(1,12,(int)((AD_GetValue()/4095.0*3.3)*100)/100%10,1);
		OLED_ShowNum(1,13,(int)((AD_GetValue()/4095.0*3.3)*100)/10%10,1);
		OLED_ShowNum(1,14,(int)((AD_GetValue()/4095.0*3.3)*100)%10,1);
		
}
void Down_scan(void)
{
	if(Down_flag)
		{
			if(beep_count<5)
			{
				beep_count++;
				Beep_On();
			}
			else  
			{
				beep_count=6;
				Beep_Off();
			}
			
		}
		else 
		{
			beep_count=0;
			Beep_Off();
		}
		
		if(Load_flag==1&&Down_flag==0)
		{
			LED1_ON();
		}
		else 
		{
			LED1_OFF();
		}
}
void Mode_GUI(void)
{
	
	switch(Work_mode)
					{
						case RC_mode:
						{
							OLED_ShowString(3,6,"         ");
								OLED_ShowChinese(3,5,9);
								OLED_ShowChinese(3,6,10);
						}
						break;
							case Follow_mode:
						{
							OLED_ShowString(3,6,"           ");
							//OLED_ShowString(3,6,"Follow");
								OLED_ShowChinese(3,5,11);
								OLED_ShowChinese(3,6,12);
						}
						break;
							case Aviod_mode:
						{
							OLED_ShowString(3,6,"           ");
							//OLED_ShowString(3,6,"Avoid");
								OLED_ShowChinese(3,5,13);
								OLED_ShowChinese(3,6,14);
						}
						break;
					}
}
void Work_mode_Sel(void)
{
				
				
				if(key_value ==1)
				{
					Work_mode=(Work_mode+1)%3;
					Target_vel=Target_turn=0;
						Mode_GUI();
				}
}
void Dis_Get(void)
{
	if(dis_dly>5)
			{
				dis_dly=0;
				delay_ms(10);
				dis_temp=Distance_Get();
			if(dis_temp!=0)dis_value=dis_temp;
				//Serial_Printf("%d\r\n",dis_value);
			}
}
void Para_Select(void)
{
	if(key_value ==2)
	{
		para_index=(para_index+1)%5;
		if(para_index == 0)
		{
			OLED_ShowString(4,1,"               ");
			OLED_ShowString(4,1,"VerKp:");
		}
		else if(para_index == 1)
		{
			OLED_ShowString(4,1,"               ");
			OLED_ShowString(4,1,"VerKd*100:");
		}
		else if(para_index == 2)
		{
			OLED_ShowString(4,1,"               ");
			OLED_ShowString(4,1,"VelKp:");
		}
		else if(para_index == 3)
		{
			OLED_ShowString(4,1,"               ");
			OLED_ShowString(4,1,"VelKi*100:");
		}
		else if(para_index == 4)
		{
			OLED_ShowString(4,1,"               ");
			OLED_ShowString(4,1,"TurnKd*10:");
		}
	}
	if(para_index ==0 )
	{
		OLED_ShowSignedNum(4,7,VerKp,3);
	}
	else if(para_index == 1)
	{
		OLED_ShowSignedNum(4,11,VerKd*100,3);
	}
	else if(para_index == 2)
	{
		OLED_ShowSignedNum(4,7,VelKp,4);
	}
		else if(para_index == 3)
	{
		OLED_ShowSignedNum(4,11,VelKi*100,4);
	}
		else if(para_index == 4)
	{
		OLED_ShowSignedNum(4,11,TurnKd*10,4);
	}
}
void Key_Scan(void)
{
			if(key_flag)
				{
					key_flag=0;
					key_value=Key_GetNum();
				}
}
int main(void)
{
	
	OLED_Init();
	Key_Init();
	LED_Init();
	LED1_ON();
	LED2_ON();
	Beep_Init();
	SR04_Init();
	MPU6050_DMP_Init();
	OLED_ShowChinese(1,3,15);
	OLED_ShowChinese(1,4,16);
	OLED_ShowChinese(1,5,17);
	OLED_ShowChinese(1,6,18);
	Delay_s(3);
	OLED_Clear();
	MPU6050_Int_Init();
	AD_Init();
	motor_init(0,7200-1);
	BlueTooth_Init(128000);
	Encoder_Init();


	OLED_ShowString(3,1,"Mode:");
	OLED_ShowChinese(3,5,9);
	OLED_ShowChinese(3,6,10);//
	
	
	OLED_ShowString(4,1,"VerKp:");
	OLED_ShowSignedNum(4,7,VerKp,3);
	
	while (1)
	{
		Key_Scan();
		Dis_Get();	
		//OLED_ShowNum(1,1,dis_value,4);
		//Power_GUI();
		OLED_ShowChinese(1,3,19);
		OLED_ShowChinese(1,4,20);
		OLED_ShowChinese(1,5,21);
		OLED_ShowChinese(1,6,22);
		BLueTooth_GUI();
		Down_scan();
		Work_mode_Sel();
		LED2_Turn();
		Para_Select();
		//TiaoCan_GUI();
	}
}
