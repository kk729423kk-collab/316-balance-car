#include "stm32f10x.h" 
#include "Encoder.h"
#include "MPU6050.h"
#include "inv_mpu.h"
#include "control.h"
#include "motor.h"
#include "BlueTooth.h"
#include "AD.h"
#include "SR04.h"
#define vel_max 6800
#define Dead_vel  500
#define turn_sum_max 30
#define RC_vel_max   20
#define RC_turn_max 20
#define RC_mode 0
#define Follow_mode 1
#define Aviod_mode 2
extern uint8_t Work_mode;
float pitch,roll,yaw;
short gx,gy,gz;

int PWM;
int Now_Velocity;
int Now_encode_bias;
float Turn_Value;
int left_code,right_code;

int Target_vel=0;
int Target_turn=0;
int Target_dis=80;

float Ver_angle=1.5;//机械中值
float VerKp=324,VerKd=2.24;//平衡环参数
float VelKp=-260,VelKi=-1.3;//速度环参数
float TurnKp=100,TurnKd=-0.6;//转向
float FLo_kp=0.3,FLo_kd=-0.15,FLo_ki=-0.1;//跟随
float Avoid_Kp=0.6;//避障

//float Ver_angle=5.05;//机械中值
//float VerKp=-540*0.6,VerKd=-1.9;//平衡环参数
//float VelKp=-250,VelKi=-1.25;//速度环参数
//float TurnKp=100,TurnKd=-0.9;//转向
//float FLo_kp=-0.33,FLo_kd=0.15,FLo_ki=0.1;//跟随
//float Avoid_Kp=0;//避障

extern uint8_t Fore,Back,Left,Right;
extern uint8_t now_blue_state,last_blue_state;
extern uint8_t key_flag;
extern uint8_t Down_flag,Load_flag;
extern uint8_t dis_dly;
extern uint16_t dis_value;
//直立环PD调节
float angle_bias;
//乘以0.6后

//速度环PI调节
int Velocity_bias,Fil_Vel_bias,Vel_Bias_sum,Vel_Bias_Last;

//转向环
int Turn_bias,Turn_Bias_Last;

//追踪环
int Dis_bias,Dis_bias_Last,Defer_dis_bias,Defer_dis_bias_sum;

//避障环
int Dis_bias,Dis_bias_Last,Defer_dis_bias,Defer_dis_bias_sum;

int XianFu(int value,int max)
{
	if(value>max)value=max;
	if(value<-max)value=-max;
	else value=value;
	return  value;
}

int Xian_min(int value,int min)
{
	if(value<min&&value>=0)value=min;
	if(value>-min&&value<0)value=-min;
	else value=value;
	return  value;
}
//直立环
int Vertical_Pid_Value(float Pitch,float GyroY)
{
	angle_bias=Pitch -Ver_angle;
	return VerKp*angle_bias+VerKd*GyroY;
}

//速度环
int Velocity_Pid_Value(int Target_Vel,float now_velocity)
{
	float a=0.3;
	Velocity_bias=now_velocity-Target_Vel;
	Fil_Vel_bias=a*Velocity_bias+(1-a)*Vel_Bias_Last;//滤波
	Vel_Bias_sum+=Fil_Vel_bias;
	Vel_Bias_sum=XianFu(Vel_Bias_sum,vel_max);
	Vel_Bias_Last=Velocity_bias;//滤波用
	return VelKp*Fil_Vel_bias+VelKi*Vel_Bias_sum;
}
//转向环
int Turn_Pid_value(int Target_turn, int gyroZ)
{
	if(Target_turn==0)
	{
		return TurnKd * gyroZ;
	}
	else 
	{
		return  TurnKp*Target_turn;
	} 
}
//追踪环
int Follow_Pid_value(int Now_dis)
{
	Dis_bias=Now_dis-Target_dis;
	Defer_dis_bias=Dis_bias-Defer_dis_bias;
	Dis_bias_Last=Dis_bias;
	Defer_dis_bias_sum+=Dis_bias;
	if(Defer_dis_bias_sum>20)Defer_dis_bias_sum=20;
	else if(Defer_dis_bias_sum<-20)Defer_dis_bias_sum=-20;
	return FLo_kp*Dis_bias+FLo_kd*Defer_dis_bias+FLo_ki*Defer_dis_bias_sum;
}
//避障环
int Avoid_Pid_value(int Now_dis)
{
	
	return Now_dis*Avoid_Kp;

}
//拿起检测
void Leave_Scan(void)
{
	 static uint16_t Leave_count;

	if(Target_vel == 0||Work_mode!=RC_mode)
	{			
				if((((left_code>30)&&(right_code>30))||((left_code<-30)&&right_code<-30))&&((pitch<30)&&(pitch>-30)))
			{

				Leave_count++;
				if(Leave_count>25)
				{
					Load_flag=0;
					Leave_count=26;
				}
			}
		else 
		{
			Leave_count=0;
		}
	}
	else 
	{
		Leave_count=0;
	}
}
//着陆检测
void Load_Scan(void)
{
	static float last_pitch;
	static uint16_t count1,count2;
	if((pitch>(Ver_angle-5))&&(pitch<(Ver_angle+5)))
	{
		count1++;
		if(count1>25)
		{
			count1=0;
			if(((last_pitch-pitch)<2)&&((last_pitch-pitch)>-2))
			{
				count2++;
				if(count2>2)
				{
					count2=0;
					count1=0;
					Load_flag=1;
				}
			}
			else
			{
				count2=0;
			}
		}
		last_pitch=pitch;
	}
	else 
	{
		count1=count2=0;
	}
}
void EXTI0_IRQHandler (void)
{
	if(EXTI_GetFlagStatus(EXTI_Line0)==SET)
	{
		key_flag=1;
		dis_dly++;
		if(MPU6050_DMP_Get_Data(&pitch,&roll,&yaw)==0)
		{
				left_code=Encoder_Left_Get();
				right_code=Encoder_Right_Get();
				Now_Velocity=(left_code+right_code)/2;
				Now_encode_bias=(left_code-right_code);
				if(Load_flag==1)Leave_Scan();
				else if(Load_flag == 0)Load_Scan();
					if((pitch>60||pitch<-60)||Load_flag == 0)//
				{
							if(pitch>60||pitch<-60)Down_flag=1;
							Vel_Bias_sum=0;
							Defer_dis_bias_sum=0;
							Target_vel=Target_turn=0;
							motor_Right_set(0);
							motor_Left_set(0);
							Fore=Back=Left=Right=0;
				}
				else if(MPU_Get_Gyroscope(&gx,&gy,&gz)==0)
				{
									Down_flag=0;
							if(Work_mode==RC_mode)
							{
										if(Fore==0&Back==0)
										{
											if(Target_vel>0)
											{
												if(Target_vel!=0)Target_vel--;
											}
											else 
											{
												if(Target_vel!=0)Target_vel++;
											}//前后减速
										}
										
										if(Fore==1)Target_vel++;
										else if(Back==1)Target_vel--;
										Target_vel=XianFu(Target_vel,RC_vel_max);
										
										if(Left==0&&Right==0)
										{
											if(Target_turn>0)
											{
												if(Target_turn!=0)Target_turn--;
							
											}
											else
											{
												if(Target_turn!=0)Target_turn++;
											}						//左右减速
										}
										
										if(Left==1)Target_turn+=2;
										else if(Right==1)Target_turn-=2;
										Target_turn=XianFu(Target_turn,RC_turn_max);
										
							}
								else if(Work_mode==Follow_mode) 
								{
									if(dis_value>20&&dis_value<200)Target_vel=Follow_Pid_value(dis_value);
									else 
									{
											if(Target_vel>0)
											{
												if(Target_vel!=0)Target_vel--;
											}
											else 
											{
												if(Target_vel!=0)Target_vel++;
											}//前后减速
									}
								}
								else if(Work_mode == Aviod_mode)
								{
//										Target_vel++;//左右单方向加速
//										if(Target_vel>=10)Target_vel=10;//避障直行速度  one
//										Target_vel--;//左右单方向加速
//										if(Target_vel<=-10)Target_vel=-10;//避障直行速度
									if(dis_value<150)
									{
										Target_turn+=25 ;
										Target_turn=XianFu(Target_turn,RC_turn_max+5);
									}
									else if(Target_turn!=0)Target_turn-=5;//左右单方向减速
								}
								//PWM=Vertical_Pid_Value(pitch,gy);
								//PWM=Velocity_Pid_Value(Now_Velocity,Target_vel);
									Turn_Value=Turn_Pid_value(Target_turn,gz);
									PWM=Vertical_Pid_Value(pitch,gy)+Velocity_Pid_Value(Now_Velocity,Target_vel);					
									PWM=XianFu(PWM,vel_max);
									PWM=Xian_min(PWM,Dead_vel);
									motor_Right_set(PWM+Turn_Value);
									motor_Left_set(PWM-Turn_Value);		
//								motor_Right_set(Turn_Value);
//								motor_Left_set(-Turn_Value);
				}
		}
	
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}
	