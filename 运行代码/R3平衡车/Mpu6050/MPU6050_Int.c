#include "MPU6050_Int.h"
#include "Encoder.h"
#include "MPU6050.h"
#include "inv_mpu.h"
#include "control.h"
#include "motor.h"


void MPU6050_Int_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitTypeDef gpio_struct;
	gpio_struct.GPIO_Mode=GPIO_Mode_IPU;
	gpio_struct.GPIO_Pin=GPIO_Pin_0;
	gpio_struct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio_struct);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);
	
	EXTI_InitTypeDef exti_struct;
	exti_struct.EXTI_Line=EXTI_Line0;
	exti_struct.EXTI_LineCmd=ENABLE;
	exti_struct.EXTI_Mode=EXTI_Mode_Interrupt;
	exti_struct.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&exti_struct);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef nvic_struct;
	nvic_struct.NVIC_IRQChannel=EXTI0_IRQn;
	nvic_struct.NVIC_IRQChannelCmd=ENABLE;
	nvic_struct.NVIC_IRQChannelPreemptionPriority=1;
	nvic_struct.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&nvic_struct);
}
