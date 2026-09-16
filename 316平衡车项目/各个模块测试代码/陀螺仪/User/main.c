#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "inv_mpu.h"
#include "MPU6050.h"
float Pitch,Roll,Yaw;								//俯仰角默认跟中值一样，翻滚角，偏航角
int16_t ax,ay,az,gx,gy,gz;							//加速度，陀螺仪角速度


extern uint16_t num;
int main(void)
{
	OLED_Init();
	MPU6050_DMP_Init();
	MPU6050_Int_Init();
	OLED_ShowString(1,1,"Wait MPU For 5s");
	delay_s(5);
	OLED_Clear();
	OLED_ShowString(2,1,"Pitch:");
	OLED_ShowString(3,1,"Roll:");
	OLED_ShowString(4,1,"Yaw:");
	while (1)
	{
		OLED_ShowNum(1,1,num,4);
		MPU6050_DMP_Get_Data(&Pitch,&Roll,&Yaw);				//读取姿态信息(其中偏航角有飘移是正常现象)
		MPU_Get_Gyroscope(&gx,&gy,&gz);
		MPU_Get_Accelerometer(&ax,&ay,&az);


		OLED_ShowSignedNum(2, 7, Pitch*10, 4);//Y
		OLED_ShowSignedNum(3, 6, Roll, 4);//X
		OLED_ShowSignedNum(4, 5, Yaw, 4);//Z
	
	}
}
