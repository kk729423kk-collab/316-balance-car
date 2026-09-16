#include "stm32f10x.h"                  // Device header
#include "delay.h"
uint64_t time=0;
uint64_t time_end=0;
uint64_t bug_time=0;
uint64_t num=0;
uint16_t time_10us;
void SR04_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	
	GPIO_InitTypeDef gpio_struct;
	gpio_struct.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio_struct.GPIO_Pin=GPIO_Pin_9;
	gpio_struct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio_struct);
	
	gpio_struct.GPIO_Mode=GPIO_Mode_IPU;
	gpio_struct.GPIO_Pin=GPIO_Pin_8;
	GPIO_Init(GPIOA,&gpio_struct);
	GPIO_WriteBit(GPIOA,GPIO_Pin_8,(BitAction)RESET);
	TIM_TimeBaseInitTypeDef base_struct;
	base_struct.TIM_ClockDivision=TIM_CKD_DIV1 ;
	base_struct.TIM_CounterMode=TIM_CounterMode_Up;
	base_struct.TIM_Period=10-1;
	base_struct.TIM_Prescaler=72-1;
	base_struct.TIM_RepetitionCounter=0;
	base_struct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2,&base_struct);
	
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);			//清除更新中断标志位
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef nvic_strcut;
	nvic_strcut.NVIC_IRQChannel=TIM2_IRQn ;
	nvic_strcut.NVIC_IRQChannelCmd=ENABLE;
	nvic_strcut.NVIC_IRQChannelPreemptionPriority=0;
	nvic_strcut.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&nvic_strcut);
	TIM_Cmd(TIM2,ENABLE);
}
void  TIM2_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM2,TIM_IT_Update)==SET)
	{
		bug_time++;
		time++;//10us加一次
		num++;
		time_10us++;
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}
 uint16_t Distance_Get(void)
{
		uint32_t dis=0;
		GPIO_SetBits(GPIOA,GPIO_Pin_9);
		Delay_us(10);
		GPIO_ResetBits(GPIOA,GPIO_Pin_9);
		TIM_Cmd(TIM2,ENABLE);
		TIM_SetCounter(TIM2,0);
		bug_time=0;
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)==RESET)
		{
			if(bug_time>2000)
				break;
		}
		TIM_Cmd(TIM2,DISABLE);
		TIM_SetCounter(TIM2,0);
		time=0;
		TIM_Cmd(TIM2,ENABLE);
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)==SET);
		TIM_Cmd(TIM2,DISABLE);
		if(time/100<38)
		{
				dis=(time*346)/2;
			dis=dis/100;
			
		}
		return dis;
}
