#include "stm32f10x.h"                  // Device header


void PWM_Init(uint32_t psr,uint32_t arr)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef gpio_struct;
	gpio_struct.GPIO_Mode=GPIO_Mode_AF_PP;
	gpio_struct.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11;
	gpio_struct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio_struct);
	
	
	TIM_TimeBaseInitTypeDef tim_struct;
	tim_struct.TIM_ClockDivision=TIM_CKD_DIV1;
	tim_struct.TIM_CounterMode=TIM_CounterMode_Up;
	tim_struct.TIM_Period=arr;
	tim_struct.TIM_Prescaler=psr;
	tim_struct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1,&tim_struct);
	
	TIM_OCInitTypeDef oc_struct;
	TIM_OCStructInit(&oc_struct); 
	
	oc_struct.TIM_OCMode=TIM_OCMode_PWM1;
	
	oc_struct.TIM_OCPolarity=TIM_OCPolarity_High;
	oc_struct.TIM_OutputState=TIM_OutputState_Enable;
	oc_struct.TIM_OCIdleState= TIM_OCIdleState_Reset;
	oc_struct.TIM_OCMode=TIM_OCMode_PWM1;
	oc_struct.TIM_Pulse=0;
	
	
	TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);
	TIM_OC3Init(TIM1,&oc_struct);
	
	TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable);
	TIM_OC4Init(TIM1,&oc_struct);
	
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	TIM_Cmd(TIM1,ENABLE);
}
