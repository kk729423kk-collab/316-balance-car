#include "stm32f10x.h"                  // Device header
#include "Serial.h"

uint8_t HC05_dat;
uint8_t rec_flag;
uint8_t Fore,Back,Left,Right;
extern uint8_t mode;
extern float TurnKd;
void HC05_init(void)
{
	Serial_Init(9600);
}
void USART2_IRQHandler(void)
{
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)		
	{
		rec_flag=1;
		HC05_dat = USART_ReceiveData(USART2);	
		Serial_SendByte(HC05_dat);
		if(HC05_dat=='S')Fore=0,Back=0,Left=0,Right=0;
		else if(HC05_dat=='F')Fore=1,Back=0,Left=0,Right=0;
		else if(HC05_dat=='B')Fore=0,Back=1,Left=0,Right=0;
		else if(HC05_dat=='L')Fore=0,Back=0,Left=1,Right=0;
		else if(HC05_dat=='R')Fore=0,Back=0,Left=0,Right=1;
		else Fore=0,Back=0,Left=0,Right=0;
		
		if(HC05_dat=='H')mode=(mode + 1)%2;
//		if(HC05_dat=='m')TurnKd+=0.01;
//		else if(HC05_dat=='c')TurnKd-=0.01;
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);			
		
	}
}
