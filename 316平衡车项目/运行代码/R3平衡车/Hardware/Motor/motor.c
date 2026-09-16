#include "stm32f10x.h"                  // Device header
#include "PWM.h"
void motor_init(uint32_t psr,uint32_t arr)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//开启GPIOA的时钟
	//打开GPIO口时钟，先打开复用才能修改是否停用复用功能
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
//关闭JTAG，使能SWD
GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);						//将PA4和PA5引脚初始化为推挽输出	
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	PWM_Init(psr,arr)	;	
}
void motor_Right_set(int speed)
{
	if(speed>=0)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_4);
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
		TIM_SetCompare3(TIM1,speed);
	}
	else
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
		TIM_SetCompare3(TIM1,-speed);
	}
}
void motor_Left_set(int speed)
{
	if(speed>=0)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_5);
		GPIO_ResetBits(GPIOB,GPIO_Pin_4);
		TIM_SetCompare4(TIM1,speed);
	}
	else
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_4);
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);
		TIM_SetCompare4(TIM1,-speed);
	}
}



	