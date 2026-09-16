#include "stm32f10x.h"                  // Device header

void Encoder_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef gpio_struct;
	
	gpio_struct.GPIO_Mode=GPIO_Mode_IPU;
	gpio_struct.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	gpio_struct.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&gpio_struct);
	GPIO_Init(GPIOB,&gpio_struct);
	
	TIM_TimeBaseInitTypeDef tim_struct;
	TIM_TimeBaseStructInit(&tim_struct);
	tim_struct.TIM_ClockDivision=TIM_CKD_DIV1;
	tim_struct.TIM_CounterMode=TIM_CounterMode_Up;
	tim_struct.TIM_Period=65536-1;
	tim_struct.TIM_Prescaler=1-1;
	TIM_TimeBaseInit(TIM3,&tim_struct);
	TIM_TimeBaseInit(TIM4,&tim_struct);
	
	TIM_ICInitTypeDef ic_struct;
	TIM_ICStructInit(&ic_struct);
	
	ic_struct.TIM_Channel=TIM_Channel_1|TIM_Channel_2;
	ic_struct.TIM_ICFilter=0xf;
	TIM_ICInit(TIM3,&ic_struct);
	TIM_ICInit(TIM4,&ic_struct);
	
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	TIM_Cmd(TIM3,ENABLE);
	TIM_Cmd(TIM4,ENABLE);
}
int16_t Encoder1_Get(void)
{
	uint16_t temp;
	temp=TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3,0);
	return temp;
}
int16_t Encoder2_Get(void)
{
	uint16_t temp;
	temp=TIM_GetCounter(TIM4);
	TIM_SetCounter(TIM4,0);
	return temp;

}
