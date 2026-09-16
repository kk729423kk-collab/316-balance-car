#include "stm32f10x.h"                  // Device header
#include "Serial.h"

uint8_t BlueTooth_dat;
uint8_t rec_flag;
uint8_t Fore,Back,Left,Right;

extern float TurnKd,VerKp,VerKd,VelKp,VelKi;
extern uint8_t para_index;
extern uint8_t Work_mode;
extern void Mode_GUI(void);
void BlueTooth_Init(uint32_t Baud)
{
	Serial_Init(Baud);
}
uint8_t  BlueTooth_Get_State(void)
{
	return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3);
}
void Vel_Para_change(float *a,float *b)
{
	*b=*a/200;
}
void USART2_IRQHandler(void)
{
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)		
	{
		rec_flag=1;
		BlueTooth_dat = USART_ReceiveData(USART2);	
		//Serial_SendByte(BlueTooth_dat);
		if(BlueTooth_dat=='S')Fore=0,Back=0,Left=0,Right=0;
		else if(BlueTooth_dat=='F')Fore=1,Back=0,Left=0,Right=0;
		else if(BlueTooth_dat=='B')Fore=0,Back=1,Left=0,Right=0;
		else if(BlueTooth_dat=='L')Fore=0,Back=0,Left=1,Right=0;
		else if(BlueTooth_dat=='R')Fore=0,Back=0,Left=0,Right=1;
		else if(BlueTooth_dat=='1')Fore=1,Back=0,Left=1,Right=0;
		else if(BlueTooth_dat=='2')Fore=1,Back=0,Left=0,Right=1;
		else if(BlueTooth_dat=='3')Fore=0,Back=1,Left=1,Right=0;
		else if(BlueTooth_dat=='4')Fore=0,Back=1,Left=0,Right=1;
		else Fore=0,Back=0,Left=0,Right=0;
		
		if(BlueTooth_dat=='H')
		{
			Work_mode=(Work_mode+1)%3;
			Mode_GUI();
		}
//		if(BlueTooth_dat=='a')VerKp+=10;
//		else if(BlueTooth_dat=='b')VerKp-=10;
////		
//		if(BlueTooth_dat=='c')VerKd+=0.01;
//		else if(BlueTooth_dat=='d')VerKd-=0.01;
////		
//		if(BlueTooth_dat=='e')
//		{
//			VelKp+=5;
//			Vel_Para_change(&VelKp,&VelKi);
//		}
//		else if(BlueTooth_dat=='f')
//		{
//			VelKp-=5;
//			Vel_Para_change(&VelKp,&VelKi);
//		}
//		

		if(para_index == 0)
		{
			if(BlueTooth_dat=='a')VerKp+=10;
			else if(BlueTooth_dat=='b')VerKp-=10;
		}
		if(para_index == 1)
		{
			if(BlueTooth_dat=='a')VerKd+=0.01;
			else if(BlueTooth_dat=='b')VerKd-=0.01;
		}
		if(para_index == 2|para_index == 3)
		{
						if(BlueTooth_dat=='a')
				{
					VelKp+=5;
					Vel_Para_change(&VelKp,&VelKi);
				}
				else if(BlueTooth_dat=='b')
				{
					VelKp-=5;
					Vel_Para_change(&VelKp,&VelKi);
				}
		}
		if(para_index == 4)
		{
						if(BlueTooth_dat=='a')
				{
					TurnKd+=0.1;
				}
				else if(BlueTooth_dat=='b')
				{
					TurnKd-=0.1;
				}
		}
		
//		Serial_Printf("VerKp:%.2f VerKd:%.2f\r\n",VerKp,VerKd);
//		Serial_Printf("VelKp:%.2f VelKi:%.2f\r\n",VelKp,VelKi);
		
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);			
		
	}
}
