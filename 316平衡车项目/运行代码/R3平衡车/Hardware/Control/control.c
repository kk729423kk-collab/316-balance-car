#include "stm32f10x.h" 
#include "Encoder.h"
#include "MPU6050.h"
#include "inv_mpu.h"
#include "control.h"
#include "motor.h"
#define max_vel 6800

#define max_turn 500

#define max_rc_vel 35
#define max_rc_turn 15

float pitch,roll,yaw;
short gx,gy,gz;

int PWM;
float Now_Velocity;
float Turn_Value;

int RC_vel;
int RC_turn;
int left_code,right_code;
uint8_t Load_flag;
extern uint8_t Fore,Back,Left,Right;
extern uint8_t dis_value;
extern uint8_t mode;
extern uint8_t dis_dly;
void Vel_Xianfu(int *value)
{
	if(*value>max_vel)*value=max_vel;
	if(*value<-max_vel)*value=-max_vel;
}
void Turn_Xianfu(int *value)
{
	if(*value>max_turn)*value=max_turn;
	if(*value<-max_turn)*value=-max_turn;
}

float Ver_angle=1.5;

int8_t turn_flag;
//直立环PD调节
float angle_bias;
//乘以0.6后
//-380
float VerKp=-330,VerKd=-2.0;

//速度环PI调节
int velocity_bias,Fil_Vel_bias,Vel_Bias_sum,Vel_Bias_Last;
float VelKp=-280,VelKi=-1.4;

//转向环P调节

float TurnKp=10,TurnKd=0.05;

//转向环
float Turn_bias;
float TurnKp;
void XianFu(int max)
{
	if(Vel_Bias_sum>max)Vel_Bias_sum=max;
	if(Vel_Bias_sum<-max)Vel_Bias_sum=-max;
}
void PWM_XianFU(int max,int *value)
{
	if(*value>=max)*value=max;
	if(*value<=(-max))*value=-max;
}
//直立环
int Vertical_Pid_Value(float Pitch,float Gyro)
{
	angle_bias=Pitch -Ver_angle;
	return VerKp*angle_bias+VerKd*Gyro;
}
//目标值为0，bias等于速度值
int Velocity_Pid_Value(int Target_Vel,float now_velocity)
{
	float a=0.3;
	velocity_bias=now_velocity-Target_Vel;
	Fil_Vel_bias=a*velocity_bias+(1-a)*Vel_Bias_Last;//滤波
	Vel_Bias_sum+=Fil_Vel_bias;
	XianFu(6800);
	Vel_Bias_Last=velocity_bias;//滤波用
	return VelKp*Fil_Vel_bias+VelKi*Vel_Bias_sum;
}
int Turn_Pid_value(int RC_turn,float gy)
{
	if(RC_turn!=0)
	return TurnKp*RC_turn;
	else
	{
		return  TurnKd*gy;
	}
}
//拿起检测
void Leave_Scan(void)
{
	 static uint16_t Leave_count;

	if(RC_vel==0)
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
		left_code=Encoder_Left_Get();
		right_code=Encoder_Right_Get();
		if(MPU6050_DMP_Get_Data(&pitch,&roll,&yaw)==0)
		{
			dis_dly++;
			if(Load_flag==1)Leave_Scan();
			else if(Load_flag == 0)Load_Scan();
			
					if(pitch>40||pitch<-40||Load_flag==0)
				{
							
							RC_vel=RC_turn=0;
							Vel_Bias_sum=0;
							motor_Right_set(0);
							motor_Left_set(0);
				}
				else
				{
					
								if(MPU_Get_Gyroscope(&gx,&gy,&gz)==0)
						{
							
							Now_Velocity=(Encoder_Left_Get()+Encoder_Right_Get())/2;
									
										if(mode == 1)
										{
											
													if(Fore==0&Back==0)
												{
													if(RC_vel>0)
													{
														if(RC_vel!=0)RC_vel-=6;
													}
													else if(RC_vel<0)
													{
														if(RC_vel!=0)RC_vel+=6;
													}
												}
												if(Fore==1)RC_vel+=6;
												else if(Back==1)RC_vel-=6;
												if(RC_vel>max_rc_vel)RC_vel=max_rc_vel;
												else if(RC_vel<-max_rc_vel)RC_vel=-max_rc_vel;
												
												if(Left==0&&Right==0)
												{
													RC_turn=0;
													
												}
												if(Left==1)RC_turn+=2;
												else if(Right==1)RC_turn-=2;
												if(RC_vel>max_rc_turn)RC_vel=max_rc_turn;
												else if(RC_vel<-max_rc_turn)RC_vel=-max_rc_turn;
										}
										else if(mode == 0)
										{
											if(dis_value<80)
											{
												RC_turn+=3;
											}
											else 
											{
												RC_turn=0;
											}
											
										}
									PWM=Vertical_Pid_Value(pitch,gy)+Velocity_Pid_Value(Now_Velocity,RC_vel);
									Turn_Value=Turn_Pid_value(RC_turn,gy);
									PWM_XianFU(6800,&PWM);			
									motor_Right_set(PWM+Turn_Value);
									motor_Left_set(PWM-Turn_Value);	
							}
				}
		}
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}



	