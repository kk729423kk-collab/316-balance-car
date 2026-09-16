#include "stm32f10x.h"                  // Device header

void Beep_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能PC端口时钟
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
		GPIO_InitTypeDef gpio_type;
    gpio_type.GPIO_Pin = GPIO_Pin_15;//端口配置
		gpio_type.GPIO_Mode=GPIO_Mode_Out_PP;
		gpio_type.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(GPIOA,&gpio_type);
		GPIO_ResetBits(GPIOA,GPIO_Pin_15);
	
}
void Beep_On(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_15);
}

void Beep_Off(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_15);
}
void Beep_Turn(void)
{
	if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_15))
	{
			GPIO_ResetBits(GPIOA, GPIO_Pin_15);	
	}
	else 
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_15);
	}
}


/*
#include "stm32f10x.h"                  // Device header

void Beep_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //使能PC端口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//使能AFIO时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;//端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;     //2M
    GPIO_Init(GPIOC, &GPIO_InitStructure);                          //根据设定参数初始化GPIO 

    PWR_BackupAccessCmd(ENABLE);//允许修改RTC 和后备寄存器
    RCC_LSEConfig(RCC_LSE_OFF);//关闭外部低速外部时钟信号功能 后，PC13 PC14 PC15 才可以当普通IO用。
    BKP_TamperPinCmd(DISABLE);//关闭入侵检测功能，也就是 PC13，也可以当普通IO 使用
    PWR_BackupAccessCmd(DISABLE);//禁止修改后备寄存器

	
}
void Beep_On(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
}

void Beep_Off(void)
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
}



*/
